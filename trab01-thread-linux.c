#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int valor_total = 0;
int peso_total = 0;
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

int alteraValor( int delta, int peso ) {
    int novo_valor;

    pthread_mutex_lock(&em);
    valor_total += delta;
    peso_total += peso;
    pthread_mutex_unlock(&em);
    return novo_valor;
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
        if(valor_total % 10 == 0) {
            printf("Peso atual: %d\n", peso_total);
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