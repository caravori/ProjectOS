#ifndef PROCESS_H
#define PROCESS_H

#define MAX_MEMORY 1073741824
#define LOW_PRIO   2000
#define HIGH_PRIO  1000
#define PAGE_SIZE  4

#include <stdio.h>
#include <stdlib.h>

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
}memoryType[248509];//1gb de memoria

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
    struct blocoControleProcesso *next;
}pcb;

pcb *processHighPriorityList;
pcb *processLowPriorityList;

pcb newNode(pcb *process, pcb *novo){ //é para add no final?
    pcb *head = process;
    while(process->next!=NULL){
        process = process->next;
    }
    process->next = novo;
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


void processInterrupt(); //Final de quantum time
pcb *processCreate(int pid);
void processFinish();
void semaphoreP();
void semaphoreV();
void memLoadReq();
void memLoadFinish();

pcb *processCreate(int pid){

  pcb *process;
  process = (pcb*)malloc(sizeof(pcb));

  if(process == NULL){
    printf("falha na alocacao de processo");
    exit(1);
  }
  process->pid = pid;
  process->next = NULL;
  process->quantum = 0;

  newNode(processHighPriorityList, process);

  return process;
}

#endif