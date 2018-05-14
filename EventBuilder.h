#ifndef EVENTBUILDER_H
#define EVENTBUILDER_H

#include <vector>

class TFile;
class TTree;
class Geometry;

class EventBuilder {
public:
    EventBuilder(const char* filename, const char* outname);
    virtual ~EventBuilder();

    void InitInput(const char* filename);
    void InitOutput(const char* outname);
    void Scan(long n=-1);
    bool Keep();

    TFile *rootFile;
    TFile *outFile;
    TTree *T;
    Geometry *geo;

    long currentEntry;
    long currentEvent;
    long lastEvent;
    double t_last;

    Long64_t evt;
    int seg;
    float E;
    double t;
    float z;
    float PSD;

    //output
    TTree *T_out;
    std::vector<int> vSeg;
    std::vector<float> vE;
    std::vector<float> vZ;
    std::vector<float> vPSD;
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
    float mPSD;

    //output run info
    TTree *T_run;
    double runtime;
    double abstime;

    std::vector<double> t_v;


private:
    void Collect();
    void Reset();
    void BuildCurrentEvent();
};

#endif