#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1
#define INVALID_VALUE -2137

// list node
typedef struct Node
{
    int *data;
    int array_size;
    struct Node* next;
    struct Node* prev;
} Node;

// doubly linked list
typedef struct List
{
    Node *head;
    Node *tail;
    int size;
} List;


// initialize list
void init(List *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// ---------------------------------------------- to implement ...

// append element to the list
void push_back(List *list, int *data, int array_size)
{
    struct Node *node = malloc(sizeof (struct Node));

    if(node == NULL)
        exit(MEMORY_ALLOCATION_ERROR);

    node->data = malloc(array_size* sizeof (int));
    node->array_size = array_size;
    node->next = NULL;
    node->prev = NULL;

    if(node->data == NULL)
        exit(MEMORY_ALLOCATION_ERROR);

    for(int i = 0; i < array_size; i++)
        node->data[i] = data[i];

    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
}

// forward iteration - get n-th element in the list
int get_forward(List *list, int n)
{
    Node * first = list->head;

    while (first->next != NULL)
    {
        int size = first->array_size;

        if (n - size <= 0)
            break;

        n-=size;
        first = first->next;

    }

    if (first == NULL)
        exit(INVALID_VALUE);

    return first->data[n-1];
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, int n)
{
    Node * last = list->tail;

    while (last->prev != NULL)
    {
        int size = last->array_size;

        if (n - size <= 0)
            break;

        n-=size;
        last = last->prev;
    }

    if (last == NULL)
        exit(INVALID_VALUE);

    return last->data[last->array_size - n];
}

// remove n-th element; if array empty remove node
void remove_at(List *list, int n)
{
    Node * first = list->head;

    while (first->next != NULL)
    {
        int size = first->array_size;

        if (n - size <= 0)
            break;

        n-=size;
        first = first->next;
    }

    if (first == NULL)
        exit(INVALID_VALUE);

    // usuwamy n-1 element
    for(int i = n; i < first->array_size; i++)
        first->data[i-1] = first->data[i];

    first->array_size -= 1;

    if(first->array_size == 0)
    {
        if(first == list->head)
        {
            list->head = first->next;
            list->head->prev = NULL;
        }
        else if(first == list->tail)
        {
            list->tail = first->prev;
            list->tail->next = NULL;
        }
        else
        {
            first->prev->next = first->next;
            first->next->prev = first->prev;
        }

        free(first->data);
        free(first);
    }
}

// -------------------- helper functions

// print list
void dumpList(const List *list) {
    for(Node *node = list->head; node != NULL; node = node->next) {
        printf("-> ");
        for (int k = 0; k < node->array_size; k++)
            printf("%d ", node->data[k]);

        printf("\n");
    }
}

// remove the first element
void delete_front(List *list)
{
    Node *to_delete;
    if(list->head == NULL)
        return;

    to_delete = list->head;
    list->head = list->head->next;

    if(list->head == NULL)
        list->tail = NULL;

    free(to_delete->data);
    free(to_delete);
    list->size--;
}

// free list
void freeList(List *list)
{
    while(list->head)
        delete_front(list);

}

// read int vector
void read_vector(int tab[], int n)
{
    for (int i = 0; i < n; ++i)
        scanf("%d", tab + i);

}

// initialize the list and push data
void read_list(List *list)
{
    int size, n;
    init(list);
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &size);
        int *tab = (int*) malloc((size_t)size * sizeof(int));
        read_vector(tab, size);
        push_back(list, tab, size);
    }
}

int main() {
    int to_do, size, m;
    List list;

    scanf("%d", &to_do);
    read_list(&list);
    switch (to_do) {
        case 1:
            dumpList(&list);
            break;
        case 2:
            scanf("%d", &size);
            for (int i = 0; i < size; i++)
            {
                scanf("%d", &m);
                printf("%d ", get_forward(&list, m));
            }
            printf("\n");
            break;
        case 3:
            scanf("%d", &size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &m);
                printf("%d ", get_backward(&list, m));
            }
            printf("\n");
            break;
        case 4:
            scanf("%d", &size);
            for (int i=0; i < size; i++) {
                scanf("%d", &m);
                remove_at(&list, m);
            }
            dumpList(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    freeList(&list);

    return 0;
}