# cd taglib-1.13; mkdir build; cd build
# cmake .. -DCMAKE_BUILD_TYPE=Release \
#   -DBUILD_TESTING=OFF \
#   -DBUILD_FRAMEWORK=ON \
#   -DBUILD_SHARED_LIBS=ON \
#   -DCMAKE_OSX_DEPLOYMENT_TARGET=10.10 \
#   -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
# make

# -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
# -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"

cd taglib-1.13

cmake \
    -DCMAKE_INSTALL_PREFIX="`pwd`/_install" \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF .

make

make install