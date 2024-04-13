#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>

#define SOCK_PATH "/tmp/pipeso"

int main()
{
    int sockfd, newsockfd, len;
    struct sockaddr_un local, remote;

    int quantidade, peso, qtd_total=0, peso_total=0;
    int items_received = 0;
    struct timeval last_print_time, current_time;

    //Create Socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("Falha em criar o pipe");
        return 1;
    }

    //Bind socket to local address
    memset(&local, 0, sizeof(local));
    local.sun_family = AF_UNIX;
    strncpy(local.sun_path, SOCK_PATH, sizeof(local.sun_path) - 1);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(sockfd, (struct sockaddr *)&local, len) < 0)
    {
        perror("Falha em capturar o socket");
        close(sockfd);
        return 1;
    }
    
    // Initialize last print time
    gettimeofday(&last_print_time, NULL);

    while(1) {

        // Get the current time
        gettimeofday(&current_time, NULL);

        // Calculate elapsed time since last print
        long elapsed_seconds = (current_time.tv_sec - last_print_time.tv_sec) + (current_time.tv_usec - last_print_time.tv_usec) / 1000000;

        // If two seconds have elapsed, print the total
        if (elapsed_seconds >= 2)
        {
            printf("Total atualizado: Quantidade = %d\n", qtd_total);

            // Update last print time
            gettimeofday(&last_print_time, NULL);
        }

        //Listen for connections
        if(listen(sockfd, 5) < 0)
        {
            perror("Falha em escutar o socket");
            close(sockfd);
            return 1;
        }

        //Accept connections
        memset(&remote, 0, sizeof(remote));
        len = sizeof(remote);
        newsockfd = accept(sockfd, (struct sockaddr *)&remote, &len);
        if(newsockfd < 0)
        {
            perror("Falha em aceitar conexao");
            close(sockfd);
            return 1;
        }

        //Read data from client
        if(read(newsockfd, &quantidade, sizeof(int)) < 0 || read(newsockfd, &peso, sizeof(int))< 0)
        {
            perror("Falha em ler do socket");
            close(newsockfd);
            close(sockfd);
            return 1;
        }

        // Atualiza total
        qtd_total += quantidade;
        peso_total += peso;
        items_received += quantidade;

        if(items_received >= 10) {
            printf("Total atualizado: Peso = %d\n", peso_total);
            items_received = 0;
        }

        
    }
    
    close(newsockfd);   
    close(sockfd);
    return 0;
}