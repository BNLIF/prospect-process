void showV(TTree* t, Long64_t entry=-1, Int_t lenmax=20)
{
    // helper method to print all active leaves from one UserData entry
    //
    // if entry==-1, print current entry (default)
    // if a leaf is an array/vector, a maximum of lenmax elements is printed.
    //
    if (entry != -1) {
        Int_t ret = t->LoadTree(entry);
        if (ret == -2) {
            Error("Show()", "Cannot read entry %lld (entry does not exist)", entry);
            return;
        } else if (ret == -1) {
            Error("Show()", "Cannot read entry %lld (I/O error)", entry);
            return;
        }
        ret = t->GetEntry(entry);
        if (ret == -1) {
            Error("Show()", "Cannot read entry %lld (I/O error)", entry);
            return;
        } else if (ret == 0) {
            Error("Show()", "Cannot read entry %lld (no data read)", entry);
            return;
        }
    }
    printf("======> EVENT:%lld\n", entry);
    TObjArray* leaves  = t->GetListOfLeaves();
    Int_t nleaves = leaves->GetEntriesFast();
    Int_t ltype;
    for (Int_t i = 0; i < nleaves; i++) {
        TLeaf* leaf = (TLeaf*) leaves->UncheckedAt(i);
        TBranch* branch = leaf->GetBranch();
        // if (branch->TestBit(kDoNotProcess)) {
        //     continue;
        // }
        Int_t len = leaf->GetLen();
        if (len <= 0) {
            continue;
        }
        len = TMath::Min(len, lenmax);
        if (leaf->IsA() == TLeafElement::Class()) {
            string leafTypeName = leaf->GetTypeName();
            // cout << "leafTypeName: " << leafTypeName << endl;
            if (leafTypeName == "vector<float>") {
                printf(" %-15s = ", leaf->GetName());
                vector<float> *vf = (vector<float>*)((TBranchElement*)branch)->GetObject();
                Int_t lenV = TMath::Min((Int_t)vf->size(), lenmax);
                for (int ii=0; ii<lenV; ii++) {
                    cout << (*vf)[ii] << ", ";
                }
                cout << "\n";
            }
            else if (leafTypeName == "vector<int>") {
                printf(" %-15s = ", leaf->GetName());
                vector<int> *vi = (vector<int>*)((TBranchElement*)branch)->GetObject();
                Int_t lenV = TMath::Min((Int_t)vi->size(), lenmax);
                for (int ii=0; ii<lenV; ii++) {
                    cout << (*vi)[ii] << ", ";
                }
                cout << "\n";
            }
            else if (leafTypeName == "vector<double>") {
                printf(" %-15s = ", leaf->GetName());
                vector<double> *vi = (vector<double>*)((TBranchElement*)branch)->GetObject();
                Int_t lenV = TMath::Min((Int_t)vi->size(), lenmax);
                for (int ii=0; ii<lenV; ii++) {
                    cout << (*vi)[ii]*1e-6 << ", ";
                }
                cout << "\n";
            }
            else {
                leaf->PrintValue(lenmax);
            }
            continue;
        }
        if (branch->GetListOfBranches()->GetEntriesFast() > 0) {
            continue;
        }
        ltype = 10;
        if (leaf->IsA() == TLeafF::Class()) {
            ltype = 5;
        }
        if (leaf->IsA() == TLeafD::Class()) {
            ltype = 5;
        }
        if (leaf->IsA() == TLeafC::Class()) {
            len = 1;
            ltype = 5;
        };
        printf(" %-15s = ", leaf->GetName());
        for (Int_t l = 0; l < len; l++) {
            leaf->PrintValue(l);
            if (l == (len - 1)) {
                printf("\n");
                continue;
            }
            printf(", ");
            if ((l % ltype) == 0) {
                printf("\n                  ");
            }
        }
    }

}