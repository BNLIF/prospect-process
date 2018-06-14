#ifndef IBDCUT_H
#define IBDCUT_H

#include <vector>

class TFile;
class TTree;

class IBDCut {
public:
    IBDCut(const char* filename, const char* outname);
    virtual ~IBDCut();

    void InitInput(const char* filename);
    // void InitOutput(const char* outname);
    // void Scan(int n=-1);
    // void Collect();
    // void SaveEvent();

    TFile *rootFile;
    TFile *outFile;
    TTree *T;
    TTree *T_out;

    //input
    std::vector<float> *fE;
    std::vector<double> *fT;
    std::vector<float> *fPSD;
    std::vector<int> *fNSeg;
    std::vector<Long64_t> *fEvt;

    std::vector<int> *fmSeg;
    std::vector<float> *fmE;
    std::vector<float> *fmX;
    std::vector<float> *fmY;
    std::vector<float> *fmZ;
    std::vector<float> *fmPSD;

    std::vector< std::vector<int>> *fvSeg;
    std::vector< std::vector<float>> *fvE;
    std::vector< std::vector<float>> *fvZ;
    std::vector< std::vector<float>> *fvPSD;

    std::vector<int> *I;
    std::vector<int> *J;
    int ts_runstart;

private:
    void Reset();
    // void ResetOutput();
    void GetEntry(Long64_t i);
    // bool IsnLi();
};

#endif