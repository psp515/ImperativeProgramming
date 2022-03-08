#include <stdio.h>
#include <math.h> // for NAN

#define STACK_SIZE 10

#define OK        0
#define EMPTY    -1
#define FULL     -2
#define OVERFLOW -3

double stack[STACK_SIZE];
int top = 0;

int stack_push(double x)
{
    if (top==10)
        return OVERFLOW;

    stack[top]=x;
    ++top;
}

double stack_pop(void)
{
    if(top==0)
        return NAN;

    --top;
    return stack[top];
}

int stack_state(void)
{
    return top==10?FULL:top;
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE]; //kolejka
int in = 0; // liczba osob w kolejce
int curr_nr = 0; //aktaulny numerek dla nowej osby w kolejce

// ONE OVERFLOW for many resigning

void queue_push(int in_nr)
{
    int isOverflowed = 0;
    while (in_nr>0)
    {
        curr_nr+=1;
        if(in < QUEUE_SIZE)
        {
            
            queue[in] = curr_nr;
            in +=1;
        }
        else
        {
            if(isOverflowed==0){
                isOverflowed=1;
                printf("OVERFLOW ");
            }
        }

        in_nr-=1;
    }
    
}

void queue_pop(int out_nr)
{
    if(out_nr>in){
        printf("UNDERFLOW");
    }

    if(out_nr==in){
        in = 0;
        return;
    }

    int j = 0;
    for(int i = out_nr;i<in;i++)
    {
        queue[j]=queue[i];
        j++;
    }

    in -= out_nr;
}

void queue_state(void)
{
    if(in==0)
    {
        printf("EMPTY");
        return;
    }

    for(int i = 0;i<in;i++)
    {
        printf("%d ", queue[i]);
    }
}

// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out=0;
int len=0;


void cbuff_push(int cli_nr)
{
    // tutaj cos nie dziala ++z cli i zaczyna dawac od 0 plus blad w tescie 
    // zapytac o to jutro
    if(len==CBUFF_SIZE)
    {
        printf("OVERFLOW ");
        return;
    }

    ++len;
    cbuff[(out+len)%CBUFF_SIZE]= ++cli_nr;

}

void cbuff_pop(void)
{
    if(len==0)
    {
        printf("UNDERFLOW ");
        return;
    }

    --len;
    ++out;
    out=out%10;

}

void cbuff_state(void)
{
    if(len==0)
    {
        printf("EMPTY ");
        return;
    }
    
    for(int i = 0;i<len;i++)
    {
        printf("%d ", cbuff[(out+i)%CBUFF_SIZE]);
    }

}

int main(void) {
    int to_do, n, client_no, op_code;
    double x;
    scanf("%d",&to_do);
    switch(to_do) {
        case 1: // stack
            do {
                scanf("%lf",&x);
                if(x > 0) stack_push(x);
                else {
                    if(x < 0) printf("%.2f ", stack_pop());
                    else printf("\n%d\n", stack_state());
                }
            } while(x != 0);
            break;
        case 2: // FIFO queue with shifts
            do {
                scanf("%d",&n);
                if(n > 0) 
                    queue_push(n);
                else 
                {
                    if(n < 0) 
                        queue_pop(-n);
                    else 
                        queue_state();
                }
            } while(n != 0);
            break;
        case 3: // queue with cyclic buffer
            client_no = 0;
            do {
                scanf("%d",&op_code);
                if(op_code > 0) 
                    cbuff_push(++client_no);
                else 
                {
                    if(op_code < 0) 
                        cbuff_pop();
                    else 
                        cbuff_state();
                }
            } while(op_code != 0);
            break;
        default:
            printf("NOTHING TO DO!\n");
    }
    return 0;
}
