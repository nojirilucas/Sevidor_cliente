#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define SIZE 10240

long getIORead()
{
    FILE * j = fopen ("/proc/self/io", "r");
    if (j == NULL)
        return -1;

    char buf[256];
    while (fgets(buf, 250, j) != NULL)
        if (strstr(buf, "read_bytes: ") != NULL)
        {
            fclose(j);
            return atol(strchr(buf, ' ') + 1);
        }
    return -1;
}

float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(sockfd, data, sizeof(data), 0)== -1)
        {
            perror("ERRO AO ENVIAR DADOS");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main(){

	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
    struct timeval start;
    struct timeval end;
	FILE *fp;
    char *filename = "file6.txt";
	unsigned long before, after;
    before = getIORead();
    
    gettimeofday(&start, NULL);
    
	//criaçao e verificacao do socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("FALHA AO CRIAR SOCKET...\n");
		exit(0);
	}
	else
		printf("SOCKET CRIADO COM SUCESSO...\n");
		bzero(&servaddr, sizeof(servaddr));

	//define ip e o port
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	//conexao do socket cliente ao socket servidor
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("ERRO AO CONECTAR NO SERVIDOR...\n");
		exit(0);
	}
	else
		printf("CONECTADO AO SERVIDOR...\n");

	fp = fopen(filename, "r");
     if(fp == NULL){
        perror("ERRO NA LEITURA DO ARQUIVO...\n");
        exit(1);
    }
    send_file(fp,sockfd);
    printf("ARQUIVO ENVIADO COM SUCESSO...\n");


	//termina a conexao
	close(sockfd);

	after = getIORead();
    gettimeofday(&end, NULL);

	printf("Bytes read from disk: %d\n", (int) (after - before));
    printf("time spent: %0.8f sec\n", time_diff(&start, &end));
    exit(EXIT_SUCCESS);

}
