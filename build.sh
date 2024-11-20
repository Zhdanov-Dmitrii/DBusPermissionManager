mkdir -p build
conan install . --build=missing
cmake --preset=conan-release
cmake --build build/Release/
