
# Stage 1: Build Stage
FROM ubuntu:22.04 AS build

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package lists and install essential build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Install Conan using pip
RUN pip3 install --upgrade pip && pip3 install conan

# Set the working directory to /usr/src/app
WORKDIR /usr/src/app

# Copy the entire project into the container
COPY . /usr/src/app

# Create build directory and install dependencies with Conan
RUN mkdir build && cd build && \
    conan profile detect --force && \
    conan install .. --build=missing

# Compile the project using cmake and make
WORKDIR /usr/src/app/build
RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j2

# Set the working directory back to /usr/src/app for convenience
WORKDIR /usr/src/app

# Set the default command to run the compiled application
CMD ["./process_manager"]
