#include <stdlib.h>
#include <pthread.h>
#include "process.h"

void menu();

int main (void){
    g_clock = 0;
    pcb *new = malloc (sizeof(pcb));
    //printf("Sizeof pcb:%ld, sizeof memory:%ld",sizeof(pcb),sizeof(memory));
    pcb head = processCreate(new,1); 
    delNode(&head,1);   
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}