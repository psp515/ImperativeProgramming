#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union
{
    int int_data;
    char char_data;
    void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element
{
    struct ht_element *next;
    data_union data;
} ht_element;

typedef struct hash_table
{
    int size;
    int no_elements;
    ht_element **ht; // dlaczego tu jest ** a nie po prostu *

    DataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    HashFp hash_function;
    DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if(ptr) return ptr;
    exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
// OK
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
             CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data)
{
    if (size == 0)
    {
        p_table->size = size;
        p_table->no_elements = 0;
    }
    else
    {
        p_table->no_elements = 0;
        p_table->size = size;

        p_table->ht = safe_malloc(size * sizeof(ht_element*));

        for(int i = 0; i < size; i++)
        {
            p_table->ht[i] = safe_malloc(sizeof(ht_element));
            p_table->ht[i]->next = NULL;
        }
    }

    p_table->hash_function = hash_function;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->modify_data = modify_data;
    p_table->dump_data = dump_data;
}

// print elements of the list with hash n
// OK
void dump_list(const hash_table* p_table, int n)
{
    ht_element* ht = p_table->ht[n]->next;
    
    while (ht != NULL)
    {
        p_table->dump_data(ht->data);
        ht = ht->next;
    }

}

// Free element pointed by data_union using free_data() function
// Tu sie moze wysypać
void free_element(DataFp free_data, ht_element *to_delete)
{

    if (free_data != NULL)
        free_data(to_delete->data);

    free(to_delete);

}

// free all elements from the table (and the table itself)
// Ok
void free_table(hash_table* p_table)
{
    ht_element** ht = p_table->ht;

    for(int i = 0; i < p_table->size; i++)
    {

        ht_element *x = p_table->ht[i];
        while (x != NULL)
        {
            ht_element *tmp = x->next;
            free_element(p_table->free_data, x);
            x = tmp;
        }
        
    }

    free(p_table->ht);
}

// calculate hash function for integer k
int hash_base(int k, int size)
{
    static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
    double tmp = k * c;
    return (int)floor(size * (tmp - floor(tmp)));
}

//TODO
void rehash(hash_table *p_table)
{
    int n = p_table->size;

    p_table->ht = realloc(p_table->ht, 2* n * sizeof(ht_element *));
    // jak powinna działąc ta funkcja

    for(int i = 0; i < n; i++)
    {
        ht_element* curr = p_table->ht[i];
        ht_element *prev   = NULL;

        while (curr != NULL)
        {
            int n = p_table->hash_function(curr->data, p_table->size);
            
            if (n == i)
                continue;
            
            // insert_element(p_table)
            
            prev = curr;
            curr = curr->next;  
        }
        
    }
        // p_table->ht[i] = realloc( p_table->ht[i], sizeof *p_table->ht[i] * n);
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first)
{
    // skip
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data)
{
    // skip
}

// insert element
void insert_element(hash_table *p_table, data_union *data)
{
    // (stosunek liczby elementow tablicy do jej dlugosci)
    if(p_table->no_elements / p_table->size > MAX_RATE)
        rehash(p_table);
    
    int n = p_table->hash_function(*data, p_table->size);

    ht_element * new_el = safe_malloc(sizeof(ht_element));
    new_el->data = *data;
    new_el->next = NULL;

    ht_element* ht = p_table->ht[n];

    // to powinno działac jak insert sorted ???

    while (ht->next != NULL) // tu moze sie wykrzaczyc
    {
        int x = p_table->compare_data(*data, ht->next->data);
        if(x < 0)
            break;

        if (x == 0) // nie umieszczam duplikatów
            return;

        ht = ht->next;
    }
    ht_element *tmp = ht->next;
    ht->next = new_el;
    new_el->next = tmp;
    

    p_table->no_elements += 1 ;
}

// remove element
void remove_element(hash_table *p_table, data_union data)
{
    // tu sie cos posypało znowu
    int n = p_table->hash_function(data, p_table->size);

    ht_element* ht = p_table->ht[n]->next;
    ht_element* prev = p_table->ht[n];
    
    while ( ht != NULL) // tu moze sie wykrzaczyc
    {
        if(p_table->compare_data(ht->data, data) == 0)
            break;
        prev = ht;
        ht = ht->next;
    }

    if (ht == NULL)
        return;

    prev->next = ht->next;

    if(p_table->free_data != NULL)
        p_table->free_data(ht->data);
        
    //free(ht); /// to robi wyjątek

    p_table->no_elements -= 1;
}

// type-specific definitions

// int element

int hash_int(data_union data, int size)
{
    return hash_base(data.int_data, size);
}

void dump_int(data_union data)
{
    printf("%d ", data.int_data);
}

// returns x > 0 if a > b
int cmp_int(data_union a, data_union b)
{
    return b.int_data - a.int_data;
}

// read int value and insert to the union
data_union create_data_int()
{
    int x;
    scanf("%d", &x);

    data_union data;
    data.int_data = x;

    return data;
}

// char element

int hash_char(data_union data, int size)
{
    return hash_base((int)data.char_data, size);
}

void dump_char(data_union data)
{
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b)
{
    char aw = tolower(a.char_data );
    char bw = tolower(b.char_data );

    if (aw < bw)
        return -1;
    else if (aw > bw)
        return 1;
    else
        return 0;
}

// read char value and insert to the union
data_union create_data_char()
{
    char x;
    scanf(" %c", &x);

    data_union data;
    data.char_data = x;

    return data;
}

// Word element

typedef struct DataWord
{
    char *word;
    int counter;
} DataWord;

void dump_word(data_union data)
{
    DataWord x = *(DataWord*) data.ptr_data;
    printf("%s %d", x.word, x.counter);
}

void free_word(data_union data)
{
    DataWord* x = (DataWord*) data.ptr_data;

    free(x->word);
    free(x);
}

int cmp_word(data_union a, data_union b)
{
    return strcmp( ((DataWord*)a.ptr_data)->word ,((DataWord*)b.ptr_data)->word);
}

int hash_word(data_union data, int size) {
    int s = 0;
    DataWord *dw = (DataWord*)data.ptr_data;
    char *p = dw->word;
    while (*p) {
        s += *p++;
    }
    return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data)
{
    DataWord *x = (DataWord*) data;
    x->counter += 1;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value)
{
    DataWord *x = safe_malloc(sizeof(DataWord));

    x->word = value;
    x->counter = 1;

    data_union data;
    data.ptr_data = x;

    return data;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream)
{
    char buff[BUFFER_SIZE];
    char *ptr = fgets(buff, BUFFER_SIZE, stream);
    char *sep1 = ".,?!:;-\n\t\r ";

    while (ptr != NULL)
    {
        char *word;
        for(word = strtok(buff, sep1); word; word = strtok(NULL, sep1))
        {
            char *element = malloc((strlen(word) + 1) * sizeof(char));
            element = strdup(word);

            data_union data = create_data_word(element);

            insert_element(p_table, &data);
        }

        ptr = fgets(buff, BUFFER_SIZE, stream);
    }
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
    char op;
    data_union data;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        data = create_data();
        switch (op) {
            case 'r':
                remove_element(p_table, data);
                break;
            case 'i':
                insert_element(p_table, &data);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n, index;
    hash_table table;
    char buffer[BUFFER_SIZE];
    data_union data;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_int,NULL,
                    cmp_int, hash_int, NULL);
            table_test(&table, n, create_data_int);
            printf ("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 2: // test char table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_char,NULL,
                    cmp_char, hash_char, NULL);
            table_test(&table, n, create_data_char);
            printf ("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 3: // read words from text, insert into table, and print
            scanf("%s", buffer);
            init_ht(&table, 8, dump_word, free_word,
                    cmp_word, hash_word, modify_word);
            stream_to_ht(&table, stdin);
            printf ("%d\n", table.size);
            data = create_data_word(buffer);
            ht_element *e = get_element(&table, &data);
            if (e) table.dump_data(e->data);
            if (table.free_data) table.free_data(data);
            free_table(&table);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

