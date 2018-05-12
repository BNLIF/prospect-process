#ifndef SKIMNLI_H
#define SKIMNLI_H

#include <vector>

class TFile;
class TTree;
class Geometry;

class SkimnLi {
public:
    SkimnLi(const char* filename, const char* outname);
    virtual ~SkimnLi();

    void InitInput(const char* filename);
    void InitOutput(const char* outname);
    void Scan(int n=-1);
    int SaveEvent(Long64_t entryNo, double dt_low, double dt_high, Long64_t firstEntry, Long64_t lastEntry);

    // bool Keep();

    TFile *rootFile;
    TFile *outFile;
    Geometry *geo;
    double DT_WINDOW; // one-side time window in ms

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
    // double dt_last;
    float mE;
    int mSeg;
    float mX;
    float mY;
    float mZ;

private:
    // void Collect();
    void Reset();
    void GetEntry(Long64_t i);
    // void BuildCurrentEvent();
};

#endif