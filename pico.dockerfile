# Fetch ubuntu image
FROM ubuntu:22.04

# Install prerequisites
RUN \
    apt update && \
    apt install -y git python3 && \
    apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

# RUN mkdir /builds && \
#     apt-get update && \
#     apt-get install -y build-essential cmake zip curl wget git doxygen graphviz python3 python3-pip && \
#     apt-get autoclean -y && \
#     apt-get autoremove -y && \
#     apt-get clean

# RUN wget -O archive.tar.bz2 "https://developer.arm.com/-/media/Files/downloads/gnu/12.2.mpacbti-rel1/binrel/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi.tar.xz?rev=71e595a1f2b6457bab9242bc4a40db90&hash=37B0C59767BAE297AEB8967E7C54705BAE9A4B95" && \
#     tar xf archive.tar.bz2 -C /opt && \
#     rm archive.tar.bz2
# ENV PATH=/opt/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi/bin:$PATH
    
# Install Pico SDK
RUN \
    mkdir -p /project && \
    cd /project/ && \
    git clone https://github.com/raspberrypi/pico-sdk.git --branch 1.5.1 && \
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
    cmake ../example && \
    cmake --build .
    
# Command that will be invoked when the container starts
ENTRYPOINT ["/bin/bash"]
