#include <stdio.h>      // printf
#include <string.h>     // strlen, strncpy
#include <ctype.h>      // isalnum
#include <stdlib.h>     // malloc, calloc
 
#define TRUE    1
#define FALSE   0
 
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
 
/*
    A logical type
 */
typedef enum {
    false,
    true,
} bool;


typedef struct twitter{
	char* id;
	char* text;
	struct twitter* next;
}twitter;

typedef struct Alltwits{
	twitter* head;
}Alltwits;

typedef struct _ListWithLength {
    char **list;
    size_t length;
} ListWithLength;


Alltwits* alt;

twitter* createtwitter()
{
	twitter* t = (twitter*) malloc(1 * sizeof(twitter));
	t->id = (char*) malloc(256 * sizeof(char));
	t->text = (char*) malloc(256 * sizeof(char));
	t->next = NULL;
return t;
}

Alltwits* createAlltwits()
{
	Alltwits* t = (Alltwits*) malloc(1 * sizeof(Alltwits));
	t->head = NULL;
	return t;
}

void printAlltwits(){
	if (alt == NULL)
	{
		printf("Estrutura Vazia!\n");
		exit(1);
	}
	if (alt->head == NULL)
	{
		printf("Fila Vazia!\n");
		exit(1);
	}
	else{
		twitter* aux;
		aux = alt->head;
		while (aux->next != NULL){
		//	printf("%s ",aux->id);
		//	printf("%s ",aux->text);
		//	printf("\n");
			aux = aux->next;
		}
	}
}

void addtwitter (twitter* t){
	//printf("\n");
	//printf("%s\n", t->id);
	//printf("%s\n", t->text);
	//printf("\n");

	twitter* aux = createtwitter();
	if (alt->head == NULL){
		alt->head = t;
	}
	else{
		aux = alt->head;
		while (aux->next != NULL){
			aux = aux->next;
		}
		printf("aooooooo%s \n", aux->id);
		aux->next = t;
	}
}

void read_File(){
	FILE *arq = fopen("filtrado2.txt","r");
	if (arq){
		char lineID[256];
		char lineText[256];
		while (!feof(arq)) {
			if(fgets(lineID, sizeof(lineID), arq) != NULL && fgets(lineText, sizeof(lineText), arq) != NULL){
				twitter* t = createtwitter();
				t->id =	fgets(lineID, sizeof(lineID), arq);
				t->text = fgets(lineText, sizeof(lineText), arq);
				addtwitter(t);
			}
		}
	}
	else{
		printf("ERRO! \n");
		exit(1);
	}
	fclose(arq);
}


/*
    Parse a text and return pointer to a ListWithLength words and count it
 */
ListWithLength* getWords(char *text) {

    // a variable for count words
    int count = 0;

    // keep length of the text
    size_t text_len = strlen(text);

    // a flag indicating the a beginning of a word
    bool new_word = false;

    // an index of a start found a word
    int index_start_word = 0;

    // 2D-array for found word
    // it will be same memory size as the original text
    char **words = malloc(text_len * sizeof(char));
    int i;
    for (i = 0; i <= text_len; ++i) {

        // if found ascii letter or digits and new no traced early
        // keep index of beginning a new word
        // and change the flag
        if (isalnum(text[i]) != 0) {
            if (new_word == false) {
                new_word = true;
                index_start_word = i;
            }

        // if it is not ascii letter or digits and a word traced early
        // it means the word ended
        } else {
            if (new_word == true) {

                // allocate a memory for a new word in the array of words
                words[count] = malloc(i - index_start_word * sizeof(char) + 1);

                // copy the found word from the text by indexes
                strncpy(words[count], text + index_start_word, i - index_start_word);

                // change the flag
                new_word = false;

                // increase the counter of words
                count++;
            }
        };
    }

    // bind the found words and it count to a structure and return it
    ListWithLength *list_with_length = malloc(sizeof(ListWithLength));

    list_with_length->length = count;
    list_with_length->list = words;

    return list_with_length;
}

