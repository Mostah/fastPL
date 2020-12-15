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

### Run test using the docker image
```
docker-compose up --build
```

### ssh connect to the docker image (just in case)
# replace container_id with the current container id
```
docker run -it <container_id> /bin/bash 
```
