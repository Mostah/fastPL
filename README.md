# Fast Preference Learning algorithm with parallelisation
This work is based on Olivier Sobrie's work on preference learning algorithms (python code : <https://github.com/oso/pymcda>, thesis: <https://tel.archives-ouvertes.fr/tel-01370555/document>).

The  objective of this repository is to translate the previous code (py-mcda) in C++ and set up the parallelisation tools needed to process a greater amount of entrypoints.

# Repository structure

* [src](https://github.com/Mostah/fastPL/tree/master/src)
Sources files (headers and classes)

* [test](https://github.com/Mostah/fastPL/tree/master/test)
Test files

* [extsrc](https://github.com/Mostah/fastPL/tree/master/extsrc)
External sources, here googletest

* [.circleci](https://github.com/Mostah/fastPL/tree/master/.circleci)
CircleCi configuration

# Tests environment configuration

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

### Run test manually

First, move to build folder
```
cd build
```

#### Run all tests
```
./Test  
```

#### Run specific tests

Run the exectuable using the filter option of gtest:
```
--gtest_filter=POSTIVE_PATTERNS[-NEGATIVE_PATTERNS]
    Run only the tests whose name matches one of the positive patterns but
    none of the negative patterns. '?' matches any single character; '*'
    matches any substring; ':' separates two patterns.

    The name of TEST(Name1, Name2) is "Name1.Name2"
```

Example:
```
./Test --gtest_filter=TestGeneralName.TestPreciseName   # One specific Test
./Test --gtest_filter=TestGeneralName.*                 # All tests of Name1 = GeneralName 
```

## Docker image

### Run test using the docker image
```
docker-compose up --build
```

SSH connect to the docker image (just in case)

### Replace container_id with the current container id
```
docker run -it <container_id> /bin/bash 
```
