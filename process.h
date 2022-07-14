#ifndef PROCESS_H
#define PROCESS_H

#define MAX_MEMORY 1073741824
#define LOW_PRIO   2000
#define HIGH_PRIO  1000
#define PAGE_SIZE  4

#include <stdio.h>

unsigned int g_clock;
/*
TODO
    * imaginar uma forma de criar memoria de 1gb com separações de 4k //done
    * Semaforo
    * Fazer todas as funções declaradas lá embaixo 
    * Criar um relógio // done
    * 

*/

typedef struct memory{
    unsigned int    pid;
    int32_t         mem[1000];//4kb de inteiros
    struct memory   *next;
    struct memory   *prev;
}memory[248509];//1gb de memoria

typedef enum PCB_STATES{
    CREATED = 0,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
}PCB_STATE;

typedef struct blocoControleProcesso{
    unsigned int pid;
    PCB_STATE states;
    unsigned int quantum;
    char instructions[1000];
    struct blocoControleProcesso *next;
}pcb;

pcb highPriorityList(pcb *process){

}

pcb lowPriorityList(pcb *process){
}

pcb newNode(pcb *process, pcb *new){ //é para add no final?
    if (process == NULL){
        
    }
    pcb *head = process;
    while(process->next!=NULL){
        process = process->next;
    }
    process->next = new;
    return *head;
}

pcb delNode(pcb *process, int pid){
    pcb *head = process;
    pcb *prev;
    while(process->pid!=pid){
        if (process->next==NULL){
            fprintf(stderr,"ERROR: PID DOES NOT EXIST\n");
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

pcb processCreate(pcb *process,int pid){//TA FUDIDO HELP
    pcb *head;
    pcb *processNew = malloc(sizeof(pcb));
    processNew->pid =       pid;
    processNew->states =    CREATED;
    processNew->quantum =   0;
    processNew->next =      NULL;
    if (process == NULL){
        return *processNew; // now processNew is head!
    }
    *head = newNode(process,processNew);
    return *head;

}

void processInterrupt(); //Final de quantum time
void processFinish();
void semaphoreP();
void semaphoreV();
void memLoadReq(pcb *process, int pid);
void memLoadFinish();


#endif