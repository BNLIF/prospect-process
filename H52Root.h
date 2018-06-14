#ifndef H52ROOT_H
#define H52ROOT_H

#include <vector>
#include <H5Cpp.h>

class TFile;
class TTree;

typedef struct{
    Long64_t evt;
    Int_t seg;
    Float_t E;
    Double_t t;
    Float_t dt;
    Float_t PE1;
    Float_t PE2;
    Float_t z;
    Float_t PSD;
    Int_t PID;
} HRecord;

class H52Root {
public:
    H52Root(const char* filename, const char* outname);
    virtual ~H52Root();

    void InitInput(const char* filename);
    void InitOutput(const char* outname);
    void Scan(Long64_t n=-1);

    H5::H5File *inFile;
    TFile *outFile;
    HRecord data;

    //output
    TTree *T_out;
    Long64_t evt;
    Double_t t0;
    Int_t ts_runstart;
    std::vector<Int_t> vSeg;
    std::vector<Float_t> vE;
    std::vector<Float_t> vZ;
    std::vector<Float_t> vPSD;
    std::vector<Int_t> vPID;

    std::vector<Double_t> vT;
    Long64_t prevEvent;

    void Print();

private:
    void Fill(bool isLast=false);
    void Collect();
    void Reset();

    char map_h5type_to_root(H5::DataType type);

};



#endif