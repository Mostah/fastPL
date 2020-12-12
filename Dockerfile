# Get the GCC preinstalled image from Docker Hub
FROM gcc:9.3

# install cmake
RUN apt-get update && apt-get -y install cmake git

RUN git clone https://github.com/Mostah/fastPL.git
# install & configure googletest 
RUN cd fastPL && git submodule init && git submodule update
RUN git clone https://github.com/google/googletest.git -b release-1.10.0 
RUN cd googletest && mkdir build && cd build && cmake .. && make && make install


# Copy the current folder which contains C++ source code to the Docker image under /usr/src
#COPY <src> … <dest> 
#COPY . /usr/src/fastpl
WORKDIR /fastPL
RUN cmake CMakeLists.txt && make
# Run the program output from the previous step
CMD ["./Test"]
