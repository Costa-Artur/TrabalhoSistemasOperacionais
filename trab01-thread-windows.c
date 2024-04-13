#include <stdio.h>
#include <Windows.h>
#include <conio.h>

//Declarando variaveis globais
int valor_total = 0;
int peso_total = 0;
int itens_passados = 0;
HANDLE mutex;

//Função para alterar valores globais
void alteraValor(int delta, int peso) {
    WaitForSingleObject(mutex, INFINITE); //Lock
    valor_total += delta;
    peso_total += peso;
    itens_passados += delta;
    ReleaseMutex(mutex); //Unlock
}

//Função da thread 1
DWORD WINAPI thread_esteira_um (LPVOID lpParam) {
    while(1) {
        Sleep(2000);
        alteraValor(1, 5);
    }
}

//Função da thread 2
DWORD WINAPI thread_esteira_dois (LPVOID lpParam) {
    while(1) {
        Sleep(1000);
        alteraValor(1, 2);
    }
}

//Função da thread de display
DWORD WINAPI thread_display (LPVOID lpParam) {
    while(1) {
        if(itens_passados >= 10) {
            printf("Peso total :%d\n", peso_total);
            itens_passados = 0;
        }
        Sleep(2000);
        printf("Produtos passados: %d\n", valor_total);
    }
}

int main() {
    HANDLE t1,t2,t3; //Declara threads
    mutex = CreateMutex(NULL, FALSE, NULL); //Inicializa mutex

    //Cria threads
    t1 = CreateThread(NULL, 0, thread_esteira_um, NULL, 0, NULL);
    t2 = CreateThread(NULL, 0, thread_esteira_dois, NULL, 0, NULL);
    t3 = CreateThread(NULL, 0, thread_display, NULL, 0, NULL);

    printf("Digite enter para terminar o programa:\n");
    _getch();

    //Finaliza threads
    CloseHandle(t1);
    CloseHandle(t2);
    CloseHandle(t3);
    CloseHandle(mutex);

    return 0;
}