#include <stdlib.h>
#include <stdio.h>
#include "process.h"

void menu();

int main (void){
    menu();
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}