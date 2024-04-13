#include <stdio.h>
#include <Windows.h>
#include <time.h>

#define BUFFER_SIZE 1024

int main () 
{
    HANDLE hPipe;
    DWORD dwBytesRead;
    int quantidade, peso, qtd_total = 0, peso_total = 0;
    int items_received = 0;
    SYSTEMTIME last_print_time, current_time;

    // Create named pipe
    hPipe = CreateNamedPipe("\\\\.\\pipe\\pipeso",
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            1,
                            BUFFER_SIZE,
                            BUFFER_SIZE,
                            0,
                            NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Falha em criar o pipe. Codigo do erro: %d\n", GetLastError());
        return 1;
    }
    
    // Initialize last print time
    GetSystemTime(&last_print_time);

    while(1) {
        // Wait for client to connect
        if (!ConnectNamedPipe(hPipe, NULL))
        {
            printf("Falha em conectar ao cliente. Codigo do erro: %d\n", GetLastError());
            CloseHandle(hPipe);
            return 1;
        }

        // Read data from client
        if (!ReadFile(hPipe, &quantidade, sizeof(int), &dwBytesRead, NULL) ||
            !ReadFile(hPipe, &peso, sizeof(int), &dwBytesRead, NULL)) {
            printf("Falha em ler do pipe. Codigo do erro: %d\n", GetLastError());
            CloseHandle(hPipe);
            return 1;
        }
        
        // Atualiza total
        qtd_total += quantidade;
        peso_total += peso;
        items_received += quantidade;
        
        // Get current time
        GetSystemTime(&current_time);

        // Calculate elapsed time since last print
        int elapsed_seconds = (current_time.wSecond - last_print_time.wSecond) +
                              60 * (current_time.wMinute - last_print_time.wMinute) +
                              3600 * (current_time.wHour - last_print_time.wHour);

        // If two seconds have elapsed, print the total quantity
        if (elapsed_seconds >= 2)
        {
            printf("Quantidade total atualizada: %d\n", qtd_total);

            // Update last print time
            last_print_time = current_time;
        }

        // If ten items have been received, print the total weight
        if (items_received >= 10)
        {
            printf("Peso total atualizado: %d\n", peso_total);
            items_received = 0; // Reset item counter
        }
        
        // Disconnect from client
        DisconnectNamedPipe(hPipe);
    }

    // Close pipe and exit
    CloseHandle(hPipe);
    return 0;
}