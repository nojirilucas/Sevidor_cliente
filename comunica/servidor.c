#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define SIZE 10240

void write_file(int sockfd){
    int n; 
    FILE *fp;
    char *filename = "servidorRecebe.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    if(fp==NULL)
    {
        perror("ERRO NA CRIACAO DO ARQUIVO...\n");
        exit(1);
    }
    while(1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if(n<=0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
    
}


int main()
{
	int sockfd, connfd, len, new_sock;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);//verificação e criaçao do socket
	if (sockfd == -1) {
		printf("FALHA AO CRIAR SOCKET...\n");
		exit(0);
	}
	else
		printf("SOCKET CRIADO COM SUCESSO...\n");
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("FALHA NO SOCKET BIND...\n");
		exit(0);
	}
	else
		printf("SUCESSO...\n");

	if ((listen(sockfd, 5)) != 0) {
		printf("FALHA NA ESCUTA...\n");
		exit(0);
	}
	else
		printf("SERVIDOR ESCUTANDO...\n");
	len = sizeof(cli);

	//aceita o pacote do cliente
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("ERRO...\n");
		exit(0);
	}
	else
		printf("CLIENTE ACEITO...\n");


	write_file(new_sock);
    printf("DADOS ESCRITOS NO ARQUIVO...\n ");
	
	//termina conexao
	close(sockfd);
}
