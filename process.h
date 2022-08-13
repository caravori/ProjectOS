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
#include <semaphore.h>

unsigned int g_clock;
int g_memory;

sem_t semaphore;
sem_t round_sem;
pthread_mutex_t mutexBuffer;
pcb *head;

/*
TODO
    * imaginar uma forma de criar memoria de 1gb com separações de 4k //done
    * Semaforo
    * Fazer todas as funções declaradas lá embaixo 
    * Criar um relógio // done
    * 
    * IDEIA PARA O RR, um algoritmo padrao que recebe a lista. MAS a gente usa duas threads? uma pra baixa prioridade e outra pra alta prioridade e quisifoda HDAUHSUHASD pode ser que de 
    * com isso a lista de maior prioridade roda 2x e a de menor uma vez so, faz sentido? 

*/

typedef struct memory{
    int             pid;
    int32_t         mem[1000];//4kb de inteiros
}memoryType;//1gb de memoria

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

pcb *newNode(pcb *process, pcb *novo){ //é para add no final?
    pcb *head = process;
    while(process->next!=NULL){
        printf("%d - ",process->pid);
        process = process->next;
    }
    process->next = novo;
    return head;
}

pcb *processFinish(pcb *process, int pid){
    pcb *head = process;
    pcb *prev = process;
    while(process->pid!=pid){
        if (process->next==NULL){
            fprintf(stderr,"ERROR: PID DOES NOT EXIST\n");
            return head;
        }
        prev = process;
        process = process->next;
    }
    if (process->next==NULL){
        //prev->next = NULL;
        printf("FREE AT PROCESS NULL %d\n",process->pid);
        free(process);
        return prev;
    }
    else{
        prev->next = process->next;
        printf("FREE AT PROCESS %d\n",process->pid);
        free(process);
        return prev;
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


//
void memLoadFinish(int pid, memoryType *memoryTotal){
    //clean memoryTotal of pid 
    int i = 0;
    while(memoryTotal[i].pid!=pid){
        i++;
    }
    while(memoryTotal[i].pid==pid){
        memoryTotal[i].pid = 0;
        i++;
    }
}

pcb *memLoadReq(pcb *process, memoryType *memoryTotal,int pid){
    pcb *head = process;
    int num_of_mBlocks = (process->memory/4);
    while (num_of_mBlocks!=0){
        memoryTotal[g_memory].pid = pid;
        g_memory++;
        num_of_mBlocks--;
    }
    return head;
}

pcb *processInterrupt(pcb *process){
    if(process->next==NULL){
        return process;
    }
    pcb *aux = process->next;
    process->next = NULL;
    process->states = BLOCKED;
    printf("Process PID %d BLOCKED\n",process->pid); 
    aux = newNode(aux,process);
    return aux;
}


void round_robin(){ //por hora o round robin só roda exec! Sera implementado na próxima etapa!
    // tamanho maximo de tempo = 1000 ou 2000 (depende da prioridade)
    //semaforo
    sem_wait(&round_sem);
    process->states = RUNNING;
    int arrival_time = g_clock;
    pcb *head = process;
    printf("Arrival_TIME %d \n", arrival_time);
    //pegue instruções ate chegar a 1000 ou 2000
    switch (process->isHigh)
    {
    case true:
        if (process->quantum>1000){
            g_clock +=1000;
            process->quantum -=1000 ;
            printf("PROCESS PID %d -1000 STATE RUNNING\n",process->pid);
            //goto final da lista
            head = processInterrupt(process);

        }
        else{
            g_clock += process->quantum;
            process->quantum = 0;
            printf("PROCESS PID %d FINISH\n",process->pid);
            if(process->next!=NULL){
                head = process->next;
            }
            else if(process->next==NULL){
                head = NULL;
            }
            processFinish(process,process->pid);
        }
        break;
    
    case false:
        if (process->quantum>2000){
            g_clock +=2000;
            process->quantum = process->quantum-2000;
            printf("PROCESS PID %d -2000 STATE RUNNING\n",process->pid);
            //goto final da lista 
            head = processInterrupt(process);

        }
        else{
            g_clock += process->quantum;
            if(process->next!=NULL){
                head = process->next;
            }
            else if(process->next==NULL){
                head = NULL;
            }
            //process finish
            printf("PROCESS PID %d FINISH\n",process->pid);
            processFinish(process,process->pid);
        }
        break;
    }
    sem_post(&round_sem);
    return head;
    
}



void semaphoreP(int mutex, pcb *process){

    if(mutex > 0){
        mutex--;
        pthread_mutex_unlock(&mutexBuffer); 
        //unlocked
    }else {
        
        pthread_mutex_lock(&mutexBuffer);
        process->states = BLOCKED;
        //lock;
        
    }
}

void semaphoreV(int mutex, pcb *process){
    
    if(process->states == BLOCKED){
        mutex++;
        
    }
    
} 


pcb *processCreate(int pid,int quantum, bool isHigh, int tamanho){

  pcb *process;
  process = malloc(sizeof(pcb));

  if(process == NULL){
    fprintf(stderr,"ERRO: FALHA NA ALOCACAO DE PROCESSO");
    exit(1);
  }
    process->pid = pid;//pelo arquivo
    process->next = NULL;
    process->quantum = quantum;
    process->states = CREATED;
    process->isHigh = isHigh;//pelo arquivo
    process->memory = tamanho;
    //process->instructions = //precisa do arquivo
  return process;
}

//execute process
pcb *processExec(pcb *process){
    process->states = RUNNING;
    
    return process;
}

//DiskRequest
void diskRequest(pcb *process){

}

#endif