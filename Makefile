compile: main.c
	clang -Wall -Wfatal-errors main.c -o main

run: compile
	./main

# cible: prerequis prerequis
#	clang -o hello.c -I.