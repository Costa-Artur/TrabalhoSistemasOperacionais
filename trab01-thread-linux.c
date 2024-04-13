#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int valor_total = 0;
int itens_recebidos = 0;
int peso_total = 0;
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

void alteraValor( int delta, int peso ) {
    pthread_mutex_lock(&em);
    valor_total += delta;
    peso_total += peso;
    itens_recebidos += delta;
    pthread_mutex_unlock(&em);
}

void thread_esteira_um (void) {
    while (1)
    {
        sleep(2);
        alteraValor(1, 5);
    }
}

void thread_esteira_dois (void) {
    while (1)
    {
        sleep(1);
        alteraValor(1, 2);
    }
}

void thread_display (void) {
    while(1)
    {
        if(itens_recebidos >= 10) {
            printf("Peso atual: %d\n", peso_total);
            itens_recebidos = 0;
        } 
        sleep(2);
        printf("Produtos passados: %d\n", valor_total);
    }
}

int main (int argc, char *argv[]) {

    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, (void *) thread_esteira_um, NULL);
    pthread_create(&t2, NULL, (void *) thread_esteira_dois, NULL);
    pthread_create(&t3, NULL, (void *) thread_display, NULL);

    printf("Digite enter para temrinar o programa:\n");
    getchar();
}