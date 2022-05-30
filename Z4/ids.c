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
char tab[MAX_IDS][MAX_ID_LEN];

char words[8000][64] = {0};

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

// checks if word is keyword
int is_keyowrd(int index)
{
    for (int i = 0; i < 32; i++)
    {
        int same = strcmp(words[index], keywords[i]);
        if(same == 0)
            return 1;
    }

    return  0;
}

// checks if word is in tab
int is_in_tab(int index, int last_in_tab)
{
    for(int i =0;i<last_in_tab;i++)
    {
        int same = strcmp(words[index], tab[i]);
        if(same == 0)
            return 1;
    }

    return 0;
}

// count unical identificators
int count_idents()
{
    int word_len = 0;
    int index = 0;
    int count = 0;

    while (*words[word_len] != 0)
        word_len++;

    for(int i = 0; i < word_len; i++)
    {
        int valid = 0;

        if(words[i][0] == 95 ||  (words[i][0] > 96 && words[i][0]<123) || (words[i][0] > 64 && words[i][0] < 91))
        {
            valid = 1;

            for(int j = 1; j < strlen(words[i]); j++)
            {
                if(!((words[i][j] == 95) || (words[i][j] > 64 && words[i][j] < 91) ||(words[i][j] > 96 && words[i][j] < 123) || (words[i][j]>47 && words[i][j]<58)))
                    valid = 0;

            }

            if(valid==1)
            {
                if(is_keyowrd(i) == 1)
                    continue;

                if(is_in_tab(i, index))
                    continue;

                strcpy(tab[index],words[i]);

                index++;
                count++;
            }
        }
    }

    return count;
}

// main func
int find_idents()
{
    //for geting signs
    int i = 0;
    char c = getchar();
    char helper[26000] = {0};

    //Flags
    int falg = 0;
    int flagline = 0;
    int flagblock = 0;
    int string_flag = 0;
    int second_string_flag = 0;

    //other
    char word[64] = {0};
    int j = 0;
    int index = 0;

    // geting signs
    while(c!=EOF)
    {
        helper[i] = c;
        i++;
        c = getchar();
    }

    //looks for words
    for(int itr =0; itr < i; itr++)
    {

        // Tutaj dodane 2 rzeczy moze da sie to jakos inaczej poukładać ale mi sie nie chciało

        if(helper[itr]=='\'' && helper[itr-1]=='\\' && helper[itr-2]=='\\')
        {
            second_string_flag = 0;
            continue;
        }

        if(helper[itr]=='\\')
        {
            itr++;
            continue;
        }
        // Koniec dodanych rzeczy
        // nizej jeszcze 1 rzecz dodana pomijanie ':'


        if(helper[itr] == 39 && flagline == 0 && flagblock == 0 && string_flag == 0 && second_string_flag==0)
        {
            second_string_flag = 1;
        }
        else
        {
            if(helper[itr] == 34 && flagline ==0 && flagblock == 0 && second_string_flag == 0 && string_flag==0)
            {
                string_flag = 1;
            }
            else
            {
                if(second_string_flag == 1)
                {
                    if(helper[itr] == 39)
                    {
                        if(helper[itr-1]!='\\')
                            second_string_flag = 0;
                    }

                }
                else
                {
                    if(string_flag == 1 )
                    {
                        if(helper[itr]==34)
                        {
                            if(helper[itr-1]!='\\')
                                string_flag = 0;
                        }
                        continue;
                    }
                    else
                    {

                        if(flagline == 0 && flagblock == 0 && helper[itr] == 47 && helper[itr-1] == 47)
                        {
                            flagline=1;
                            continue;
                        }

                        if(flagline==1 && helper[itr]==10)
                        {
                            flagline=0;
                            continue;
                        }

                        if(flagblock == 0 && (helper[itr-1] == 47 && helper[itr]==42))
                        {
                            flagblock=1;
                            continue;
                        }

                        if(flagblock == 1 && ((helper[itr-1] == 42 && helper[itr]==47)))
                        {
                            flagblock=0;
                            continue;
                        }

                        if(helper[itr]!=32 && helper[itr]!=61 && helper[itr]!=10 && helper[itr]!=59&&helper[itr]!=9 && helper[itr]!=0 && helper[itr]!=40
                           && helper[itr]!=41 && flagline ==0 && flagblock==0 && helper[itr]!=43 && helper[itr]!=45 && helper[itr]!=42 && helper[itr]!=47
                           && helper[itr]!=44 && helper[itr]!=93 && helper[itr]!=91 && helper[itr]!=58)
                        {
                            // w tym if ie dodane do pominiecia helper[itr] != 58
                            word[j]=helper[itr];
                            j++;
                            falg = 1;
                        }

                        if(flagline == 1 || flagblock== 1 || helper[itr]==32 || helper[itr]==61 || helper[itr]==59 || helper[itr]==10 || helper[itr]==9 || helper[itr]==0 ||
                           helper[itr]==40 || helper[itr]==41 ||  helper[itr]==43|| helper[itr]==45 || helper[itr]==42 || helper[itr]==47 || helper[itr]==44 || helper[itr]==93 || helper[itr]==91)
                        {
                            j=0;
                            if(falg==1)
                            {
                                falg = 0;
                                strcpy(words[index],word);
                                memset(word, 0, sizeof(word));
                                index++;
                            }
                        }
                    }
                }
            }
        }
    }

    return count_idents();
}

int main(void)
{
    printf("%d\n", find_idents());
    int i =0;
    return 0;
}