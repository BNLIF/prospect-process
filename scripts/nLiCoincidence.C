void nLiCoincidence(const char* inputname, const char* outname)
{
    // gROOT->ProcessLine(".x loadClasses.C" );
    BundlenLi s(inputname, outname);
    s.Scan();
}