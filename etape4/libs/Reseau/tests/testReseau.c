#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<assert.h>

#include"reseau.h"
#include"../dns.h"
#include"../dns_server.h"

int testInitSocketUDP(char *service, int test){
	int status = initialisationSocketUDP(service);
	if(test == 0){//Test de bonne initialisation
		printf("Test initialisation d'une socket UDP\n");
		assert(status > 0);
		printf("Success\n");
	}
	else if(test == 1){//Test d'erreur d'initialisation
		printf("Test d'erreur sur l'initialisation d'une socket UDP\n");
		assert(status < 0);
		printf("Success\n");
	}
	return status;

}

int testMessageUDP(char * hote, char * service, unsigned char message[DNS_UDP_MAX_PACKET_SIZE], int taille, int test){
	int status = messageUDP(hote,service,message,taille);
	if(test == 0){//Test de bon lancement du prog
		printf("Test initialisation sur message UDP\n");
		assert(status > 0);
		printf("Success\n");
	}
	else if(test == 1){//Test d'erreur sur le prog
		printf("Test d'erreur sur messageUDP\n");
		assert(status < 0);
		printf("Success\n");
	}
	return status;
}

void testSendRepProxyDNS(int s, unsigned char * message, int taille_message, void * adresse, int taille,int test){
	int status = send_rep_proxy_dns(s,message,taille_message,adresse,taille);
	if(test == 0){//Test de bon lancement du prog
		printf("Test d'envoie de la réponse vers le client initial\n");
		assert(status == 0);
		printf("Success\n");
	}
	else if(test == 1){//Test d'erreur sur le prog
		printf("Test d'erreur sur l'envoie de la réponse vers le client initial\n");
		assert(status != 0);
		printf("Success\n");
	}

}

char GoodPort[256] = "53",BadPort[256] = "666";
char GoodServer[256] = "193.48.57.48",BadServer[256] ="0";


void testProxyDNS(int s, unsigned char * message, int taille_message, void * adresse, int taille){
	printf("Message recu : ");
	unsigned char * octet = message;
	for(int i=0;i<taille_message;i++){
		printf("%02x ",*octet);
		octet++;
	}
	printf("\n");
	int nboctets = testMessageUDP(BadServer,BadPort,message,taille_message,0);
	testSendRepProxyDNS(s,message,nboctets,adresse,taille,0);
}


int main(int argc,char * argv[]){
	assert(argc==1);
	int s = testInitSocketUDP("53",0);
	boucleServeurUDP(s,testProxyDNS);
	return 0;
}





