#include "BundlenLi.h"
// #include "Geometry.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TEntryList.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;

BundlenLi::BundlenLi(const char* filename, const char* outname)
{
    GAP_WINDOW = 1; // ms

    InitInput(filename);
    InitOutput(outname);
}

void BundlenLi::InitInput(const char* filename)
{
    vSeg = new vector<int>;
    vE = new vector<float>;
    vZ = new vector<float>;
    vPSD = new vector<float>;

    rootFile = TFile::Open(filename);
    if (!rootFile) {
        string msg = "Unable to open ";
        msg += filename;
        throw runtime_error(msg.c_str());
    }

    T = (TTree*)rootFile->Get("Event");

    T->SetBranchAddress("evt", &evt);
    T->SetBranchAddress("E_total", &E_total);
    T->SetBranchAddress("wPSD", &wPSD);
    T->SetBranchAddress("t0", &t0);
    T->SetBranchAddress("ts_runstart", &ts_runstart);

    T->SetBranchAddress("mE", &mE);
    T->SetBranchAddress("mSeg", &mSeg);
    T->SetBranchAddress("mX", &mX);
    T->SetBranchAddress("mY", &mY);
    T->SetBranchAddress("mZ", &mZ);
    T->SetBranchAddress("mPSD", &mPSD);

    T->SetBranchAddress("vSeg", &vSeg);
    T->SetBranchAddress("vE", &vE);
    T->SetBranchAddress("vZ", &vZ);
    T->SetBranchAddress("vPSD", &vPSD);
}

void BundlenLi::InitOutput(const char* outname)
{
    outFile = new TFile(outname, "recreate");
    T_out = new TTree("Event", "Event Tree");

    T_out->Branch("fE", &fE);
    T_out->Branch("fT", &fT);
    T_out->Branch("fPSD", &fPSD);
    T_out->Branch("fEvt", &fEvt);
    T_out->Branch("fNSeg", &fNSeg);

    T_out->Branch("fmSeg", &fmSeg);
    T_out->Branch("fmE", &fmE);
    T_out->Branch("fmX", &fmX);
    T_out->Branch("fmY", &fmY);
    T_out->Branch("fmZ", &fmZ);
    T_out->Branch("fmPSD", &fmPSD);

    T_out->Branch("fvSeg", &fvSeg);
    T_out->Branch("fvE", &fvE);
    T_out->Branch("fvZ", &fvZ);
    T_out->Branch("fvPSD", &fvPSD);

    T_out->Branch("I", &I);
    T_out->Branch("J", &J);
    T_out->Branch("ts_runstart", &ts_runstart);

    TTree *T_run = (TTree*)rootFile->Get("Run");
    outFile->cd();
    T_run->CloneTree()->Write();
}

void BundlenLi::Scan(int n)
{
    Long64_t totalEntry = T->GetEntries();
    int maxEntry = n;
    if (n<0) { maxEntry = totalEntry; }

    double t_prev = 0;
    int nCandidate = 0;

    for (int i=0; i<maxEntry; i++) {
        GetEntry(i);

        if ( ((t0-t_prev)*1e-6<GAP_WINDOW && i!=maxEntry-1) || i==0) {
            Collect();
        }
        else {
            SaveEvent();
            nCandidate++;
            // cout << "end at " << (t0-t_prev)*1e-6 << " " << nCandidate << endl;
        }
        t_prev = t0;
    }

    T_out->Write(0,TObject::kOverwrite);
    outFile->Close();

    cout << endl << "total entry: " << totalEntry << endl;
    cout << "coincidence candidates: " << nCandidate << endl;
}


void BundlenLi::Collect()
{
    int ind = fE.size();
    if (IsnLi()) {
        J.push_back(ind);
    }
    else {
        I.push_back(ind);
    }
    fE.push_back(E_total);
    fT.push_back(t0);
    fPSD.push_back(wPSD);
    fEvt.push_back(evt);
    fNSeg.push_back(int(vSeg->size()));

    fmSeg.push_back(mSeg);
    fmE.push_back(mE);
    fmX.push_back(mX);
    fmY.push_back(mY);
    fmZ.push_back(mZ);
    fmPSD.push_back(mPSD);

    fvSeg.push_back(*vSeg);
    fvE.push_back(*vE);
    fvZ.push_back(*vZ);
    fvPSD.push_back(*vPSD);


}

void BundlenLi::ResetOutput()
{
    fE.clear();
    fT.clear();
    fPSD.clear();
    fEvt.clear();
    fNSeg.clear();

    fmSeg.clear();
    fmE.clear();
    fmX.clear();
    fmY.clear();
    fmZ.clear();
    fmPSD.clear();

    fvSeg.clear();
    fvE.clear();
    fvZ.clear();
    fvPSD.clear();

    I.clear();
    J.clear();
}


void BundlenLi::SaveEvent()
{

    T_out->Fill();
    ResetOutput();
    Collect(); // start new one
}

bool BundlenLi::IsnLi()
{
    return wPSD>0.22&&wPSD<0.35&&E_total>0.40&&E_total<0.65;
}

void BundlenLi::GetEntry(Long64_t i)
{
    Reset();
    T->GetEntry(i);
}

void BundlenLi::Reset()
{
    vSeg->clear();
    vE->clear();
    vZ->clear();
    vPSD->clear();
}



BundlenLi::~BundlenLi()
{

}