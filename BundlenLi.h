#ifndef BUNDLENLI_H
#define BUNDLENLI_H

#include <vector>

class TFile;
class TTree;

class BundlenLi {
public:
    BundlenLi(const char* filename, const char* outname);
    virtual ~BundlenLi();

    void InitInput(const char* filename);
    void InitOutput(const char* outname);
    void Scan(int n=-1);
    void Collect();
    void SaveEvent();

    TFile *rootFile;
    TFile *outFile;
    double GAP_WINDOW; // max gap between events

    //input
    TTree *T;
    TTree *T_out;
    std::vector<int> *vSeg;
    std::vector<float> *vE;
    std::vector<float> *vZ;
    std::vector<float> *vPSD;
    Long64_t evt;
    float E_total;
    float wPSD;
    double t0;
    int ts_runstart;
    float mE;
    int mSeg;
    float mX;
    float mY;
    float mZ;
    float mPSD;

    //output
    std::vector<float> fE;
    std::vector<double> fT;
    std::vector<float> fPSD;
    std::vector<int> fNSeg;
    std::vector<Long64_t> fEvt;

    std::vector<int> fmSeg;
    std::vector<float> fmE;
    std::vector<float> fmX;
    std::vector<float> fmY;
    std::vector<float> fmZ;
    std::vector<float> fmPSD;

    std::vector< std::vector<int>> fvSeg;
    std::vector< std::vector<float>> fvE;
    std::vector< std::vector<float>> fvZ;
    std::vector< std::vector<float>> fvPSD;

    std::vector<int> I;
    std::vector<int> J;

private:
    void Reset();
    void ResetOutput();
    void GetEntry(Long64_t i);
    bool IsnLi();
};

#endif