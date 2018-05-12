#include "SkimnLi.h"
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

SkimnLi::SkimnLi(const char* filename, const char* outname)
{
    DT_WINDOW = 1; // ms

    // geo = new Geometry();

    InitInput(filename);
    InitOutput(outname);
}

void SkimnLi::InitInput(const char* filename)
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
    // T->SetBranchAddress("dt_last", &dt_last);

    T->SetBranchAddress("mE", &mE);
    T->SetBranchAddress("mSeg", &mSeg);
    T->SetBranchAddress("mX", &mX);
    T->SetBranchAddress("mY", &mY);
    T->SetBranchAddress("mZ", &mZ);

    T->SetBranchAddress("vSeg", &vSeg);
    T->SetBranchAddress("vE", &vE);
    T->SetBranchAddress("vZ", &vZ);
    T->SetBranchAddress("vPSD", &vPSD);
}

void SkimnLi::InitOutput(const char* outname)
{
    outFile = new TFile(outname, "recreate");
    T_out = new TTree("Event", "Event Tree");
    T_out->Branch("evt", &evt);
    T_out->Branch("E_total", &E_total);
    T_out->Branch("wPSD", &wPSD);
    T_out->Branch("t0", &t0);
    T_out->Branch("ts_runstart", &ts_runstart);
    // T_out->Branch("dt_last", &dt_last);

    T_out->Branch("vSeg", vSeg);
    T_out->Branch("vE", vE);
    T_out->Branch("vZ", vZ);
    T_out->Branch("vPSD", vPSD);

    T_out->Branch("mE", &mE);
    T_out->Branch("mSeg", &mSeg);
    T_out->Branch("mX", &mX);
    T_out->Branch("mY", &mY);
    T_out->Branch("mZ", &mZ);

    TTree *T_run = (TTree*)rootFile->Get("Run");
    outFile->cd();
    T_run->CloneTree()->Write();
}

void SkimnLi::Scan(int n)
{
    Long64_t totalEntry = T->GetEntries();
    int maxEvent = n;
    TCut isnLi("wPSD>0.22&&wPSD<0.35&&E_total>0.40&&E_total<0.65");
    T->Draw(">>nLiList", isnLi, "entrylist");
    TEntryList *list = (TEntryList*)gDirectory->FindObject("nLiList");
    int nEvents = list->GetN();
    int i=0;
    Long64_t firstEntry = 0;
    Long64_t lastEntry = 0;
    int nSavedEvents = 0;
    if (n<0) { maxEvent = nEvents; }

    Long64_t nextEntry = 0;
    Long64_t currentEntry = 0;
    double t_next = 0;
    double dt_ms = 0;

    while (i<maxEvent) {
        // int span_events = 0;
        if (i%1000 == 0) {cout << i << " nLi events out of " << maxEvent << " done." << endl; }
        double span_window = 0;

        currentEntry = list->GetEntry(i);
        GetEntry(currentEntry);
        double t_this = t0;

        i++;
        if (i==maxEvent) {lastEntry = totalEntry; goto SAVE;}
        nextEntry = list->GetEntry(i);
        GetEntry(nextEntry);
        t_next = t0;
        dt_ms = (t_next - t_this)* 1e-6;

        while (dt_ms<DT_WINDOW) {
            // cout << "add event " << list->GetEntry(i+1+span_events) << " by " << dt_ms << " ms" << endl;
            span_window += dt_ms;
            // span_events++;
            i++;
            if (i==maxEvent) {lastEntry = totalEntry; goto SAVE;}
            nextEntry = list->GetEntry(i);
            GetEntry(nextEntry);
            t_this = t_next;
            t_next = t0;
            dt_ms = (t_next - t_this)* 1e-6;
        }

        lastEntry = nextEntry;
SAVE:
        nSavedEvents += SaveEvent(currentEntry, DT_WINDOW, DT_WINDOW+span_window+1e-3, firstEntry, lastEntry);
        firstEntry = currentEntry;

        // cout << currentEntry << ": " << dt_ms << " ms, span " << span_events << ", " << span_window << " ms" << endl;
        // i = i+1+span_events;
    }
    cout << "total events: " << lastEntry << endl;
    cout << "total nLi events: " << nEvents << endl;
    cout << "saved events: " << nSavedEvents << endl;

    T_out->Write(0,TObject::kOverwrite);
    outFile->Close();
}

int SkimnLi::SaveEvent(Long64_t entryNo, double dt_low, double dt_high, Long64_t firstEntry, Long64_t lastEntry)
{
    GetEntry(entryNo);
    double t_this = t0;
    float x_this = mX;
    float y_this = mY;

    TCut timeCut(TString::Format(
        "(t0-%.6lf)*1e-6<%.6lf && (%.6lf-t0)*1e-6<%.6lf",
        t_this, dt_high, t_this, dt_low));
    TCut segCut(TString::Format(
        "abs(%.4f-mX)<0.3 && abs(%.4f-mY)<0.3",
        x_this, y_this));
    TCut ocsCut("E_total<1e4");
    TCut eCut("E_total>10");
    TCut psdCut("wPSD>0.22 && wPSD <0.35");
    TCut allCuts = timeCut && ocsCut && (segCut || eCut || psdCut);
    // TCut allCuts = timeCut && segCut;

    TEntryList *list = (TEntryList*)gDirectory->FindObject("candidateList");
    if (list) delete list;
    T->Draw(">>candidateList", allCuts, "entrylist", lastEntry-firstEntry-1, firstEntry);
    list = (TEntryList*)gDirectory->FindObject("candidateList");
    int nCandidate = list->GetN();
    // cout << entryNo << ": " << nCandidate << ", " << allCuts.GetTitle() << endl;
    // cout << evt << ": " << nCandidate << endl;

    for (int i=0; i<nCandidate; i++) {
        GetEntry(list->GetEntry(i));
        T_out->Fill();
    }
    return nCandidate;
}

void SkimnLi::GetEntry(Long64_t i)
{
    Reset();
    T->GetEntry(i);
}

void SkimnLi::Reset()
{
    vSeg->clear();
    vE->clear();
    vZ->clear();
    vPSD->clear();
}

SkimnLi::~SkimnLi()
{

}