#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>

int main(void){
    int sock;
    struct sockaddr_in me, from;
    char mensagemServer[2000], mensagemClient[2000];
    int fromLength = sizeof(from);
    
    memset(mensagemServer, '\0', sizeof(mensagemServer));
    memset(mensagemClient, '\0', sizeof(mensagemClient));
    
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(sock < 0){
        printf("Erro criando o sockert\n");
        return -1;
    }
    printf("Socket criado\n");
    
    me.sin_family = AF_INET;
    me.sin_port = htons(9000);
    me.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(bind(sock, (struct sockaddr*)&me, sizeof(me)) < 0){
        printf("Erro no bind da socket\n");
        return -1;
    }
    printf("Socket bindado\n");
    
    printf("Esperando mensagem do cliente \n\n");
    
    while(true){
        if (recvfrom(sock, mensagemClient, sizeof(mensagemClient), 0,
            (struct sockaddr*)&from, &fromLength) < 0){
            printf("Erro na mensagem do cliente\n");
            return -1;
        }
        printf("IP do cliente : %s; Porta do cliente: %i\n",
                inet_ntoa(from.sin_addr), ntohs(from.sin_port));
            
        if(strcmp(mensagemClient, "exit") != 0){
            for(int i=0;i<strlen(mensagemClient);i++){
                if((mensagemClient[i]>='a') && (mensagemClient[i]<='z')){
                    mensagemServer[i] = toupper(mensagemClient[i]);            
                }
            }
        }else{
            strcpy(mensagemServer, "exit");
        }
        if (sendto(sock, mensagemServer, strlen(mensagemServer), 0,
                (struct sockaddr*)&from, fromLength) < 0){
            printf("Erro de envio\n");
            return -1;
        }
        printf("Mensagem enviada para o cliente\n");

        if(strcmp(mensagemClient, "exit") == 0)
            break;
    }
    
    printf("Fechando socket\n");
    close(sock);
    return 0;
}
