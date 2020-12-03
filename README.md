# fastPL
Implementation in C++ of PYMCDA with parallelization set-up


## Test C++ code on Circle CI

see .circle/config.yml 

## Manual test with google-test

### Get gtest from repo

``` 
git submodule init
git submodule update
``` 

### Build gtest

#### Ubuntu

```
sudo apt-get install cmake
mkdir build
cd build
cmake ..
make
```

### Run test

```
cd build
./Test
```
