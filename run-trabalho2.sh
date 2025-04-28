set -e

mkdir -p build

g++ -o build/object-code-executor.exe src/object-code-executor.cpp
./build/object-code-executor.exe