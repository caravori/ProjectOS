#include <stdlib.h>
#include <pthread.h>
#include "process.h"

void menu();

int main (void){
    bcp *nsei = malloc(sizeof(bcp));
    bcp *novo = malloc(sizeof(bcp));
    novo->pid = 1;
    *nsei = newNode(nsei,novo);
    *nsei = delNode(nsei,1);
    menu();
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}