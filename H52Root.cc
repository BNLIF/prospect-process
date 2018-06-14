#include "H52Root.h"

#include <H5Cpp.h>
using namespace H5;

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;

H52Root::H52Root(const char* filename, const char* outname)
{
    InitInput(filename);
    InitOutput(outname);
}

void H52Root::InitInput(const char* filename)
{
    inFile = new H5File(filename, H5F_ACC_RDONLY);
    string s(filename);
    int start = s.find("_ts") + 3;
    int end = s.find("_", start);
    ts_runstart = stoi(s.substr(start, end-start));
}

void H52Root::InitOutput(const char* outname)
{
    Reset();

    outFile = new TFile(outname, "recreate");
    T_out = new TTree("Event", "Event Tree");
    T_out->Branch("evt", &evt);
    T_out->Branch("t0", &t0);
    T_out->Branch("ts_runstart", &ts_runstart);
    T_out->Branch("vSeg", &vSeg);
    T_out->Branch("vE", &vE);
    T_out->Branch("vZ", &vZ);
    T_out->Branch("vPSD", &vPSD);
    T_out->Branch("vPID", &vPID);
}

void H52Root::Scan(Long64_t N)
{
    DataSet ds = inFile->openDataSet("PhysPulse");
    DataSpace dsp = ds.getSpace();
    // cout << "Rank: " << dsp.getSimpleExtentNdims() << "\n";

    const hssize_t nrecs = dsp.getSimpleExtentNpoints(); // # records
    const CompType type = ds.getCompType();
    const int nm = type.getNmembers(); // # fields
    const size_t twidth = type.getSize();
    // cout << nrecs << ", "
    // << nm << ", "
    // << twidth << endl;

    vector<size_t> offsets(nm);
    // vector<char> rflags(nm);
    for(int k = 0; k < nm; ++k) {
      offsets[k] = type.getMemberOffset(k);
      // rflags[k] = map_h5type_to_root(type.getMemberDataType(k));
      // description += type.getMemberName(k) + "/" + rflags[k] + ":";
    }


    hsize_t dims[] = { 1 };
    hsize_t count[] = { 1 };
    hsize_t offset[] = { 0 };
    DataSpace mem(1,dims);
    // hssize_t start[] = { 0 };
    // hssize_t end[] = { 0 };

    if (N==-1) { N = nrecs; }

    for(long i = 0; i < N; ++i){
        dsp.selectHyperslab(H5S_SELECT_SET, count, offset, 0, dims);
        ds.read(&data, type, mem, dsp);
        offset[0] += count[0];
        // Print();

        if (i == 0) {
            prevEvent = data.evt;
        }

        if (prevEvent != data.evt) {
            Fill();
            prevEvent = data.evt;
        }

        if (i == N-1) { // always save the last event
            Collect();
            Fill(true);
        }

        Collect();
    }

    T_out->Write(0, TObject::kOverwrite);
    outFile->Close();

}

void H52Root::Fill(bool isLast) {
    if (isLast) {
        evt = data.evt;
    }
    else {
        evt = prevEvent;
    }
    // cout << evt << endl;
    t0 = vT.at(0);
    T_out->Fill();
    Reset();
}



void H52Root::Collect()
{
    vSeg.push_back(data.seg);
    vE.push_back(data.E);
    vZ.push_back(data.z);
    vPSD.push_back(data.PSD);
    vPID.push_back(data.PID);
    vT.push_back(data.t);
}

void H52Root::Reset()
{
    vSeg.clear();
    vE.clear();
    vZ.clear();
    vPSD.clear();
    vPID.clear();
    vT.clear();
}


H52Root::~H52Root()
{
}

void H52Root::Print() {
    cout << data.evt << ", "
    << data.seg << ", "
    << data.E << ", "
    << data.t << ", "
    << data.z << ", "
    << data.PSD << ", "
    << data.PID << ", "
    << endl;
}


char H52Root::map_h5type_to_root(H5::DataType type) {
    // https://root-forum.cern.ch/t/hdf5-to-root/590

    if(type == H5::PredType::NATIVE_SHORT){
      return 'B';
    }
    if(type == H5::PredType::NATIVE_USHORT){
      return 'b';
    }
    if(type == H5::PredType::NATIVE_INT){
      if(type.getSize() == 16)
    return 'S';
      else
    return 'I';
    }
    if(type == H5::PredType::NATIVE_UINT){
      if(type.getSize() == 16)
    return 's';
      else
    return 'I';
    }
    if(type == H5::PredType::NATIVE_LONG){
      return 'I';
    }
    if(type == H5::PredType::NATIVE_ULONG){
      return 'i';
    }
    if(type == H5::PredType::NATIVE_FLOAT){
      return 'F';
    }
    if(type == H5::PredType::NATIVE_DOUBLE){
      return 'D';
    }

    return 'U'; // unknown
}