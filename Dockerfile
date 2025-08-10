FROM arm64v8/ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wget \
    cmake \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://github.com/mavlink/MAVSDK/releases/download/v3.9.0/libmavsdk-dev_3.9.0_debian12_arm64.deb && \
    apt-get install -y ./libmavsdk-dev_3.9.0_debian12_arm64.deb && \
    rm libmavsdk-dev_3.9.0_debian12_arm64.deb

WORKDIR /workspace

COPY src/main.cpp .
COPY CMakeLists.txt .

RUN mkdir build && cd build && \
    cmake .. && \
    make

ENTRYPOINT ["/workspace/build/main"]