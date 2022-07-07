#include <stdlib.h>
#include <pthread.h>
#include "process.h"

void menu();

int main (void){
    printf("Sizeof pcb:%ld",sizeof(pcb));
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}