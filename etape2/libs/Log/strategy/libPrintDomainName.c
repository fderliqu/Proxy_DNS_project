//stratégie qui affiche tous les noms de domaines dans un paquet de type query

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../log_dns.h"

#define GoTo_BYTE2 2
#define GoTo_QNAME 11
#define MASK_OPCODE 0x78 //0b01111000 masque pour tester l'opcode

int initLog(void * f){
	//Cette fonction est inutile pour cette stratégie mais il faut quand même la définir, cette fonction ne fera rien, le code sert juste à eviter le warning dans la compilation
	if(f==NULL)return 0;
	return 0;
}


int addLog(logMsg_t * paquet){
	unsigned char* octet = (unsigned char *)paquet->msg;
	octet += GoTo_BYTE2; //On veut l'octet comprenant l'opcode
	#ifdef DEBUG
	printf("opcode : %x\n", *octet);
	#endif

	#ifdef DEBUG
	printf("test opcode : %x\n", (*octet & MASK_OPCODE));
	#endif
	if ((*octet & MASK_OPCODE) == 0x00)  //on teste si opcode == 0
	{
		octet += GoTo_QNAME; //on pointe sur le premier octet donnant le nom de domaine du QNAME
		#ifdef DEBUG
		printf("first element in QNAME :%x\n", *octet);
		#endif
		while (*octet != 0x00) //RFC1035 : Le QNAME se termine par le caractère NUL de code ascii 0
		{
			printf("%c", *octet);
			if (*octet < 32) printf("."); //si code ascii est une commande de controle de terminal on a remarqué que c'était en réalité un point, pour afficher le point, nous affichons un /'./' au lieu du caractère non imprimable
			octet++; //On passe l'octet suivant
		}
		printf("\n");
	}
	
	return 0;
}
