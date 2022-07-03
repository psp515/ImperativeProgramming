#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

int cmp_person(const void *p1, const void *p2)
{
    Person p1c = *(Person*)p1;
    Person p2c = *(Person*)p2;
    if (p1c.age==p2c.age)
    {
        int i = strcmp(p1c.first_name,p2c.first_name);
        if (i==0)
            return strcmp(p1c.last_name, p2c.last_name);
        return i;
    }
    else if (p1c.age < p2c.age)
        return 1;
    else
        return -1;
}

int read_person_array(Person *persons)
{
    int i =0;
    while(1)
    {
        int isEOF = 0;
        char x = ' ';
        int j = 0;
        char buff[MAX_STR_LEN] = "";
        while (x !='\n')
        {
            x = getchar();
            if (x == EOF)
            {
                isEOF = 1;
                break;
            }
            else
            {
                buff[j] = x;
                j++;
            }
        }

        if (isEOF)
            break;

        Person p = {0, "", ""};

        sscanf(buff, "%d %s %s", &p.age, &p.first_name, &p.last_name);

        persons[i] = p;
        i++;
    }

    return i;
}

void print_person_array(Person *persons, int n)
{
    for(int i = 0;i < n;i++)
        printf("%d %s %s\n",persons[i].age, persons[i].first_name, persons[i].last_name);

}

int is_woman(char* name)
{
    return name[strlen(name)-1] == 'a' ? 1: -1;
}

int cmp_lines(const void *l1, const void *l2)
{
    char*l1c = (char*) l1;
    char*l2c = (char*) l2;

    Person p1 ={0,"",""};
    Person p2 ={0,"",""};
    sscanf(l1c,"%d %s %s",&p1.age,&p1.first_name,&p1.last_name);
    sscanf(l2c,"%d %s %s",&p2.age,&p2.first_name,&p2.last_name);
    int is_p1_woman = is_woman(p1.first_name);
    int is_p2_woman = is_woman(p2.first_name);

    // mans
    if(is_p1_woman == -1 && is_p2_woman==-1)
        return strcmp(p1.last_name,p2.last_name);

    // p1 is woman
    if (is_p1_woman==1 && is_p2_woman==-1)
        return -1;
    // p2 is woman
    if (is_p1_woman == -1 && is_p2_woman == 1)
        return 1;
    // both woman
    if (p1.age < p2.age)
        return -1;

    return 1;
}

int read_lines(char lines[][MAX_STR_LEN])
{
    int i = 0;
    int isEOF = 0;
    while(isEOF == 0)
    {
        char x = ' ';
        int j = 0;
        while(x != '\n')
        {
            x = getchar();
            if(x==EOF)
            {
                if (j>5)
                    i+=1;
                isEOF = 1;
                break;
            }
            else if(x == '\n')
            {
                if(j < 5)
                    isEOF = 1;

                break;
            }
            else
            {
                lines[i][j]=x;
                j++;
            }
        }

        if(isEOF==0)
            i++;
    }

    return i;
}

void print_lines(char lines[][MAX_STR_LEN], int n)
{
    for(int i = 0; i < n; i++)
        printf("%s\n", lines[i]);
}

// -------------------------------------------------

int read_int()
{
    char buf[MAX_STR_LEN];
    int n;
    fgets(buf, MAX_STR_LEN, stdin);
    sscanf(buf, "%d", &n);
    return n;
}

int main(void) {
    int to_do = read_int();
    int n;
    Person persons[MAX_PERSONS];
    char lines[MAX_PERSONS][MAX_STR_LEN];
    switch (to_do) {
        case 1:
            n = read_person_array(persons);
            qsort(persons, (size_t)n, sizeof(Person), cmp_person);
            print_person_array(persons, n);
            break;
        case 2:
            n = read_lines(lines);
            qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
            print_lines(lines, n);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }
}

