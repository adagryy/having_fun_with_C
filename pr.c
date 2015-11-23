/*
5. Program sortujący stałe znakowe (wyrazy) w kolejności rosnącej przy pomocy sortowania Shella. 
Program ma zawierać funkcję sortującą, która będzie wywoływana w funkcji main. Dane do 
posortowania mają być wczytywane z pliku dyskowego. Wynikiem pracy programu ma być drugi
 plik, do którego zapisane zostaną posortowane dane. Program ma prawidłowo obsługiwać operacje 
 na plikach oraz ewentualne błędy (np. brak pliku wejściowego, niemożliwość utworzenia/zapisania 
 pliku wyjściowego). Nazwy plików wejściowego i wyjściowego mają być przekazane do programu jako 
 argumenty wywołania programu. Przykładowe wywołanie programu: ./sortowanie dane_do_sortowania.txt 
 dane_posortowane.txt
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OUT 0
#define IN 1
#define MIN(a,b) (((a)<(b))?(a):(b))

int compareString(char *s1, char *s2);
char **readData(FILE *ptr, int *i, int *size);
void shell_sort (char **a, int n);
void printTable(char ** tab, int n);
void saveToFile(FILE *f, char ** tab, int n);
void freeTab(char **tab, int n);

int main(int argc, char *argv[]){
	int i = 0, size = 16;//assumption, that input file contains less
						 //than 16 words, if not, the 'tab' array is reallocated 
						 //to larger size

	char **tab;   

	FILE *fp = fopen (argv[1], "r");
	if(fp == NULL) //checks if an opening file for reading was successful
	{
	   perror("Error in opening file");
	   return(-1);
	}

	tab = readData(fp, &i, &size);//reads data from *.txt file given as input and returns 
								  //a pointer to array including these data
	shell_sort(tab, i);	//sorts 'tab' using Shell sort sorting algorithm.
	
	//printTable(tab, i);	//you can print data to screen	

	FILE *f = fopen(argv[2], "w");
	if(f == NULL) //checks if an opening new file to saving was successful
	{
	   perror("Error in saving contents to file");
	   return(-1);
	}

	saveToFile(f, tab, i);

	freeTab(tab, i);  //it highy recommended to free dynamically allocated memory 
				//before ending the program

	fclose(fp);//we have to close 'reading' and 'saving' streams.
	fclose(f);

	return 0;
}

void freeTab(char **tab, int n){
	int i;
	for(i=0; i < n;i++){
		free(tab[i]);
	}
	free(tab);
}

void shell_sort (char **a, int n) {
    int h, i, j;
    char *t;
    for (h = n; h /= 2;) {
        for (i = h; i < n; i++) {
            t = a[i];
            for (j = i; j >= h && compareString(t, a[j - h]); j -= h) {
                a[j] = a[j - h];
            }
            a[j] = t;
        }
    }
}

void saveToFile(FILE *f, char ** tab, int n){
	int it;
	for(it = 0; it < n; it++){
		fprintf(f, "%s\n", tab[it]);
	}
}

void printTable(char ** tab, int n){
	int it;
	for(it = 0; it < n; it++){
		puts(tab[it]);
	}
}

char ** readData(FILE *ptr, int *i, int *size){
	char **tab = malloc((*size) * sizeof(char**));

	char line[1024];//assumption, that word found in file is shorter, than 1024 characters
	
	while (fscanf(ptr, "%s", line) != EOF ){	
		 tab[*i] = malloc(strlen(line) * sizeof(char));
		 strcpy(tab[*i], line);
		 if((*i) == (*size - 1)){//if an 'tab' array is going to be too small
		 	(*size) *= 2;		 //I reallocate it to TWO TIMES LARGER one using 'realloc' function.
		 	tab = realloc(tab, (*size) * sizeof(char**));
		 }
		 (*i)++;
	}
	return tab;
}

int compareString(char *s1, char *s2){//returns 0, when first 
	int count = 0;                    //string is bigger, than the second one, 1 otherwise
	int min = MIN((int)strlen(s1), (int)strlen(s2));

	while(count < min ){
		if(tolower(s1[count]) == tolower(s2[count])){
			if(count == min - 1){                       //handling when we have one wor i.e. "vel" and second "velvet"
				if((int)strlen(s1) == (int)strlen(s2))  //we have to decide, that "vel" is before "velvet"
					return 1;							//after sorting
				if((int)strlen(s1) > (int)strlen(s2))
					return 0;
				if((int)strlen(s1) < (int)strlen(s2))
					return 1;
			}
			count++;
			continue;
		}
		if(tolower(s1[count]) > tolower(s2[count]))
			return 0;
		if(tolower(s1[count]) < tolower(s2[count]))
			return 1;
		count++;
	}
	return 1;
}
