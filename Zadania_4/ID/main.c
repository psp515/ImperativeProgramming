#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
};
// 3 typy
// 1 keyword
// 2 definy
// 3 funkcja+(
// komentarze i stringi pomijamy
//
int find_idents()
{
    char x[MAX_ID_LEN];
    /* assumes no word exceeds length of 1023 */
    while (fscanf(stdin, " %64s", x) == 1) {
        puts(x);
    }
    /*int lasti_identifier_place = 0;
    char idents[MAX_ID_LEN] = {0};
    int x = 0;
    int counter = 0;
    int comment_flag = 0;
    while(x !=EOF)
    {
        counter = 0;
        while (counter < 64)
        {
            x = getchar();
            idents[counter] = x;

            // sprawdz czy to nie koniec "

            // sprawdz czy poczatek "

            //sprawdz czy to nie keyword -> zeruj tablice

            // jezeli komentarz to szukaj konca

            // sprawdz czy nie zaczyna sie komentarz //

            //sprawdz czy nie zaczyna sie komentarz /*

            // sprawdz czy to nie bd define

            //sprawdz czy nie ma ( -> sprawdz czy poprawny identyfikator

            // sprawdz czy nie ma ' =' -> sprawdz czy poprawny identyfikator

            counter++;
        }
    }*/
}


int cmp(const void* first_arg, const void* second_arg) {
    char *a = *(char**)first_arg;
    char *b = *(char**)second_arg;
    return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
    int a = *(int*)first_arg;
    int b = *(int*)second_arg;
    return strcmp(tab[a], tab[b]);
}

int main(void) {
    printf("%d\n", find_idents());
    return 0;
}