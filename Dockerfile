FROM docker.io/library/alpine:edge AS builder

RUN apk add curl && \
    curl https://ftp.travitia.xyz/alpine/jens@troet.org-5ea01144.rsa.pub -o /etc/apk/keys/jens@troet.org-5ea01144.rsa.pub && \
    echo "https://ftp.travitia.xyz/alpine" >> /etc/apk/repositories

RUN apk add util-linux jsoncpp zlib libpq

RUN apk add --virtual .headers util-linux-dev jsoncpp-dev zlib-dev gcc g++ make cmake git postgresql-dev

# Compile drogon
RUN git clone https://github.com/an-tao/drogon && \
    cd drogon && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=release .. && \
    make -j $(nproc) && \
    make install && \
    cd ../.. && \
    rm -rf drogon

# Compile cpp_redis
RUN git clone https://github.com/cpp-redis/cpp_redis && \
    cd cpp_redis && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=release .. && \
    make -j $(nproc) && \
    make install && \
    cd ../.. && \
    rm -rf cpp_redis

# Compile fmt
RUN git clone https://github.com/fmtlib/fmt && \
    cd fmt && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_BUILD_TYPE=release . && \
    make -j $(nproc) && \
    make install && \
    cd .. && \
    rm -rf fmt

WORKDIR /src

COPY . .

RUN rm -rf build && \
    mkdir build && \
    ./build.sh && \
    mv build/teatro /usr/bin/teatro && \
    apk del .headers curl && \
    cd / && \
    rm -rf src && \
    find /usr -type f -name "*.a" -exec rm -f "{}" \; && \
    rm -rf /usr/include /usr/local

WORKDIR /teatro

CMD /usr/bin/teatro