void printListWithLength(ListWithLength *list_with_length) {
    printf("Total items: %li\n", list_with_length->length);
    puts("----------------------");
    int i;
    for (i = 0; i < list_with_length->length; ++i) {
        printf("%d. %s\n", i + 1, list_with_length->list[i]);
    }
}

double jaro(const char *str1, const char *str2) {
    // length of the strings
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
 
    // if both strings are empty return 1
    // if only one of the strings is empty return 0
    if (str1_len == 0) return str2_len == 0 ? 1.0 : 0.0;
 
    // max distance between two chars to be considered matching
    // floor() is ommitted due to integer division rules
    int match_distance = (int) max(str1_len, str2_len)/2 - 1;
 
    // arrays of bools that signify if that char in the matching string has a match
    int *str1_matches = calloc(str1_len, sizeof(int));
    int *str2_matches = calloc(str2_len, sizeof(int));
 
    // number of matches and transpositions
    double matches = 0.0;
    double transpositions = 0.0;
    int i =0;
    // find the matches
    for (i =0; i < str1_len; i++) {
        // start and end take into account the match distance
        int start = max(0, i - match_distance);
        int end = min(i + match_distance + 1, str2_len);
        int k;
        for (k = start; k < end; k++) {
            // if str2 already has a match continue
            if (str2_matches[k]) continue;
            // if str1 and str2 are not
            if (str1[i] != str2[k]) continue;
            // otherwise assume there is a match
            str1_matches[i] = TRUE;
            str2_matches[k] = TRUE;
            matches++;
            break;
        }
    }
 
    // if there are no matches return 0
    if (matches == 0) {
        free(str1_matches);
        free(str2_matches);
        return 0.0;
    }
 
    // count transpositions
    int k = 0;
    for (i = 0; i < str1_len; i++) {
        // if there are no matches in str1 continue
        if (!str1_matches[i]) continue;
        // while there is no match in str2 increment k
        while (!str2_matches[k]) k++;
        // increment transpositions
        if (str1[i] != str2[k]) transpositions++;
        k++;
    }
 
    // divide the number of transpositions by two as per the algorithm specs
    // this division is valid because the counted transpositions include both
    // instances of the transposed characters.
    transpositions /= 2.0;
 
    // free the allocated memory
    free(str1_matches);
    free(str2_matches);
 
    // return the Jaro distance
    return ((matches / str1_len) +
        (matches / str2_len) +
        ((matches - transpositions) / matches)) / 3.0;
}

double media(size_t tam, double *valortext){
    double media =0;
    int i;
        for (i = 0; i < tam; i++){
            media += valortext[i];
        }

    return (media/tam);
}

double stringCompare(ListWithLength* frase, ListWithLength* frase2){
	int i,j,k,l;
    double mediaf;
    l = frase->length*frase2->length;
    double x[l];
    
	i = 0;
	for (j = 0; j < frase->length; j++)
	{
		for (k = 0; k < frase2->length; k++)
		{
			x[i] = jaro(frase->list[j],frase2->list[k]);
			i++;
		}
	}
	mediaf = media(l,x);
	return mediaf;
}

int main(int argc, char const *argv[])
{
	alt = createAlltwits();

    char c_keywords[300] = "Religious and ideological sites carry more malware than porn sites http://t.co/FGWvdfw0 #GooglePhone";
    char teste[300] = "Religious Sites Carry More Malware Than Porn Sites, Security Firm Reports http://t.co/A1E6zLRz";

    ListWithLength *frase = getWords(c_keywords);
    ListWithLength *frase2 = getWords(teste);
	read_File();
	
	//printf("%s", alltwits->next->id);
	//printf("%s", alltwits->next->text);
	printAlltwits();

	
	//double x = stringCompare(frase,frase2);	
	//printf("%f",x);

    return 0;
}
