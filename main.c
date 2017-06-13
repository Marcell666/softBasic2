#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "compila.h"

int main(int argc, char *argv[]){
	FILE *myfp;
	funcp func;
	if(argc!=5){
		printf("Argumentos demais(menos) foram usados, este programa aceita necessariamente 4 \n");
		exit(EXIT_FAILURE);
	}
	  if ((myfp = fopen ("programa", "r")) == NULL) {
	    perror ("nao conseguiu abrir arquivo!");
	    exit(1);
	  }
	printf("ola\n");
	func = compila(myfp);
	printf("%d\t%d\t%d\t%d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	printf("mundo\n");
	printf("%d\n", (*func)(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])));
	fclose(myfp);
  return 0;
}

