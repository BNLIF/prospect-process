void nLi(const char* inputname, const char* outname)
{
    // gROOT->ProcessLine(".x loadClasses.C" );
    SkimnLi s(inputname, outname);
    s.Scan();
}