# Fast Preference Learning algorithm with parallelisation
This work is based on Olivier Sobrie's work on preference learning algorithms (python code : <https://github.com/oso/pymcda>, thesis: <https://tel.archives-ouvertes.fr/tel-01370555/document>).

The  objective of this repository is to translate the previous code (py-mcda) in C++ and set up the parallelisation tools needed to process a greater amount of entrypoints.

# Repository structure

* src

* test

* extsrc

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

### Run test

```
cd build
./Test
```

### Run test using the docker image
```
docker-compose up --build
```

### ssh connect to the docker image (just in case)
# replace container_id with the current container id
```
docker run -it <container_id> /bin/bash 
```
