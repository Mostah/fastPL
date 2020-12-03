# Get the GCC preinstalled image from Docker Hub
FROM gcc:0.3

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/dockertest1

# Specify the working directory
WORKDIR /usr/src/dockertest1

# Use GCC to compile the Test.cpp source file
RUN g++ -o Test Test.cpp

# Run the program output from the previous step
CMD ["./Test"]