#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)
#define MAX_LINE 128

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = { 0 };
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
    return count[vb] - count[va];
}
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

void wc(int *nl, int *nw, int *nc) {
    *nl = 0;
    *nw = 0;
    *nc = 0;

    char line[MAX_LINE];

    while ((fgets(line, MAX_LINE, stdin)) != NULL)
    {
        *nl += 1;

        for (int i = 0; i < MAX_LINE; i++)
        {
            *nc += 1;

            if (line[i] == NEWLINE)
            {
                if(i>=1 && line[i - 1] != NEWLINE && line[i - 1] != ' ' && line[i - 1] != '\t')
                {
                    *nw += 1;
                }
                break;
            }

            if (i >= 1 && line[i - 1] != NEWLINE && line[i - 1] != ' ' && line[i - 1] != '\t'
                && (line[i] == '\t'|| line[i] == NEWLINE || line[i] == ' ')){
                *nw += 1;
            }
        }
    }
}

void char_count(int char_no, int *n_char, int *cnt)
{
    for(int j = 0; j < MAX_CHARS; j++)
        count[j] = 0;

    int x;
    while (1)
    {
        x = getchar();
        if (x==EOF)
            break;
        count[x-FIRST_CHAR] = count[x-FIRST_CHAR] + 1;
    }

    int indeks[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; i++)
        indeks[i] = i;

    qsort(indeks,MAX_CHARS, sizeof(int), cmp);

    char_no -= 1;
    *cnt = count[indeks[char_no]];
    *n_char = indeks[char_no] + FIRST_CHAR;
}

void digram_count(int digram_no, int digram[])
{
    for(int j = 0; j < MAX_DIGRAMS; j++)
        count[j] = 0;

    int i = getchar();
    int index[MAX_DIGRAMS];

    for(; i != EOF;)
    {
        int j = getchar();
        if(j != EOF && j > FIRST_CHAR-1 && i > FIRST_CHAR-1 && j < LAST_CHAR && i < LAST_CHAR){
            count[(i-FIRST_CHAR) * MAX_CHARS + (j - FIRST_CHAR)]++;
        }
        i = j;
    }

    for (int i = 0; i < MAX_DIGRAMS; i++)
        index[i] = i;

    qsort(index,MAX_DIGRAMS, sizeof(int), cmp_di);

    digram_no -= 1;
    int val = index[digram_no] % MAX_CHARS + FIRST_CHAR;
    digram[1] = val;
    digram[0] = ((index[digram_no] - val + FIRST_CHAR) / MAX_CHARS) + FIRST_CHAR;
    digram[2] = count[index[digram_no]];
}

//TODO
void find_comments(int *line_comment_counter, int *block_comment_counter)
{
    *line_comment_counter = 0;
    *block_comment_counter = 0;

    int  line4[MAX_LINE];
    int flag_block = 0;

    int c;
    int lenght;
    for (lenght = 0; lenght < MAX_LINE && (c = getchar()) != EOF && c != '\n'; lenght++){
        line4[lenght] = c;
    }

    while (lenght != 0 || c != EOF)
    {
        for (int i = 1; i < lenght; ++i)
        {
            if (flag_block == 0)
            {
                if (line4[i] == '/' && line4[i -1] == '/')
                {
                    *line_comment_counter+=1;
                    break;
                }

                if (line4[i] == '*' && line4[i - 1] == '/') {
                    *block_comment_counter += 1;
                    flag_block = 1;
                }
            }
            if (line4[i] == '/' && line4[i - 1] == '*'){
                flag_block = 0;
                i++;
            }
        }

        lenght = 0;
        for (lenght = 0; lenght < MAX_LINE && (c = getchar()) != EOF && c != '\n'; lenght++){
            line4[lenght] = c;
        }

    }
}

int read_line() {
    char line[MAX_LINE];
    int n;

    fgets (line, MAX_LINE, stdin); // to get the whole line
    sscanf (line, "%d", &n);
    return n;
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int digram[3];

    to_do = read_line();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_line();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // digram_count()
            char_no = read_line();
            digram_count(char_no, digram);
            printf("%c%c %d\n", digram[0], digram[1], digram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
