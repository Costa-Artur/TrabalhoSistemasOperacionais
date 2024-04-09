#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/pipeso"

int main ()
{
    int sockfd, len;
    struct sockaddr_un remote;
    int quantidade, peso;

    while(1)
    {
        //Create socket
        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if(sockfd < 0)
        {
            perror("Falha em criar o socket");
            return 1;
        }

        //Connect to server
        memset (&remote, 0, sizeof(remote));
        remote.sun_family = AF_UNIX;
        strncpy(remote.sun_path, SOCK_PATH, sizeof(remote.sun_path) - 1);
        len = strlen(remote.sun_path) + sizeof(remote.sun_family);
        if(connect(sockfd, (struct sockaddr *)&remote, len) < 0)
        {
            perror("Falha em conectar no servidor");
            close(sockfd);
            return 1;
        }

        printf("Conectado ao servidor!\n");

        //Declares quantity and weight
        quantidade = 1;
        peso = 5;

        if(write(sockfd, &quantidade, sizeof(int)) < 0 || write(sockfd, &peso, sizeof(int)) < 0)
        {
            perror("Falha em escrever no socket");
            close(sockfd);
            return 1;
        }

        printf("Dado enviado ao servidor\n");

        close(sockfd);

        sleep(2);
    }
    
    
    return 0;

}