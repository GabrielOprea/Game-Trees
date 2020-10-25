#include "Trees.h"


/*Functie ce aloca memorie necesara pentru arborele de joc */
TTree alocTree(char** game)
{
	TTree new = calloc(1, sizeof(TNode));
	if(!new) return NULL;

	new->game = game;
	new->firstSon = NULL;
	new->next = NULL;
	return new;
}

/*Functii ce aloca memorie pentru matricea corespunzatoare jocului Connect-4 */
char** alocGame(int N, int M)
{
	/*Aloc memorie pentru vectorul de pointeri */
	char** table = calloc(N, sizeof(char*));
	if(!table) return NULL;
	int i;
	for(i = 0; i < N; i++)
	{
		/*Aloc fiecare pointer */
		table[i] = calloc(M, sizeof(char));
		if(!table[i]) return NULL;
	}
	return table;
}

/*Functie ce elibereaza memorie asociata unei matrice de joc */
void freeGame(char*** gameAdr , int N)
{
	/*Elberez fiecare vector */
	char** game = *gameAdr;
	int i;
	for(i = 0; i < N; i++)
		free(game[i]);
	/*Eliberez pointerul initial */
	free(game);
	/*Setez adresa eliberata la NULL */
	*gameAdr = NULL;
}


/*Functie ce va schimba jucatorul care urmeaza sa mute */
char changeTurn(char turn)
{
	if(turn == 'R')
		return 'B';
	return 'R';
}

/*Functie ce primeste o matrice de joc si verifica daca jocul s-a terminat */
int isComplete(char** game, int N, int M)
{

	int full = 1;
	/*Verific daca e plina tabla de joc */
	int i, j, k;
	for(i = 0; i < N; i++)
		for(j = 0; j < M; j++)
			if(game[i][j] == '-')
				full = 0;
	if(full) return 1;


	int nrPieces;
	int turn = 'R';

	/*Verific daca sunt 4 pe aceeasi linie */
	for(i = 0; i < N; i++)
	{
		/*Retin in nrPieces numarul de piese consecutive */
		nrPieces = 0;
		for(j = 0; j < M; j++)
		{
			if(game[i][j] == turn)
				nrPieces++;
			else if(game[i][j] == changeTurn(turn))
			{
				/* Daca e randul celuilalt jucator, setez numarul
				de piese curente la 1(piesa acestui jucator) */
				nrPieces = 1;
				turn = changeTurn(turn);
			}
			else
				nrPieces = 0; /* daca pozitia este goala, restez nr de piese */

			/*Daca am intalnit 4 piese consecutive ma opresc */
			if(nrPieces == 4)
				return 1;
		}
	}

	/*Verific daca sunt 4 pe aceeasi coloana */
	for(j = 0; j < M; j++)
	{
		nrPieces = 0;
		for(i = 0; i < N; i++)
		{
			if(game[i][j] == turn)
				nrPieces++;
			else if(game[i][j] == changeTurn(turn))
			{
				nrPieces = 1;
				turn = changeTurn(turn);
			}
			else
				nrPieces = 0;

			if(nrPieces == 4)
				return 1;
		}
	}

	/*Verific daca sunt 4 pe diagonala principala + cele de sus */
	/*k imi va da numarul de diagonale pe care se afla minim 4 elemente */
	for(k = 0; k <= M - 4; k++)
	{
		nrPieces = 0;

		for(i = 0; i < N; i++)
		{

			if(i + k >= M) break;
			
			if(game[i][i + k] == turn)
				nrPieces++;
			else if(game[i][i + k] == changeTurn(turn))
			{
				nrPieces = 1;
				turn = changeTurn(turn);
			}
			else
				nrPieces = 0;

			if(nrPieces == 4)
				return 1;
		}
	}

	/*Verific daca sunt 4 sub diagonala principala*/
	for(k = 1; k <= N - 4; k++)
	{
		nrPieces = 0;

		for(i = 0; i < M; i++)
		{

			if(i + k >= N) break;
			
			if(game[i + k][i] == turn)
				nrPieces++;
			else if(game[i + k][i] == changeTurn(turn))
			{
				nrPieces = 1;
				turn = changeTurn(turn);
			}
			else
				nrPieces = 0;

			if(nrPieces == 4)
				return 1;
		}

	}
	
	/*Verific daca sunt 4 pe diagonala secundara + cele de deasupra */
	for(k = 0; k <= M - 4; k++)
	{
		nrPieces = 0;
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < M; j++)
			{
				if(i + j == M - 1 - k)
				{
					if(game[i][j] == turn)
						nrPieces++;
					else if(game[i][j] == changeTurn(turn))
					{
						nrPieces = 1;
						turn = changeTurn(turn);
					}
					else
						nrPieces = 0;

					if(nrPieces == 4)
						return 1;
				}
			}
		}
	}

	/*Verific daca sunt 4 sub diagonala secundara */
	for(k = 0; k <= N - 4; k++)
	{
		nrPieces = 0;
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < M; j++)
			{
				if(i + j == M - 1 + k)
				{
					if(game[i][j] == turn)
						nrPieces++;
					else if(game[i][j] == changeTurn(turn))
					{
						nrPieces = 1;
						turn = changeTurn(turn);
					}
					else
						nrPieces = 0;

					if(nrPieces == 4)
						return 1;
				}
			}
		}
	}

	return 0;
}

