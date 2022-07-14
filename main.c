#include <stdlib.h>
#include <pthread.h>
#include "process.h"

void menu();

int main (void){
    g_clock = 0;
    pcb *new = malloc (sizeof(pcb));
    pcb *newNew = malloc (sizeof(pcb));
    //printf("Sizeof pcb:%ld, sizeof memory:%ld",sizeof(pcb),sizeof(memory));
    new->pid = 1;
    pcb head = newNode(new,newNew);
    delNode(new,1);   
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}