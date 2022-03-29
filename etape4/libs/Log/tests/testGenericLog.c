#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"genericLog.h"


void test_loadStrategy(char * name)
{
	printf("Test de loadStrategy pour la stratégie %s : ", name);
	int t = loadStrategy(name);
	assert((t==0) || (t==-1));
	if (t==-1) printf("Echec \nLa stratégie %s n'a pu être lancée, merci de vérifier qu'elle existe, sinon de chosiir \"libDNSinFILE.so\" ou \"libPrintDomainName.so\"\n", name);
	else if (t==0) printf("Succès\n");
	return;
}

void test_initStrategy(char * f)
{
	printf("Test de initStrategy ");
	if (f != NULL) printf("avec en paramètre %s : ",f);
	else printf("sans paramètre : ");
	int t = initStrategy(f);
	assert((t==0) || (t==-1));
	if (t==-1) printf("Echec \nLa stratégie n'a pu être initialisée\n");
	else if (t==0) printf("Succès\n");
	return;
}

void test_logStrategy(logMsg_t * msg)
{
	printf("Test de logStrategy : ");
	int t = logStrategy(msg);
	assert((t==0) || (t==-1));
	if (t==-1) printf("Echec\nUne erreur est survenue");
	else if (t==0) printf("Succès\n");
	return;
}

void test_endStrategy()
{
	printf("Test de endStrategy : ");
	int t = endStrategy();
	assert((t==0) || (t==-1));
	if (t==-1) printf("Echec\nUne erreur est survenue, la stratégie,n'a pas pu être fermée\n");
	else if (t==0) printf("Succès\n");
	return;
}

int main()
{
	printf("\nTEST DE LA LIBRAIRIE GENERICLOG\n");

	test_loadStrategy("libDNSinFILE.so");
	test_initStrategy("output.txt");

	unsigned char chaine[]={0x4c, 0x7b, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,0x00,0x00,0x03,0x72,0x65,0x78,0x04,0x70,0x6c,0x69,0x6c,0x02,0x66,0x72,0x00, 0x00,0x01,0x00,0x01};
	int taille_msg = sizeof(chaine);
	logMsg_t * msg = malloc(sizeof(logMsg_t)-1+taille_msg);
	msg->size=taille_msg;
	memcpy(msg->msg, chaine, taille_msg);

	test_logStrategy(msg);
	test_endStrategy();

	test_loadStrategy("libPrintDomainName.so");
	test_initStrategy(NULL);
	test_endStrategy();

	test_loadStrategy("WrongName.so");
	free(msg);

	return 0;
}