# Get the GCC preinstalled image from Docker Hub
FROM gcc:9.3

RUN apt-get update && apt-get -y install cmake
# Copy the current folder which contains C++ source code to the Docker image under /usr/src
#COPY <src> … <dest> 
COPY . /usr/src/fastpl

# Specify the working directory
WORKDIR /usr/src/fastpl/dockertest

# Use GCC to compile the Test.cpp source file
RUN g++ -o Test Test.cpp

# Run the program output from the previous step
CMD ["./Test"]
