void loadClasses()
{
    TString include = ".include ";
    TString load = ".L ";
    TString pwd = gSystem->pwd();
    pwd = pwd + '/';

    TString prefix;
    prefix = "..";

    gSystem->AddLinkedLibs("-lhdf5_cpp");

    gROOT->ProcessLine( include + "/usr/include/hdf5/serial" );
    gROOT->ProcessLine( include + pwd + prefix );
    gROOT->ProcessLine( load + prefix + "/Geometry.cc+" );
    gROOT->ProcessLine( load + prefix + "/H52Root.cc+" );
    gROOT->ProcessLine( load + prefix + "/EventBuilder.cc+" );
    gROOT->ProcessLine( load + prefix + "/SkimnLi.cc+" );
    gROOT->ProcessLine( load + prefix + "/BundlenLi.cc+" );
    gROOT->ProcessLine( load + prefix + "/IBDCut.cc+" );

}
