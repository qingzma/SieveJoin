## Quasi Join Using Bloom Filter

# run experiments

## TPC-H

## Synthetic RTS

### Step 1: create tables

```
cd data/synthetic/
mkdir build
cd build
cmake ..
cmake --build .
```

then you get the executable `rst`, which is used to create RST tables.\
copy executable `rst` to `data/synthetic/data/`, and run
`./generate.sh`
to create all required tables.

### Step 2: run queries

`./qentry --qrst -p ../data/synthetic/data`
