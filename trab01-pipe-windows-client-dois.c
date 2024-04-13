#include <stdio.h>
#include <Windows.h>

#define BUFFER_SIZE 1024

int main()
{
    HANDLE hPipe;
    int quantidade = 1, peso = 2;
    DWORD dwRead, dwWritten;

    for(;;) {
        // Open named pipe
        hPipe = CreateFile("\\\\.\\pipe\\pipeso",
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            printf("Falha em abrir o pipe. Codigo do erro: %d\n", GetLastError());
            return 1;
        }

        printf("Conectado ao servidor!\n");

        //Send data to server
        if (!WriteFile(hPipe, &quantidade, sizeof(int), &dwWritten, NULL) ||
            !WriteFile(hPipe, &peso, sizeof(int), &dwWritten, NULL))
        {
            printf("Falha em escrever no pipe. Codigo do erro: %d\n", GetLastError());
            CloseHandle(hPipe);
            return 1;
        }

        printf("Dados enviado ao servidor.\n");

        // Close named pipe and exit
        CloseHandle(hPipe);

        Sleep(1000);
    }
    
    return 0;
}