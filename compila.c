#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compila.h"

struct desvio{
	int de, para;
};

static void error (const char *msg, int line) {
	fprintf(stderr, "erro %s na linha %d\n", msg, line);
	exit(EXIT_FAILURE);
}

/*
Operações sempre entre os mesmos registradores
caso um operando nao estiver em um registrador, ele é levado para aquele registrador.
as variaveis v1 v2 v3 e v4 serão registradores conhecidos
Eu preciso escolher os registradores de:
 operações
 e variaveis
Eu preciso conhecer os codigos de:
 Passar parametro para os registradores de operações
 Passar dos registradores de operações para os registradores de variavel
 Passar dos registradores de operações para os parametros
 Seria bom se os registradores de operações, variaveis, retorno e parametros  fossem todos separados.
Posso usar a pilha para variaveis ao inves de registradores.
Uma operação diz qual função deve ser chamada para aqueles registradores de operações.


Variaveis serão colocadas na pilha
v4 -16
v3 -12
v2 -8
v1 -4

o registradores de operações serão
r10 e r11


Passando tanto as variaveis quanto os parametros para a pilha, vou precisar de menos codigo assembly diferente para fazer as operações.
Para passar os parametros para/(de volta da) a pilha, um codigo será usado serão. Para passar a pilha para/(de volta) dos registradores, um codigo será usado. Mas nos dois caso, existe um padrão, os codigos serão deduzidos a partir dele.
Os codigos de parametro-pilha codigos são:
   4:   89 7d ec             	mov    %edi,-0x14(%rbp)
   7:	89 75 e8             	mov    %esi,-0x18(%rbp)
   a:	89 55 e4             	mov    %edx,-0x1c(%rbp)
   d:	89 4d e0             	mov    %ecx,-0x20(%rbp)
  10:	8b 7d ec             	mov    -0x14(%rbp),%edi
  13:	8b 75 e8             	mov    -0x18(%rbp),%esi
  16:	8b 55 e4             	mov    -0x1c(%rbp),%edx
  19:	8b 4d e0             	mov    -0x20(%rbp),%ecx
Note que 89 é o sentido parametro pilha
e 8b é o sentido pilha parametro
os pares seguintes serão sempre os mesmos
então é preciso saber
89 -> parametro pilha
8b -> pilha parametro
7d ec - parametro 1
75 e8 - parametro 2
55 e4 - parametro 3
4d e0 - parametro 4

44 -> variavel
(por alguma razao mov para r10 e r11 tem 44 no inicio)
89 -> variavel pilha
8b -> pilha variavel
55 -> r10
5d -> r11
XX -> indexação de rbp

*/

void insereRetorno(unsigned char func[], int *index){
	unsigned char v1ParaEax[]={0x8b, 0x45, 0xfc};
	int i;
	for(i=0;i<3;i++)
		func[*index+i] = v1ParaEax[i];
	*index+=3;
	func[*index] = 0xc9;//leave
	func[*index+1] = 0xc3;//ret
	*index+=2;
}

void insereAtribuicao(unsigned char func[], int *index, char var0, int idx0, char var1, int idx1, char op, char var2, int idx2){
	unsigned char r10 = 0x55;
	unsigned char r11 = 0x5d;
	unsigned char mover = 0x44;
	unsigned char paraPilha = 0x89;
	unsigned char daPilha = 0x8b;
	
	if(var0=='p') idx0+=4;
	if(var1=='p') idx1+=4;
	if(var2=='p') idx2+=4;
	idx0*=-4;
	idx1*=-4;
	idx2*=-4;

	//coloco primeiro operando em r11
	func[*index] = mover;
	func[*index+1] = daPilha;
	func[*index+2] = r11;//para r11
	func[*index+3] = idx1;
	*index+=4;

	//coloco segundo operando em r10
	func[*index] = mover;
	func[*index+1] = daPilha;
	func[*index+2] = r10;//para r10
	func[*index+3] = idx2;
	*index+=4;

	if(op=='*'){ //somo subtraio ou multiplico r10 com r11
		func[*index] = 0x45;
		func[*index+1] = 0x0f;
		func[*index+2] = 0xaf;
		func[*index+3] = 0xda;
		*index+=4;
	}
	else {
		func[*index] = 0x45;
		func[*index+1] = op=='+'?0x01:0x29;
		func[*index+2] = 0xd3;
		*index+=3;
	}
	
	//coloco r11 em v1
	func[*index] = mover;
	func[*index+1] = paraPilha;
	func[*index+2] = r11;//de r11
	func[*index+3] = idx0;
	*index+=4;
}