/*Functie ce returneaza prima pozitie libera de pe coloana col */
int getFreePoz(char** game, int N, int col)
{
	int i;
	for(i = N - 1; i >= 0; i--)
		if(game[i][col] == '-')
			return i;

	return -1;
}

/* Functie ce aloca memorie pentru un nou joc, cu aceeasi informatie ca jocul
primit ca parametru */
char** copyGame(char** game, int N, int M)
{
	int i;
	char** newGame = alocGame(N, M);
	if(!newGame) return NULL;
	for(i = 0; i < N; i++)
		memcpy(newGame[i], game[i], M);
	return newGame;
}

/*Functie ce construieste arborele de joc */
void buildTree(TTree root, int N, int M, char turn)
{
	/*Verific ca arborele curent sa nu aiba un joc completat */
	if(isComplete(root->game, N, M))
		return;

	int first = 1; /*Tratez separat primul copil al nodului curent */
	TTree currentNode = NULL;

	int i;
	for(i = 0; i < M; i++)
	{
		/*Pentru fiecare linie din matrice, obtin linia unde
		trebuie sa inserez piesa */
		int poz = getFreePoz(root->game, N, i);

		if(poz != -1)
		{
			if(first)
			{
				/*Daca este la primul copil, inserez noua matrice in 
				firstSon */
				first = 0;
				char** newGame = copyGame(root->game, N, M);
				newGame[poz][i] = turn;
				root->firstSon = alocTree(newGame);
				currentNode = root->firstSon; 
			}
			else
			{
				char** newGame = copyGame(root->game, N, M);
				newGame[poz][i] = turn;
				currentNode->next = alocTree(newGame);
				currentNode = currentNode->next;
			}
			/*Apelez recursiv functia pentru nodul curent modificat
			(pentru fiecare copil al nodului root) */
			buildTree(currentNode, N, M, changeTurn(turn));
		}
	}
}

/*Functie ce afiseaza un joc in fisierul output cu un numar de tab-uri */
void printGame(char** game, int N, int M, int nrTabs, FILE* output)
{
	int i, j, k;
	for(i = 0; i < N; i++)
	{
		/*Printez fiecare tab */
		for(k = 0; k < nrTabs; k++)
			fprintf(output,"\t");

		for(j = 0; j < M; j++)
		{
			/*Printez fiecare element in parte */
			fprintf(output, "%c", game[i][j]);
			if(j != M - 1)
				fprintf(output," ");
		}
		fprintf(output, "\n");
	}

	fprintf(output, "\n");
}

/*Functie ce citeste un joc dintr-un fisier */
void readGame(char** game, int N, int M, FILE* input)
{
	int i, j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			fscanf(input, "%c", &game[i][j]);
			fscanf(input, " ");
		}
	}
}

/*Functie ce afiseaza un arbore de joc multicai */
void printMultiTree(TTree root, int N, int M, int level, FILE* output)
{
	if(root == NULL)
		return; /*conditia de iesire din recursivitate */
	
	TTree currentNode = root;
	while(currentNode != NULL)
	{
		/*Pentru fiecare copil afisez matricea asociata,apoi apelez recursiv */
		printGame(currentNode->game, N, M, level, output);
		printMultiTree(currentNode->firstSon, N, M, level + 1, output);
		currentNode = currentNode->next;
	}
}

/*Functie ce elibereaza memoria pentru un arbore de joc multicai */
void freeMultiTree(TTree* rootAdr, int N, int M)
{

	TTree root = *rootAdr;

	if(root == NULL) /*conditia de iesire din recursivitate */
		return;

	TTree currentNode = root;
	while(currentNode != NULL)
	{
		/*Pentru ficare nod de pe nivelul curent, eliberez memoria pentru copii */
		freeMultiTree(&currentNode->firstSon, N, M);
		TTree aux = currentNode;
		currentNode = currentNode->next;
		if(aux->game != NULL)
			freeGame(&aux->game, N);
		aux->firstSon = NULL;
		aux->next = NULL;
		free(aux);
		/*Setez nodul eliberat la NULL */
		aux = NULL;
	}	
}


