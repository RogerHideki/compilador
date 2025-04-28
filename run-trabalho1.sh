set -e

mkdir -p build

flex -o build/lex.yy.c src/lex.l
gcc -o build/lex.exe build/lex.yy.c
./build/lex.exe

g++ -o build/compiler-core.exe src/compiler-core.cpp
./build/compiler-core.exe