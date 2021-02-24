# Get the GCC preinstalled image from Docker Hub
FROM ubuntu:groovy

# install c++ tools
RUN apt-get update && apt-get -y install build-essential cmake git graphviz python3 doxygen

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /home/fastPL
WORKDIR /home/fastPL/

# install & configure submodules 
RUN git submodule init && git submodule update

# build c++ programs
RUN mkdir -p /home/fastPL/build
WORKDIR /home/fastPL/build/
RUN cmake .. -DBUILD_DEPS:BOOL=ON -DUSE_SCIP=OFF && make

# To visualize profiling data
RUN git clone https://github.com/jrfonseca/gprof2dot.git

CMD ["./Main -h"]