#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"shmDNS.h"

#ifdef DEBUG
int debug = 1;
#endif
#ifndef DEBUG
int debug = 0;
#endif

void test_tidy_mgr(struct mgr_s * p_mgr_s, char *ligne)
{
	printf("\nTest de tidy_mgr avec ligne : %s\n",ligne);
	tidy_mgr(p_mgr_s, ligne);
	printf("Nom de domaine : %s\n", p_mgr_s->domaine);
	printf("IPV4 : %s\n", p_mgr_s->ipv4);
	printf("IPV6 : %s\n", p_mgr_s->ipv6);
	printf("MX : %s\n", p_mgr_s->mx);
	printf("Fin du test \n");
}

int main(){
	struct mgr_s m, n;
	test_tidy_mgr(&m, "t.vantroys.plil.fr=172.26.145.55,2001:660:4401:6050:5000::5,172.26.145.55");
	test_tidy_mgr(&n, "www.google.com=172.26.145.56,,");
}

