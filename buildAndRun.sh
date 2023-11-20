export CXX=/usr/bin/clang++
cd build &&  rm ./twoWindows; DCMAKE_BUILD_TYPE=Debug cmake ..&& cmake --build .&& cd ../ && ./build/twoWindows "$@"
