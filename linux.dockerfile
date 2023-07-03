# Fetch ubuntu image
FROM ubuntu:22.04

# Install prerequisites
RUN \
    apt update && \
    apt install -y cmake build-essential libncurses-dev libgtest-dev
    
RUN mkdir -p /project    
ENV LINUX=1

# Copy in our source files
COPY ./ /project/src/

# Build project
RUN \
    mkdir -p /project/src/build && \
    cd /project/src/build && \
    cmake .. && \
    cmake --build . && \
    ctest && find /project/src/build
    
# Command that will be invoked when the container starts
ENTRYPOINT ["/bin/bash"]
