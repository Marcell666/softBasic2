
pushq %rbp
movq %rsp, %rbp
/* DEPRECATED
/*movendo parametros para registradores de operação
movl %edi, %r10d
movl %esi, %r10d
movl %edx, %r10d
movl %ecx, %r10d

movl %edi, %r11d
movl %esi, %r11d
movl %edx, %r11d
movl %ecx, %r11d

/*movendo dos registradores de operação para os parametros
movl %r10d, %edi
movl %r10d, %esi 
movl %r10d, %edx
movl %r10d, %ecx

movl %r11d, %edi
movl %r11d, %esi
movl %r11d, %edx
movl %r11d, %ecx
NAO FAZER ATE AQUI
*/

/*movendo parametros para pilha*/
movl %edi, -20(%rbp)
movl %esi, -24(%rbp)
movl %edx, -28(%rbp)
movl %ecx, -32(%rbp)

movl -20(%rbp), %edi
movl -24(%rbp), %esi
movl -28(%rbp), %edx
movl -32(%rbp), %ecx
/*
/*movendo dos registradores de operação para as variaveis*/
movl %r10d, -4(%rbp)
movl %r10d, -8(%rbp)
movl %r10d, -12(%rbp)
movl %r10d, -16(%rbp)


movl %r11d, -4(%rbp)
movl %r11d, -8(%rbp)
movl %r11d, -12(%rbp)
movl %r11d, -16(%rbp)

jne X

/*movendo das variaveis para os registradores de operação*/
movl  -4(%rbp), %r10d
movl  -8(%rbp), %r10d
movl  -12(%rbp), %r10d
movl  -16(%rbp), %r10d

movl  -4(%rbp), %r11d
movl  -8(%rbp), %r11d
movl  -12(%rbp), %r11d
movl  -16(%rbp), %r11d

/*movendo dos parametros para os registradores de operação*/

movl  -20(%rbp), %r10d



imul %r10d, %r11d
addl %r10d, %r11d
subl %r10d, %r11d

f1:
cmp $0, -4(%rbp)
cmp $0, -8(%rbp)
cmp $0, -12(%rbp)
cmp $0, -16(%rbp)
jne f1


 
leave
ret
/*


Eu preciso conhecer os codigos de:
 Passar/de volta parametro para os registradores de operações
 Passar/de volta dos registradores de variavel para os registradores de operações
 Seria bom se os registradores de operações, variaveis, retorno e parametros  fossem todos separados.
Posso usar a pilha para variaveis ao inves de registradores.
Uma operação diz qual função deve ser chamada para aqueles registradores de operações.



   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	89 f8                	mov    %edi,%eax
   6:	83 c0 01             	add    $0x1,%eax
   9:	c9                   	leaveq 
   a:	c3                   	retq  



   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	83 07 01             	addl   $0x1,(%rdi)
   7:	c9                   	leaveq
   8:	c3                   	retq 
*/

potencia
v1 = p3 * p3
v1 = v1 * p1
p2 = p2 - p3
if p2 2
ret



p1 = 2
p3 = 1
p4 = 0
