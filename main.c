#include <stdlib.h>
#include <pthread.h>
#include "process.h"

void menu();

int main (void){
    g_clock = 0;
    //printf("Sizeof pcb:%ld, sizeof memory:%ld",sizeof(pcb),sizeof(memory));
    pcb *aux;
    pcb *highPriorityList = NULL;
    pcb *lowPriorityList = NULL;
    aux = processCreate(1);
    //printf("AQUIOH %d\n",highPriorityList->states);
    if (aux->isHigh==true){
        if(highPriorityList==NULL){
            highPriorityList = aux;
        }
        else{
            newNode(highPriorityList,aux);
        }
    }
    else{
        if(lowPriorityList==NULL){
            lowPriorityList = aux;
        }
        else{
            newNode(lowPriorityList,aux);
        }
    }
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}