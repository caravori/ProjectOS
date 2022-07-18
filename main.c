#include <stdlib.h>
#include <pthread.h>
#include "process.h"
#include <semaphore.h>
#define QUANTUM_TIME_HIGH 1000

sem_t semaphore;


void menu();

int main (void){
    int counter = 0;
    g_clock  = 0;
    g_memory = 0;
    memoryType *memoryTotal = malloc(sizeof(memoryType));
    sem_init(&semaphore, 0,1);
    //printf("Sizeof pcb:%ld, sizeof memory:%ld",sizeof(pcb),sizeof(memory));
    pcb *aux;
    pcb *highPriorityList = NULL;
    pcb *lowPriorityList = NULL;
    pcb *aux_rr;
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

    aux_rr = highPriorityList;

    //round_robin para a highPriorityList, ai torna isso uma função e manda uma thread executa-la, a msm coisa para a low, e fazer um esquema de jogar um processo de uma lista pra outra e aumentar o counter 
    while(aux_rr != NULL){
        if(aux_rr->quantum > 0)counter++;
        aux_rr = aux_rr->next;
    }
    aux_rr = highPriorityList;  
    while(counter > 0){
        
        if(aux_rr->quantum < QUANTUM_TIME_HIGH && aux_rr->quantum > 0){
            printf("processo %s finalizado", aux_rr->name);
            aux_rr->quantum = 0;
            counter--;
        }
        else if(aux_rr->quantum > 0){
            printf("processo %s com remaning time: %d",aux_rr->name, aux_rr->quantum);
            aux_rr->quantum = aux_rr->quantum - QUANTUM_TIME_HIGH;
            printf("processo %s com remaning time apos execucao: %d",aux_rr->name, aux_rr->quantum);
        
        }
        if(aux_rr->next == NULL)aux_rr = highPriorityList; //volta pro inicio, tornando o rr circular
        else{
            aux_rr = aux_rr->next;
        }
    }

    free(highPriorityList);
    free(memoryTotal);
    sem_close(&semaphore);
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}