#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void){
    int sock;
    struct sockaddr_in target;
    char mensagemServer[2000], mensagemCliente[2000];
    int targetLength = sizeof(target);
    
    memset(mensagemServer, '\0', sizeof(mensagemServer));
    memset(mensagemCliente, '\0', sizeof(mensagemCliente));
    
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(sock < 0){
        printf("Não foi possível criar o socket");
        return -1;
    }
    printf("Socket criado com sucesso\n");
    
    target.sin_family = AF_INET;
    target.sin_port = htons(9000);
    target.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    do { 
        printf("Digite 1 (uma) palavra em minisculo (sem espacos) (Caso deseje sair digite 'exit'): ");
        gets(mensagemCliente);

        if(sendto(sock, mensagemCliente, strlen(mensagemCliente), 0,
                (struct sockaddr*)&target, targetLength) < 0){
            printf("Erro \n");
            return -1;
        }

        if(recvfrom(sock, mensagemServer, sizeof(mensagemServer), 0,
                (struct sockaddr*)&target, &targetLength) < 0){
            printf("Erro recebendo a mensagem do servidor \n");
            return -1;
        }

        printf("Servidor converteu para: %s\n", mensagemServer);
    }while (strcmp(mensagemCliente, "exit") != 0);
    
    printf("Cliente encerrado\n");
    close(sock);
    return 0;
}