void insereDesvio(unsigned char func[], int *index, int qtdLinha, int *linha, char var0, int idx0, int num){

/*

Esta faltando uma coisa, se o desvio for para frente, vai dar erro.

Para ir para frente preciso guardar em algum lugar todas as vezes que houve um desvio para frente. E a cada linha que avançar, checo se alguem quer avançar para aquela linha, se sim, volto naquela instrução e preencho o que falta (desvio em relaçao a posiçao atual).

*/


//	unsigned char comparaComZero[]={0x83, 0x7d};
//	unsigned char pulaSeDiferente[]={0x0f, 0x85};
	int end = 0;
	
	if(var0=='p') idx0+=4;
	idx0*=-4;
	
	//insere compara com zero
	func[*index] = 0x83;
	func[*index+1] = 0x7d;

	//insere idx0 
	func[*index+2] = idx0;
	//0x00
	func[*index+3] = 0x00;

	//insere pulaSeDiferente
	func[*index+4] = 0x0f;
	func[*index+5] = 0x85;
	*index+=6;

	func[*index] = 00;
	func[*index+1] = 00;
	func[*index+2] = 00;
	func[*index+3] = 00;
	*index += 4;
	/*
	int i;
	for(i=0;i<5;i++)
		func[*index+i] = somaUm[i];
  *index+=5;
  */
}


void corrigeDesvio(unsigned char func[]){

	int i;
	for(i=0)

	if(var0=='p') idx0+=4;
	idx0*=-4;
	
	//insere compara com zero
	func[*index] = 0x83;
	func[*index+1] = 0x7d;

	//insere idx0 
	func[*index+2] = idx0;
	//0x00
	func[*index+3] = 0x00;

	//insere pulaSeDiferente
	func[*index+4] = 0x0f;
	func[*index+5] = 0x85;
	*index+=6;


	end = linha[num-1]-(*index+4);
	func[*index] = end & 0xFF;
	func[*index+1] = (end>> 1*8) & 0xFF;
	func[*index+2] = (end>> 2*8) & 0xFF;
	func[*index+3] = (end>> 3*8) & 0xFF;
	*index += 4;
	/*
	int i;
	for(i=0;i<5;i++)
		func[*index+i] = somaUm[i];
  *index+=5;
  */
}

/*

d    b
1101 1011
0010 0101




*/

void init(unsigned char func[], int *index){
	unsigned char regAtiv[]={0x55, 0x48, 0x89, 0xe5};
	unsigned char paramParaPilha[]={0x89, 0x7d, 0xec, 0x89, 0x75, 0xe8, 0x89, 0x55, 0xe4, 0x89, 0x4d, 0xe0};
	int i;
	for(i=0;i<4;i++)
		func[*index+i] = regAtiv[i];
	*index+=4;
	
	for(i=0;i<12;i++)
			func[*index+i] = paramParaPilha[i];

	*index+=12;
}


void end(unsigned char func[], int *index){
	unsigned char v[]={0xc9, 0xc3};
	int i;
	for(i=0;i<2;i++)
		func[*index+i] = v[i];
	*index+=2;
}


#define N 1024
#define MAX_LINHAS 50

//A cada linha que avanço na leitura do codigo, guardo aqui aonde vou começar a preencher o vetor que guarda o codigo de maquina.
//Usando esse vetor posso implementar o desvio
static int linha[MAX_LINHAS];
int qtdLinha = 1;
static Desvio desvio[MAX_LINHAS];
int qtdDesvio = 0;

funcp compila(FILE *f){

	int i;
	int  c;
	//Queremos adicionar codigo no final do vetor
	int index=0;
	//quanto espaço devemos alocar?
	unsigned char *func = (unsigned char*) malloc(N*sizeof(unsigned char));
	//Esse vetor guarda, para cada linha do codigo fonte, que comando equivale no codigo de maquina.



	init(func, &index);

	while ((c = fgetc(f)) != EOF) {
	    switch (c) {
			case 'r': { // retorno
					char c0;
					if (fscanf(f, "et%c", &c0) != 1)
						error("comando invalido", qtdLinha);
					printf("ret\n");
					linha[qtdLinha-1] = index;
					insereRetorno(func, &index);
					break;
			}
			case 'v': 
			case 'p': {  // atribuicao 
					int idx0, idx1, idx2;
					char var0 = c, var1, var2, op;
					if (fscanf(f, "%d = %c%d %c %c%d", &idx0, &var1, &idx1,
							&op, &var2, &idx2) != 6)
							error("comando invalido", qtdLinha);
						printf("%c%d = %c%d %c %c%d\n",
						var0, idx0, var1, idx1, op, var2, idx2);
						linha[qtdLinha-1] = index;
						insereAtribuicao(func, &index, var0, idx0, var1, idx1, op, var2, idx2);
					break;
			}
			case 'i': { // desvio 
					char var0;
					int idx0, num;
					if (fscanf(f, "f %c%d %d", &var0, &idx0, &num) != 3)
							error("comando invalido", qtdLinha);
						printf("if %c%d %d\n", var0, idx0, num);
						linha[qtdLinha-1] = index;
						insereDesvio(func, &index, qtdLinha, linha, var0, idx0, num);
					break;
			}
			default: error("comando desconhecido", qtdLinha);
	    }
	    qtdLinha ++;
	    fscanf(f, " ");
	}
	corrigeDesvio(func);
	for (i=0;i<index;i++)
		printf("%d:%x ", i, func[i]);
	printf("\n");
	//end(func, &index);
	return (funcp)func;
}

/*
0x83, 0x07, 0x01, 0xc3
0x89, 0xf8, 0x83, 0xc0, 0x01, 0xc3


55
48 89 e5

89 7d ec
89 75 e8
89 55 e4
89 4d e0

44 8b 55 ec
44 8b 5d e8
45 1 d3
8b 45 fc

c9 c3




*/
