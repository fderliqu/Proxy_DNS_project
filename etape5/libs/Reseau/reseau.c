#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include"reseau.h"
#include"dns.h"
#include"dns_server.h"
//SERVEUR

int initialisationSocketUDP(char *service){
	struct addrinfo precisions,*resultat,*origine;
	int statut;
	int s;

	/* Construction de la structure adresse */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_DGRAM;
	precisions.ai_flags=AI_PASSIVE;
	statut=getaddrinfo(NULL,service,&precisions,&origine);
	if(statut<0){ perror("initialisationSocketUDP.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  		if(p->ai_family==AF_INET6){ resultat=p; break; }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("initialisationSocketUDP.socket"); exit(EXIT_FAILURE); }

	/* Options utiles */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
 		perror("initialisationServeurUDPgenerique.setsockopt (REUSEADDR)");
  		exit(-1);
	}

	/* Specification de l'adresse de la socket */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0) {perror("initialisationServeurUDP.bind"); exit(-1);}

	/* Liberation de la structure d'informations */
	freeaddrinfo(origine);

	return s;
}

//CLIENT

int messageUDP(char *hote,char *service,unsigned char *message,int taille){
	struct addrinfo precisions,*resultat,*origine;
	int statut;
	int s;

	/* Creation de l'adresse de socket */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_DGRAM;
	statut=getaddrinfo(hote,service,&precisions,&origine);
	if(statut<0){ perror("messageUDPgenerique.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  		if(p->ai_family==AF_INET6){ resultat=p; break; }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("messageUDPgenerique.socket"); exit(EXIT_FAILURE); }

	/* Option sur la socket */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_BROADCAST,&vrai,sizeof(vrai))<0){
  		perror("initialisationServeurUDPgenerique.setsockopt (BROADCAST)");
  		exit(-1);
  	}

	/* Envoi du message */
	int nboctets=sendto(s,message,taille,0,resultat->ai_addr,resultat->ai_addrlen);
	if(nboctets<0){ perror("messageUDPgenerique.sento"); exit(EXIT_FAILURE); }

	/* Liberation de la structure d'informations */
	freeaddrinfo(origine);

	/*Reception reponse*/
	nboctets=recvfrom(s,message,DNS_UDP_MAX_PACKET_SIZE,0,NULL,NULL);

	/* Fermeture de la socket d'envoi */
	close(s);

	return nboctets;
}

int boucleServeurUDP(int s,void (* traitement)(int,unsigned char*,int,void *,int)){

	//boucleServeurUDP(s);
	while(1){
		struct sockaddr_storage adresse;
  		socklen_t taille=sizeof(adresse);
  		unsigned char message[DNS_UDP_MAX_PACKET_SIZE];
		printf("En attente d'un message\n");
  		int nboctets=recvfrom(s,message,DNS_UDP_MAX_PACKET_SIZE,0,(struct sockaddr *)&adresse,&taille);
		traitement(s,message,nboctets,(struct sockaddr *)&adresse,taille);
	}
//	close(s);
	return 0;
}

int send_rep_proxy_dns(int s, unsigned char* message, int taille_message, void * adresse, int taille){
	int nboctets = sendto(s,message,taille_message,0,(struct sockaddr *)adresse,taille);
	if(nboctets<0){
		perror("send_rep_proxy_dns_generique.sendto");return -1;
	}
	return 0;
}

int nomVersAdresse(char *hote,unsigned char *buffer)
{
struct sockaddr_storage adresse; 
struct addrinfo *resultat,*origine;
int i;
int statut=getaddrinfo(hote,NULL,NULL,&origine);
if(statut==EAI_NONAME) return -1;
if(statut<0){ perror("nomVersAdresse.getaddrinfo"); exit(EXIT_FAILURE); }
struct addrinfo *p;
for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  if(p->ai_family==AF_INET6){ resultat=p; break; }
memcpy(&adresse,resultat->ai_addr,resultat->ai_addrlen);
if(adresse.ss_family==PF_INET6){
  struct sockaddr_in6 *s6=(struct sockaddr_in6 *)&adresse;
  for(i=0;i<16;i++) buffer[i] = s6->sin6_addr.s6_addr[i];
  }
if(adresse.ss_family==PF_INET){
  struct sockaddr_in *s4=(struct sockaddr_in *)&adresse;
  for(i=0;i<4;i++) buffer[i] = ((unsigned char *)&s4->sin_addr.s_addr)[i];
  }
return 0;

}
