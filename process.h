#ifndef PROCESS_H
#define PROCESS_H

#define MAX_MEMORY 1073741824
#define LOW_PRIO   2000
#define HIGH_PRIO  1000
#define PAGE_SIZE  4
#include <stdio.h>
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

}

bcp lowPriorityList(bcp *process){
}

bcp newNode(bcp *process, bcp *new){ //é para add no final?
    bcp *head = process;
    while(process->next!=NULL){
        process = process->next;
    }
    process->next = new;
    return *head;
}

bcp delNode(bcp *process, int pid){
    bcp *head = process;
    bcp *prev;
    while(process->pid!=pid){
        if (process->next==NULL){
            fprintf(stderr,"ERROR: PID DO NOT EXIST\n");
            return *head;
        }
        prev = process;
        process = process->next;
    }
    if (process->next==NULL){
        prev->next = NULL;
        free(process);
    }
    else{
        prev->next = process->next;
        free(process);
    }

}

void processInterrupt(); //Final de quantum time
void processCreate();
void processFinish();
void semaphoreP();
void semaphoreV();
void memLoadReq();
void memLoadFinish();


#endif