#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
    
#define BUFLEN 512
#define NPACK 10
#define PORT 9930
   

#define SRV_IP "127.0.0.1"

int verifica (char *string){
	int res;
	res= strcmp(string, "rsv");

	return res;
	
}


void diep(char *s)
      {
        perror(s);
        exit(1);
      }

    /* diep(), #includes and #defines like in the server */
    
int main(int argc, char* const argv[])
{
	struct sockaddr_in si_other;
        int ver, s, i, slen=sizeof(si_other);
        char buf[BUFLEN];

	if (argc <3){
		puts("   Este programa cria um cliente que se comunica"); 
		puts("   a um servidor através dos dados especificados");
		puts("   pelo usuario. Para permitir que o cliente comunique-se");
		puts("   com este servidor, o servidor deve ser");
		puts("   executado inicialmente em um terminal");
		puts("   e o cliente devera ser executado em outra");
		puts("   janela ou em outra aba do terminal.");
		puts("   O cliente deve passar para o servidor 2 parâmetros: a operação (rsv ou rtt) e o endereço.");
		puts("   Caso o cliente envie rsv este deve colocar o endereço da web. Caso contrário, deve escrever rtt sem ");
		puts("   nenhum outro parâmetro.");
		puts("   Modo de Uso:");
		printf("      %s <tipo_do_socket> <endereço>\n", argv[0]);
		printf("   Exemplo: %s rsv www.google.com \n", argv[0]);
		printf("   Exemplo: %s rtt\n", argv[0]);
		exit(1);
	}
	
	ver=verifica(argv[1]);
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//Caso rsv
	if(ver == 0){
		if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
          		diep("socket");
    
        	memset((char *) &si_other, 0, sizeof(si_other));
        	si_other.sin_family = AF_INET;
       		si_other.sin_port = htons(PORT);

        	if (inet_aton(SRV_IP,&si_other.sin_addr)==0) {
          	fprintf(stderr, "inet_aton() failed\n");
          	exit(1);
        	}
    
        
          	sprintf(buf,"%s",argv[2]);
         	if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen)==-1)
           		diep("sendto()");

		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)==-1)
            		diep("recvfrom()");
	
		printf("%s\n", buf);
    
        	close(s);

        }

		
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//Caso rtt
	else{
		printf("Voce digitou rtt\n");


    
        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
          diep("socket");
    
        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(PORT);
        if (inet_aton(SRV_IP,&si_other.sin_addr)==0) {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
        }
    
        for (i=0; i<NPACK; i++) {
          printf("Sending packet %d\n", i);
          sprintf(buf, "This is packet %d\n", i);
          if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen)==-1)
            diep("sendto()");
        }
	
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)==-1)
            diep("recvfrom()");
	
	printf("Mensagem recebida do servidor: %s\n", buf);
    
        close(s);
     
      }
	return 0;
}

