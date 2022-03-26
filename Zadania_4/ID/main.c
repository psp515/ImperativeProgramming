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

char words[1000][64] = {0};

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

void create_words(char helper[], int end){
    int flag =0;
    int flagline = 0;
    int flagblock = 0;
    char word[MAX_ID_LEN] = {0};
    int j = 0;
    int wordidx = 0;
    int string_flag = 0;
    int second_string_flag = 0;

    for(int i=0;i<end;i++)
    {

        if(helper[i]==34 && flagline ==0 && flagblock == 0 && second_string_flag == 0 && string_flag!=1)
        {
            string_flag = 1;
            continue;
        }
        if(helper[i]==39 && flagline ==0 && flagblock == 0 && string_flag == 0 && second_string_flag!=1)
        {
            second_string_flag = 1;
            continue;
        }
        if(string_flag == 1 )
        {
            if(helper[i]==34)
            {
                if(helper[i-1]!='\\')
                {
                    string_flag = 0;
                }
            }

            continue;
        }
        if(second_string_flag == 1)
        {
            if(helper[i] == 39)
            {
                if(helper[i-1]!='\\')
                {
                    second_string_flag = 0;
                }
            }

            continue;
        }
        if(flagline == 0 && flagblock == 0 && helper[i] == 47 && helper[i-1] == 47)
        {
            flagline=1;
        }
        if(flagline==1 && helper[i]==10)
        {
            flagline=0;
        }


        if(flagblock == 0 && (helper[i-1] == 47 && helper[i]==42))
        {
            flagblock=1;
        }

        if(flagblock == 1 && ((helper[i-1] == 42 && helper[i]==47)))
        {
            flagblock=0;
        }

        if(helper[i]!=32 && helper[i]!=61 && helper[i]!=10 &&
           helper[i]!=59&&helper[i]!=9 && helper[i]!=0 &&
           helper[i]!=40 && helper[i]!=41
           && flagline ==0 && flagblock==0 && helper[i]!=43 &&
           helper[i]!=45 && helper[i]!=42 &&
           helper[i]!=47 && helper[i]!=44 &&
           helper[i]!=93 && helper[i]!=91){
            word[j]=helper[i];
            j++;
            flag = 1;
        }

        if((helper[i]==32 || helper[i]==61 || helper[i]==59 || helper[i]==10 ||
            helper[i]==9 || helper[i]==0 || helper[i]==40 || helper[i]==41 || flagline ==1 || flagblock==1 || helper[i]==43||
            helper[i]==45 || helper[i]==42 || helper[i]==47 || helper[i]==44 || helper[i]==93 || helper[i]==91))
        {
            j=0;
            if(flag==1)
            {
                flag=0;
                strcpy(words[wordidx],word);
                wordidx++;
                memset(word, 0, sizeof(word));
            }
        }
    }
}

// 0-> to nie keyword,  1 -> to keyword
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

//0 => nie ma w tab, 1 - jest w tab
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

int requirements()
{
    int w_len=0;
    int idx=0;
    int id_count = 0;

    for(int i=0;i<100;i++)
    {
        if(*words[i]==0)
        {
            w_len=i;
            break;
        }
    }

    for(int i=0; i < w_len; i++)
    {
        if(words[i][0] == 95 || (words[i][0] > 64 && words[i][0] < 91) || (words[i][0] > 96 && words[i][0]<123))
        {
            int valid = 1;

            for(int j = 1; j < strlen(words[i]); j++)
            {
                if(!((words[i][j]>64 && words[i][j]<91)||(words[i][j]>96 && words[i][j]<123) || (words[i][j]>47 && words[i][j]<58)))
                {
                    valid = 0;
                    break;
                }
            }

            if(valid == 1)
            {
                if(is_keyowrd(i) == 1)
                    continue;

                if(is_in_tab(i, idx))
                    continue;

                strcpy(tab[idx],words[i]);
                idx++;
                id_count++;

            }
        }
    }

    return id_count;
}

int find_idents()
{
    char c = getchar();
    char helper[25000] = {0};
    int count = 0;

    int i = 0;
    while(c!=EOF)
    {
        helper[i] = c;
        i++;
        c=getchar();
    }

    create_words(helper, i);
    count = requirements();
    return count;
}

int main(void) {
    printf("%d\n", find_idents());
    return 0;
}

