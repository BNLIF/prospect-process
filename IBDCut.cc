#include "IBDCut.h"
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

IBDCut::IBDCut(const char* filename, const char* outname)
{
    InitInput(filename);
    // InitOutput(outname);
}

void IBDCut::InitInput(const char* filename)
{
    fE = new vector<float>;
    fT = new vector<double>;
    fPSD = new vector<float>;
    fNSeg = new vector<int>;
    fEvt = new vector<Long64_t>;

    fmSeg = new vector<int>;
    fmE = new vector<float>;
    fmX = new vector<float>;
    fmY = new vector<float>;
    fmZ = new vector<float>;
    fmPSD = new vector<float>;

    fvSeg = new vector< vector<int>>;
    fvE = new vector< vector<float>>;
    fvZ = new vector< vector<float>>;
    fvPSD = new vector< vector<float>>;

    I = new vector<int>;
    J = new vector<int>;

    rootFile = TFile::Open(filename);
    if (!rootFile) {
        string msg = "Unable to open ";
        msg += filename;
        throw runtime_error(msg.c_str());
    }

    T = (TTree*)rootFile->Get("Event");

    T->SetBranchAddress("fE", &fE);
    T->SetBranchAddress("fT", &fT);
    T->SetBranchAddress("fPSD", &fPSD);
    T->SetBranchAddress("fEvt", &fEvt);
    T->SetBranchAddress("fNSeg", &fNSeg);

    T->SetBranchAddress("fmSeg", &fmSeg);
    T->SetBranchAddress("fmE", &fmE);
    T->SetBranchAddress("fmX", &fmX);
    T->SetBranchAddress("fmY", &fmY);
    T->SetBranchAddress("fmZ", &fmZ);
    T->SetBranchAddress("fmPSD", &fmPSD);

    T->SetBranchAddress("fvSeg", &fvSeg);
    T->SetBranchAddress("fvE", &fvE);
    T->SetBranchAddress("fvZ", &fvZ);
    T->SetBranchAddress("fvPSD", &fvPSD);

    T->SetBranchAddress("I", &I);
    T->SetBranchAddress("J", &J);
    T->SetBranchAddress("ts_runstart", &ts_runstart);


}

// void IBDCut::InitOutput(const char* outname)
// {
//     outFile = new TFile(outname, "recreate");
//     T_out = new TTree("Event", "Event Tree");

//     T_out->Branch("fE", &fE);
//     T_out->Branch("fT", &fT);
//     T_out->Branch("fPSD", &fPSD);
//     T_out->Branch("fEvt", &fEvt);
//     T_out->Branch("fNSeg", &fNSeg);

//     T_out->Branch("fmSeg", &fmSeg);
//     T_out->Branch("fmE", &fmE);
//     T_out->Branch("fmX", &fmX);
//     T_out->Branch("fmY", &fmY);
//     T_out->Branch("fmZ", &fmZ);
//     T_out->Branch("fmPSD", &fmPSD);

//     T_out->Branch("fvSeg", &fvSeg);
//     T_out->Branch("fvE", &fvE);
//     T_out->Branch("fvZ", &fvZ);
//     T_out->Branch("fvPSD", &fvPSD);

//     T_out->Branch("I", &I);
//     T_out->Branch("J", &J);
//     T_out->Branch("ts_runstart", &ts_runstart);

//     TTree *T_run = (TTree*)rootFile->Get("Run");
//     outFile->cd();
//     T_run->CloneTree()->Write();
// }

// void IBDCut::Scan(int n)
// {
//     Long64_t totalEntry = T->GetEntries();
//     int maxEntry = n;
//     if (n<0) { maxEntry = totalEntry; }

//     double t_prev = 0;
//     int nCandidate = 0;

//     for (int i=0; i<maxEntry; i++) {
//         GetEntry(i);

//         if ( ((t0-t_prev)*1e-6<GAP_WINDOW && i!=maxEntry-1) || i==0) {
//             Collect();
//         }
//         else {
//             SaveEvent();
//             nCandidate++;
//             // cout << "end at " << (t0-t_prev)*1e-6 << " " << nCandidate << endl;
//         }
//         t_prev = t0;
//     }

//     T_out->Write(0,TObject::kOverwrite);
//     outFile->Close();

//     cout << endl << "total entry: " << totalEntry << endl;
//     cout << "coincidence candidates: " << nCandidate << endl;
// }



// void IBDCut::ResetOutput()
// {
//     fE->clear();
//     fT.clear();
//     fPSD.clear();
//     fEvt.clear();
//     fNSeg.clear();

//     fmSeg.clear();
//     fmE.clear();
//     fmX.clear();
//     fmY.clear();
//     fmZ.clear();
//     fmPSD.clear();

//     fvSeg.clear();
//     fvE.clear();
//     fvZ.clear();
//     fvPSD.clear();

//     I.clear();
//     J.clear();
// }


// void IBDCut::SaveEvent()
// {

//     T_out->Fill();
//     ResetOutput();
//     Collect(); // start new one
// }

// bool IBDCut::IsnLi()
// {
//     return wPSD>0.22&&wPSD<0.35&&E_total>0.40&&E_total<0.65;
// }

void IBDCut::GetEntry(Long64_t i)
{
    Reset();
    T->GetEntry(i);
}

void IBDCut::Reset()
{
    fE->clear();
    fT->clear();
    fPSD->clear();
    fEvt->clear();
    fNSeg->clear();

    fmSeg->clear();
    fmE->clear();
    fmX->clear();
    fmY->clear();
    fmZ->clear();
    fmPSD->clear();

    fvSeg->clear();
    fvE->clear();
    fvZ->clear();
    fvPSD->clear();

    I->clear();
    J->clear();
}



IBDCut::~IBDCut()
{

}