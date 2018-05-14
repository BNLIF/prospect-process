void bundle(const char* inputname, const char* outname)
{
    // gROOT->ProcessLine(".x loadClasses.C" );
    EventBuilder eb(inputname, outname);
    eb.Scan();
    // eb.Scan(10000);
}