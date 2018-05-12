void bundle(const char* inputname, const char* outname)
{
    // gROOT->ProcessLine(".x loadClasses.C" );
    EventBuilder eb(inputname, outname);
    eb.Scan(1.4e6);
    // eb.Scan(100);
}