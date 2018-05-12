#include "EventBuilder.h"
#include "Geometry.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TVectorT.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;

EventBuilder::EventBuilder(const char* filename, const char* outname)
{
    geo = new Geometry();
    InitInput(filename);
    InitOutput(outname);
}

void EventBuilder::InitInput(const char* filename)
{
    rootFile = TFile::Open(filename);
    if (!rootFile) {
        string msg = "Unable to open ";
        msg += filename;
        throw runtime_error(msg.c_str());
    }

    T = (TTree*)rootFile->Get("PhysPulse");
    T->SetBranchStatus("*",0);

    T->SetBranchStatus("evt",1);
    T->SetBranchAddress("evt",&evt);

    T->SetBranchStatus("seg",1);
    T->SetBranchAddress("seg",&seg);

    T->SetBranchStatus("E",1);
    T->SetBranchAddress("E",&E);

    T->SetBranchStatus("t",1);
    T->SetBranchAddress("t",&t);

    T->SetBranchStatus("z",1);
    T->SetBranchAddress("z",&z);

    T->SetBranchStatus("PSD",1);
    T->SetBranchAddress("PSD",&PSD);

    currentEntry = 0;
    currentEvent = -1;
    t_last = 0;
}

void EventBuilder::InitOutput(const char* outname)
{
    Reset();

    outFile = new TFile(outname, "recreate");
    T_out = new TTree("Event", "Event Tree");
    T_out->Branch("evt", &evt);
    T_out->Branch("E_total", &E_total);
    T_out->Branch("wPSD", &wPSD);
    T_out->Branch("t0", &t0);
    T_out->Branch("ts_runstart", &ts_runstart);

    T_out->Branch("mE", &mE);
    T_out->Branch("mSeg", &mSeg);
    T_out->Branch("mX", &mX);
    T_out->Branch("mY", &mY);
    T_out->Branch("mZ", &mZ);
    T_out->Branch("mPSD", &mPSD);

    T_out->Branch("vSeg", &vSeg);
    T_out->Branch("vE", &vE);
    T_out->Branch("vZ", &vZ);
    T_out->Branch("vPSD", &vPSD);

    T_run = new TTree("Run", "Run Info Tree");
    T_run->Branch("runtime", &runtime);
    T_run->Branch("abstime", &abstime);
    runtime = ((TVectorT<double>*)rootFile->Get("runtime"))->Max();
    abstime = ((TVectorT<double>*)rootFile->Get("abstime"))->Max();
    T_run->Fill();
    T_run->Write();
}

void EventBuilder::Scan(int n)
{
    for (int i=0; i<n; i++) {
        T->GetEntry(currentEntry);
        if (currentEvent!=evt) {
            if(Keep()) {
                BuildCurrentEvent();
                T_out->Fill();
            }
            Reset();
            currentEvent = evt;
        }
        Collect();
        currentEntry++;
    }

    T_out->Write(0,TObject::kOverwrite);
    outFile->Close();
}

bool EventBuilder::Keep()
{
    int size = vSeg.size();
    if (size==0) return false; // skip first
    // else if (vSeg[0]<0 || vSeg[0]>300) return false;

    return true;
}

void EventBuilder::BuildCurrentEvent()
{
    int nSeg = vSeg.size();

    mE = -100;
    int i_m = -100;

    for (int i=0; i<nSeg; i++) {
        float E_i = vE[i];
        float PSD_i = vPSD[i];
        E_total += E_i;
        wPSD += E_i * PSD_i;

        if (E_i>mE) {
            mE = E_i;
            i_m = i;
        }
    }

    wPSD /= E_total;
    t0 = t_v[0];
    ts_runstart = abstime;
    // dt_last = t0 - t_last;
    t_last = t0;
    // cout << vSeg.size() << endl;

    mZ = vZ[i_m]/1000.;
    mSeg = vSeg[i_m];
    mPSD = vPSD[i_m];
    mX = geo->SegX(mSeg);
    mY = geo->SegY(mSeg);

}

void EventBuilder::Collect()
{
    vSeg.push_back(seg);
    vE.push_back(E);
    vZ.push_back(z);
    vPSD.push_back(PSD);
    t_v.push_back(t);
}

void EventBuilder::Reset()
{
    vSeg.clear();
    vE.clear();
    vZ.clear();
    vPSD.clear();
    t_v.clear();
    E_total = 0;
    wPSD = 0;
    t0 = 0;

    mE=-100;
    mSeg=-100;
    mX=-100;
    mY=-100;
    mZ=-100;
    mPSD=0;
}


EventBuilder::~EventBuilder()
{
}
