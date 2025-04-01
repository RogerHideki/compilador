- trabalho-1
   - flex ./lex.l
   - gcc ./lex.yy.c -o ./lex.exe
   - cat ./lex-input.txt | ./lex.exe > ./lex-output.txt

   - g++ ./syntactic-analyzer.cpp -o ./syntactic-analyzer.exe
   - cat ./lex-output.txt | ./syntactic-analyzer.exe > ./syntactic-analyzer-output.txt
   
- trabalho-2
   - g++ ./trabalho-2.cpp -o ./trabalho-2.exe
   - cat ./trabalho-2-input.txt | ./trabalho-2.exe > ./trabalho-2-output.txt