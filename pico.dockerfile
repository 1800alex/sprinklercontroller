# Fetch ubuntu image
FROM ubuntu:22.04

# Install prerequisites
RUN \
    apt update && \
    apt install -y git python3 && \
    apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
    
# Install Pico SDK
RUN \
    mkdir -p /project && \
    cd /project/ && \
    git clone https://github.com/raspberrypi/pico-sdk.git --branch 1.3.0 && \
    cd pico-sdk/ && \
    git submodule update --init && \
    cd /
    
# Set the Pico SDK environment variable
ENV PICO_SDK_PATH=/project/pico-sdk/
ENV PICO_PLATFORM=rp2040
ENV PICO_BOARD=pico
ENV RASPBERRY_PI_PICO=1

# Copy in our source files
COPY ./ /project/src/

# Build project
RUN \
    mkdir -p /project/src/build && \
    cd /project/src/build && \
    cmake .. && \
    cmake --build .
    
# Command that will be invoked when the container starts
ENTRYPOINT ["/bin/bash"]