int main(int argc, char* argv[])
{
	/*Task-ul rezolvat va fi primul argument din linia de comanda */
	char* mode = argv[1];
	if(argc != 4)
	{
		printf("Not enough arguments\n");
		return -1;
	}

	/*Deschid fisierele primite ca parametri */
	FILE* input = fopen(argv[2], "r");
	if(!input) return -1;
	FILE* output = fopen(argv[3], "w");
	if(!output) return -1;

	int i;
	/*Tratez fiecare task separat */
	if(!strcmp(mode, "-c1"))
	{
		int N, M;
		char turn;
		/*Citesc din fisier dimensiunile matricei si jucatorul curent */
		fscanf(input, "%d", &N);
		fscanf(input, "%d", &M);
		fscanf(input, " ");
		fscanf(input, "%c", &turn);
		fscanf(input, "\n");

		char** game;

		/*Aloc si citesc matricea */
		game = alocGame(N, M);
		readGame(game, N, M, input);

		/*Aloc radacina, construiesc arborele multicai de joc */
		TTree root = alocTree(game);
		buildTree(root, N, M, turn);

		/*Afisez arborele construit in fisier */
		printMultiTree(root, N, M, 0, output);

		/*Distrug arborele */
		freeMultiTree(&root, N, M);
		
		fclose(output);
		fclose(input);
	}
	else
	if(!strcmp(mode, "-c2"))
	{

		int N;
		/*Citesc inaltimea arborelui */
		fscanf(input, "%d", &N);
		fscanf(input, "\n");
		char** lines = calloc(N, sizeof(char*));
		/*Obtin dimensiunea maxima a unei linii */
		int maxLineLen = getFileSize(argv[2]);
		for(i = 0; i < N; i++)
		{
			/*Citesc fiecare linie in parte */
			lines[i] = calloc(maxLineLen, sizeof(char));
			fgets(lines[i], maxLineLen, input);
			int len = strlen(lines[i]);
			if(lines[i][len - 1] == '\n')
				lines[i][len - 1] = '\0';
		}

		/*Parsez liniile, si aloc memorie pentru radacina */
		TTree2 root = alocTreeC2(readString(lines[0]));

		/*Construiesc arborele initial */
		buildTreeC2(root, lines, root->num, 1);
		
		/*Construiesc arborele MinMax */
		MinMaxTree(root, 0);

		/*Afisez arborele construit in fisier */
		printMinMaxTree(root, 0, output);

		/*Eliberez memoria pentru arborele MinMax */
		FreeTreeC2(&root);

		/*Iar apoi eliberez memoria pentru linii */
		for(i = 0; i < N; i++)
			free(lines[i]);
		free(lines);

		fclose(output);
		fclose(input);
	}
	else if(!strcmp(mode, "-c3"))
	{
		int N;
		fscanf(input, "%d", &N);
		fscanf(input, "\n");
		char** lines = calloc(N, sizeof(char*));
		int maxLineLen = getFileSize(argv[2]);
		for(i = 0; i < N; i++)
		{
			lines[i] = calloc(maxLineLen, sizeof(char));
			fgets(lines[i], maxLineLen, input);
			int len = strlen(lines[i]);
			if(lines[i][len - 1] == '\n')
				lines[i][len - 1] = '\0';
		}


		TTree2 root = alocTreeC2(readString(lines[0]));
		buildTreeC2(root, lines, root->num, 1);
		MinMaxTree(root, 0);
		
		/*Setez alpha si beta la valorile initiale, -inf respectiv +inf */
		int alpha = INT_MIN;
		int beta = INT_MAX;

		/*Aplic algoritmul Alpha-Beta Prunning */
		AlphaBetaTree(root, 1, alpha, beta);

		/*Afisez noul arbore MinMax rezultat */
		printMinMaxTree(root, 0, output);

		/*Eliberez arborele ramas */
		FreeTreeC2(&root);

		for(i = 0; i < N; i++)
			free(lines[i]);
		free(lines);

		fclose(input);
		fclose(output);
	}
	else if(!strcmp(mode, "-b"))
	{
		int N, nrJucatori;
		/*Pentru bonus voi citi inaltimea arborelui si nr de jucatori */
		fscanf(input, "%d", &N);
		fscanf(input, "%d", &nrJucatori);
		fscanf(input, "\n");
		char** lines = calloc(N, sizeof(char*));
		int maxLineLen = getFileSize(argv[2]);
		for(i = 0; i < N; i++)
		{
			lines[i] = calloc(maxLineLen, sizeof(char));
			fgets(lines[i], maxLineLen, input);
			int len = strlen(lines[i]);
			if(lines[i][len - 1] == '\n')
				lines[i][len - 1] = '\0';
		}

		/*Parsez liniile ca mai devreme si aloc memorie pentru radacina */
		TTreeB root = alocTreeB(readString(lines[0]), nrJucatori);

		/*Construiesc arborele initial */
		buildTreeB(root, lines, root->num[0], 1, nrJucatori);
		
		/*Construiesc arborele Paranoic */
		ParanoicTree(root, 0, nrJucatori - 1, nrJucatori);
		
		/*Afisez arborele si eliberez memoria */
		printParanoicTree(root, 0, nrJucatori, output);
		FreeTreeB(&root);

		for(i = 0; i < N; i++)
			free(lines[i]);
		free(lines);

		fclose(input);
		fclose(output);
	}

	return 0;
}