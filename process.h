#ifndef PROCESS_H
#define PROCESS_H
#include "clock.h"

#define MAX_MEMORY 1073741824
#define LOW_PRIO   2000
#define HIGH_PRIO  1000
#define PAGE_SIZE  4

typedef struct blocoControleProcesso{
    int pointer;
    char procesState[32];
    int prioridade;
    //char register[10][3];
    int limMemory;
    int pid;
    struct blocoControleProcesso *next;
}bcp;

bcp highPriorityList(bcp *process){
    bcp *head = process;
    while (process->next!=NULL){
        process = process->next;
    }
    return *head;
}

bcp lowPriorityList(){

}



void processInterrupt(); //Final de quantum time
void processCreate();
void processFinish();
void semaphoreP();
void semaphoreV();
void memLoadReq();
void memLoadFinish();


#endif