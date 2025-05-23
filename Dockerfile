# Use Ubuntu as base
FROM ubuntu:22.04

# Install build tools
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    g++ cmake make curl wget git && \
    apt-get clean

RUN apt-get install -y build-essential make cmake \
	qtbase5-dev qt5-qmake git libssl-dev openssl

# Check OpenSSL version
RUN openssl version


# Set working dir
WORKDIR /app

# Copy source code
COPY . .

# Build your app
RUN mkdir build && cd build && cmake .. && make


# Expose the port your app listens on
EXPOSE 8080
#EXPOSE 8193

# Run the server
CMD ["./build/CoreHttpServer"]

