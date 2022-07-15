#ifndef PROCESS_H
#define PROCESS_H

#define MAX_MEMORY 248509
#define LOW_PRIO   2000
#define HIGH_PRIO  1000
#define PAGE_SIZE  4


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

unsigned int g_clock;
unsigned int g_memory;
/*
TODO
    * imaginar uma forma de criar memoria de 1gb com separações de 4k //done
    * Semaforo
    * Fazer todas as funções declaradas lá embaixo 
    * Criar um relógio // done
    * 

*/

typedef struct memory{
    int             pid;
    int32_t         mem[1000];//4kb de inteiros
    struct memory   *next;
    struct memory   *prev;
}memoryType[MAX_MEMORY];//1gb de memoria

typedef enum PCB_STATES{
    CREATED = 0,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
}PCB_STATE;

typedef struct blocoControleProcesso{
    int pid;
    PCB_STATE states;
    bool isHigh;
    int quantum;
    int memory;
    int instructions[1000];
    struct blocoControleProcesso *next;
}pcb;

pcb newNode(pcb *process, pcb *novo){ //é para add no final?
    pcb *head = process;
    while(process->next!=NULL){
        process = process->next;
    }
    process->next = novo;
    return *head;
}

pcb processFinish(pcb *process, int pid){
    pcb *head = process;
    pcb *prev = process;
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
        return *head;
    }
    else{
        prev->next = process->next;
        free(process);
        return *head;
    }

}

pcb *findProcess(pcb *process, int pid){
    while (!(process->pid==pid))
    {
        process = process->next;
        if(process->next==NULL){
            fprintf(stderr,"ERROR AT FINDING PROCESS <WRONG OR MISSING PID>");
            return process;
        }

    }
    return process;
}

void memLoadFinish(){
    //semaforo V(mutex);
}

pcb memLoadReq(pcb *process, memoryType *memoryTotal,int pid){
    pcb *head = process;
    process = findProcess(process,pid);
    int num_of_mBlocks = (process->memory/4);
    // semaforo P(mutex);
    while (!num_of_mBlocks==0){
        memoryTotal[g_memory]->pid = pid;
        g_memory++;
        num_of_mBlocks--;
    }
    memLoadFinish();
    return *head;
}



void processInterrupt(); //Final de quantum time
void semaphoreP();
void semaphoreV();


pcb *processCreate(int pid){

  pcb *process;
  process = malloc(sizeof(pcb));

  if(process == NULL){
    fprintf(stderr,"ERRO: FALHA NA ALOCACAO DE PROCESSO");
    exit(1);
  }
    process->pid = pid;//pelo arquivo
    process->next = NULL;
    process->quantum = 0;
    process->states = CREATED;
    process->isHigh = true;//pelo arquivo
    //process->instructions = //precisa do arquivo
  return process;
}

#endif