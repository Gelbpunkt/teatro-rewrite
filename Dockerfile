FROM docker.io/library/alpine:edge AS builder

RUN apk add g++ gcc make cmake git openssl-dev jsoncpp-dev util-linux-dev zlib-dev

# Compile drogon
RUN git clone https://github.com/an-tao/drogon && \
    cd drogon && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc) && \
    make install

WORKDIR /src

COPY . .

RUN rm -rf build && mkdir build && ./build.sh

FROM alpine:edge

COPY --from=builder /src/build/teatro /usr/bin/teatro

CMD teatro
