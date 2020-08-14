FROM docker.io/library/alpine:edge AS builder

RUN apk add curl && \
    curl https://ftp.travitia.xyz/alpine/jens@troet.org-5ea01144.rsa.pub -o /etc/apk/keys/jens@troet.org-5ea01144.rsa.pub && \
    echo "https://ftp.travitia.xyz/alpine" >> /etc/apk/repositories

RUN apk add g++ gcc make cmake git \
    openssl-dev util-linux-dev \
    zlib-dev libpq postgresql-dev \
    jsoncpp-dev fmt-dev

# Compile drogon
RUN git clone https://github.com/an-tao/drogon && \
    cd drogon && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc) && \
    make install && \
    cd ../..

# Compile cpp_redis
RUN git clone https://github.com/cpp-redis/cpp_redis && \
    cd cpp_redis && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc) && \
    make install

WORKDIR /src

COPY . .

RUN rm -rf build && mkdir build && ./build.sh

CMD build/teatro
