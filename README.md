# Fast Preference Learning algorithm with parallelisation
This work is based on Olivier Sobrie's work on preference learning algorithms (python code : <https://github.com/oso/pymcda>, thesis: <https://tel.archives-ouvertes.fr/tel-01370555/document>).

The  objective of this repository is to translate the previous code (py-mcda) in C++ and set up the parallelisation tools needed to process a greater amount of entrypoints.

# Repository structure  

* [src](https://github.com/Mostah/fastPL/tree/master/src) : Sources files (headers and classes)

* [test](https://github.com/Mostah/fastPL/tree/master/test) : Test files

* [extsrc](https://github.com/Mostah/fastPL/tree/master/extsrc) : External sources and dependencies

* [.circleci](https://github.com/Mostah/fastPL/tree/master/.circleci) : CircleCi configuration

# Run app and test in Docker

## Build Docker image

First thing first, build the docker image

```
docker build . -t fastpl
```

## Run test using the docker image
```
docker run fastpl ./Test
```

## Run the main app using the docker image
The following command will show the run config options (helper):
```
docker run fastpl
```

To run the app with default config:
```
docker run fastpl ./Main
```

## SSH connect to the docker image and check the logs

Replace container_id with the current container id
```
docker run -it <container_id> /bin/bash 
cd /home/fastpl/logs
```

# Online Documentation

## Link to the online documentation
https://mostah.github.io/fastPL/

## Script to update online doc
From root directory:

```
sh doc_generation.sh
```

# Local Tests environment configuration

## Test C++ code on Circle CI

See [Circle CI configuration file](https://github.com/Mostah/fastPL/blob/master/.circleci/config.yml)

## Manual test with google-test

### Download and set up dependencies

``` 
git submodule init
git submodule update
``` 

### Build the project

cmake must have been installed in the machine. Run `sudo apt-get install cmake` for Ubuntu

```
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

## Generating documentation locally with doxygen

### Installing doxygen

#### On Ubuntu
```
sudo apt-get install doxygen
```

#### On MacOSX with brew
```
brew install doxygen
```

### Generating documentation

```
cd doc
cmake .
doxygen Doxyfile.Doxigen
cd html
google-chrome index.html
```

## Profiling

### Profiling with GPROF

Running the docker container and ssh into it:
```
docker run -it fastpl /bin/bash
```

Now we are inside the container:


Executing the program we want to profile
```
./{$PROGRAM}
```

Creating the profiling data
```
gprof {$PROGRAM} gmon.out > analysis.txt
```

Keep this terminal open, and open a new one:

copy the analysis from the docker container into your machine:

Get the {$ID) of the fastpl container running with `docker ps`
copy the file from the docker container into your machine:
```
docker cp {$ID}:/home/fastPL/build/analysis.txt {$YOUR_PATH_TO_STORE_IT}
```

The profiling data should now be in {$YOUR_PATH_TO_STORE_IT} in your machine.
