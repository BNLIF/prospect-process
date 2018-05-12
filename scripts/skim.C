void skim(const char* inputname, const char* outname, TString option="nLi")
{
    // gROOT->ProcessLine(".x loadClasses.C" );
    if ( option=="nLi" ) {
        SkimnLi s(inputname, outname);
        s.Scan();
    }
}