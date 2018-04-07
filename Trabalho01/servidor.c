#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
    
#define BUFLEN 512
#define NPACK 10
#define PORT 9930
   
void diep(char *s)
{
	perror(s);
       exit(1);
}
    
int main(void)
{
  	struct sockaddr_in si_me, si_other;
	struct hostent *h;

  	int s, i, slen=sizeof(si_other);
   	char buf[BUFLEN];
    
	s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
          diep("socket");
    
        memset((char *) &si_me, 0, sizeof(si_me));
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(PORT);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);
        if ( (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))) <0)
            diep("bind");
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//Pegado o endereço IP e enviando
	
	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)==-1)
            diep("recvfrom()");
	
	h = gethostbyname(buf);
  	if(h==NULL) {
    		printf("Não foi possível encontrar IP.\n");
		
		sprintf(buf,"Rrsv: 1 Não encontrado.");
		if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen)==-1)
           		diep("sendto()");
		
    		exit(1);
  	}

	sprintf(buf,"Rrsv: 0   %s", inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));
	if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen)==-1)
            diep("sendto()");

	
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
            
//        for (i=0; i<NPACK; i++) {
//          if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)==-1)
//            diep("recvfrom()");
//          printf("Received packet from %s:%d\nData: %s\n\n", 
//                 inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
//        }
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
        
//	sprintf(buf,"Mensagem rebida com sucesso.");
//	if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen)==-1)
//            diep("sendto()");
	
	close(s);
        return 0;
     }

