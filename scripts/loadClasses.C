void loadClasses()
{
    TString include = ".include ";
    TString load = ".L ";
    TString pwd = gSystem->pwd();
    pwd = pwd + '/';

    TString prefix;
    prefix = "..";
    gROOT->ProcessLine( include + pwd + prefix );
    gROOT->ProcessLine( load + prefix + "/Geometry.cc+" );
    gROOT->ProcessLine( load + prefix + "/EventBuilder.cc+" );
    gROOT->ProcessLine( load + prefix + "/SkimnLi.cc+" );

}
