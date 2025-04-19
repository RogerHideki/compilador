- trabalho-1
   - flex -o build/lex.yy.c src/lex.l
   - gcc -o build/lex.exe build/lex.yy.c
   - build/lex.exe

   - g++ -o build/compiler-core.exe src/compiler-core.cpp
   - build/compiler-core.exe
   
- trabalho-2
   - g++ -o build/object-code-executor.exe src/object-code-executor.cpp
   - build/object-code-executor.exe