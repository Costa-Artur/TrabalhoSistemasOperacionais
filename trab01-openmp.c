#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int valor_total = 0;
int itens_recebidos = 0;
int peso_total = 0;

void alteraValor(int quantidade, int peso) 
{
    #pragma omp critical
    {
        valor_total += quantidade;
        peso_total += peso;
        itens_recebidos += quantidade;
    }
}

void thread_esteira_um(void)
{
    while(1)
    {
        sleep(2);
        alteraValor(1, 5);
    }
}

void thread_esteira_dois(void)
{
    while(1)
    {
        sleep(1);
        alteraValor(1,2);
    }
}

void thread_display(void)
{
    while(1)
    {
        if(itens_recebidos >= 10)
        {
            printf("Peso atual: %d\n", peso_total);
            itens_recebidos = 0;
        }
        sleep(2);
        printf("Produtos passados: %d\n", valor_total);
    }
}

int main (int argc, char *argv[]) 
{

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            thread_esteira_um();
        }
        #pragma omp section
        {
            thread_esteira_dois();
        }
        #pragma omp section
        {
            thread_display();
        }
    }
}