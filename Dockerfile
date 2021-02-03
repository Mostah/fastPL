# Get the GCC preinstalled image from Docker Hub
FROM gcc:9.3

# install cmake
RUN apt-get update && apt-get -y install cmake git doxygen
RUN git clone --branch thomas/gperftools https://github.com/Mostah/fastPL.git

# install & configure submodules 
RUN cd fastPL && git submodule init && git submodule update

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
#COPY <src> … <dest> 
COPY . /home/fastPL
RUN rm -rf /home/fastPL/build && mkdir /home/fastPL/build
WORKDIR /home/fastPL/build/
RUN cmake .. && make

RUN ./Test

# Run the program output from the previous step
CMD ["./Main -h"]