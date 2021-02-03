FROM gcc:9.3

# install c++ tools
RUN apt-get update && apt-get -y install build-essential cmake git doxygen

# install google profiler: gperftools
RUN apt-get install -y gperf libgoogle-perftools-dev google-perftools
#RUN apt install -yq golang
#RUN go get -u github.com/google/pprof

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /home/fastPL
WORKDIR /home/fastPL/

# install & configure submodules 
RUN git submodule init && git submodule update

# build c++ programs
RUN mkdir -p /home/fastPL/build
WORKDIR /home/fastPL/build/
RUN cmake .. && make

# Run the program output from the previous step
CMD ["./Main -h"]