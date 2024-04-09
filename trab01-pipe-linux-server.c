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
    char buffer[1024];

    int quantidade, peso, qtd_total=0, peso_total=0;

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
    
    struct timeval last_print_time, current_time;

    while(1) {

        // Get the current time
        gettimeofday(&current_time, NULL);

        // Calculate elapsed time since last print
        long elapsed_time = (current_time.tv_sec - last_print_time.tv_sec) * 1000000 + (current_time.tv_usec - last_print_time.tv_usec);

        // If two seconds have elapsed, print the total
        if (elapsed_time >= 2000000) {
            printf("Total atualizado: Quantidade = %d, Peso = %d\n", qtd_total, peso_total);
            
            // Update last print time
            last_print_time = current_time;
        }

        //Listen for connections
        if(listen(sockfd, 5) < 0)
        {
            perror("Falha em escutar o socket");
            close(sockfd);
            return 1;
        }

        printf("Servidor named pipe ouvindo em %s... \n", SOCK_PATH);

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
    
    }
    

    //Closes sockets and exit
    close(newsockfd);
    close(sockfd);
    return 0;
}