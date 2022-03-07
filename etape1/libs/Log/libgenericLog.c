#include<stdio.h>
#include<stdlib.h>
#include<dlfct.h>
#include"genericLog.h"

#define STRAT1 "./libstrat1.so"
#define STRAT2 "./libstrat2.so"
#define LIB_MAX_SIZE 100

void * handle;

int loadStrategy(char lib_name[LIB_MAX_SIZE]){
	//some code here
}

int initStrategy(void * f){
	//some code here
}

int logStrategy(void * f){
	//some code here
}
