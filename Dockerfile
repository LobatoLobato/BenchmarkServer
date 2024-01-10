FROM ubuntu:latest as build

RUN apt-get update && apt-get upgrade -y
RUN apt-get install build-essential libtool autoconf curl zip unzip wget libssl-dev ninja-build git -y

# BUILDING CMAKE
ENV CMAKE_VERSION=3.27
ENV CMAKE_BUILD=7
ENV CMAKE_FULL_VERSION_NAME=cmake-$CMAKE_VERSION.$CMAKE_BUILD

WORKDIR /tmp
RUN wget https://cmake.org/files/v$CMAKE_VERSION/$CMAKE_FULL_VERSION_NAME.tar.gz \
    && tar -xzvf $CMAKE_FULL_VERSION_NAME.tar.gz \
    && rm -rf $CMAKE_FULL_VERSION_NAME.tar.gz

WORKDIR /tmp/$CMAKE_FULL_VERSION_NAME
RUN ./bootstrap --parallel=$(nproc) \
  && make -j$(nproc)\
  && make install\
  && rm -rf /tmp/$CMAKE_FULL_VERSION_NAME

# BUILDING BOOST
ENV BOOST_VERSION=1_84_0

WORKDIR /tmp
RUN BOOST_VERSION_DOTS=$(echo "$BOOST_VERSION" | sed 's/_/./g') && \
    wget https://archives.boost.io/release/$BOOST_VERSION_DOTS/source/boost_$BOOST_VERSION.tar.bz2 \
  && tar --bzip2 -xf /tmp/boost_$BOOST_VERSION.tar.bz2 \
  && rm -rf /tmp/boost_$BOOST_VERSION.tar.bz2

WORKDIR /tmp/boost_$BOOST_VERSION
RUN ./bootstrap.sh \
  && ./b2 install \
  && rm -rf /tmp/boost_$BOOST_VERSION

# INSTALLING VCPKG
WORKDIR /
RUN git clone https://github.com/Microsoft/vcpkg.git \
  && /vcpkg/bootstrap-vcpkg.sh

# INSTALLING TINYORM
RUN apt-get install software-properties-common -y
RUN add-apt-repository universe && apt-get update
RUN apt install qt6-base-dev libqt6sql6-mysql -y
RUN /vcpkg/vcpkg install range-v3 tabulate

WORKDIR /tmp
RUN git clone https://github.com/silverqx/TinyORM.git \
  && cmake \
    -S "/tmp/TinyORM" \
    -B "/tmp/TinyORM/build-release" \
    -G 'Ninja' \
    -D CMAKE_BUILD_TYPE:STRING='Release' \
    -D CMAKE_TOOLCHAIN_FILE:FILEPATH="/vcpkg/scripts/buildsystems/vcpkg.cmake" \
    -D CMAKE_INSTALL_PREFIX:PATH="/usr" \
    -D VERBOSE_CONFIGURE:BOOL=ON \
    -D BUILD_TESTS:BOOL=OFF \
    -D MYSQL_PING:BOOL=OFF \
    -D TOM:BOOL=ON \
    -D TOM_EXAMPLE:BOOL=OFF \
    -D TOM_MODELS_DIR:PATH="database/models" \
    -D MATCH_EQUAL_EXPORTED_BUILDTREE:BOOL=ON \
  && cmake --build /tmp/TinyORM/build-release --target install \
  && rm -rf /tmp/TinyORM

WORKDIR /server

COPY . .

RUN cmake -G "Ninja" -B build . -D CMAKE_TOOLCHAIN_FILE:FILEPATH="/vcpkg/scripts/buildsystems/vcpkg.cmake" \
  && ninja -C build

FROM ubuntu:latest
RUN apt update && apt install docker.io git -y
RUN mkdir -p /tmp/benchmark_server/cloned_repositories

COPY --from=build /server/build/BenchmarkServer /server/BenchmarkServer
COPY --from=build /server/build/database/tom /server/tom
COPY --from=build /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu
COPY --from=build /usr/local/lib/libboost* /usr/local/lib

CMD /server/tom migrate && /server/BenchmarkServer
