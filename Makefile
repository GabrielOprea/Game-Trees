FLAGS=-Wall -Wextra
EXE=minimax

build: $(EXE)

run: build
	./$(EXE)

$(EXE): Trees.o TreesC2.o TreesAlphaBeta.o Paranoic.o
	 gcc Trees.o TreesC2.o TreesAlphaBeta.o Paranoic.o -o $(EXE)
Trees.o: Trees.c
	gcc -c $(FLAGS) Trees.c
TreeC2.o: TreesC2.c
	gcc -c $(FLAGS) TreesC2.c
TreesAlphaBeta.o: TreesAlphaBeta.c
	gcc -c $(FLAGS) TreesAlphaBeta.c
Paranoic.o: Paranoic.c
	gcc -c $(FLAGS) Paranoic.c
clean:
	rm *.o $(EXE)
