## A few scripts to reprocess PROSPECT data

### Compile

```
cd scripts/
root -b -q loadClasses.C
```

### Re-bundle Events

```
cd scripts/
root -b -q loadClasses.C 'bundle.C("input.root", "output.root")'
```

### Skim nLi Coincidence Events

```
cd scripts/
root -b -q loadClasses.C 'skim.C("input.root", "output.root")'
```
