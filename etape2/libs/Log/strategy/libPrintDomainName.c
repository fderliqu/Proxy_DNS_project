//stratégie qui affiche tous les noms de domaines dans un paquet de type query

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../log_dns.h"

/*int initLog(void * f){
	return 0;
}
*/

int addLog(logMsg_t * paquet){
	unsigned char* octet = (unsigned char *)paquet->msg;
	octet += 2; //On veut l'octet comprenant l'opcode
	#ifdef DEBUG
	printf("opcode : %x\n", *octet);
	#endif

	#ifdef DEBUG
	printf("test : %x\n", (*octet & 0x78));
	#endif
	if ((*octet & 0x78) == 0x00)  //on teste si opcode == 0
	{
		octet += 11; //on pointe sur le premier octet donnant le nom de domaine du QNAME
		#ifdef DEBUG
		printf("first element in QNAME :%x\n", *octet);
		#endif
		while (*octet != 0x00)
		{
			printf("%c", *octet);
			if (*octet < 32) printf("."); //si code ascii système on a remarqué que c'était en réalité un point
			octet ++;
		}
		printf("\n");
	}
	
	return 0;
}
