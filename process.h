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
#include <ctype.h>
#include <string.h>


unsigned int g_clock;
int g_memory;
int pid;

sem_t semaphore;
sem_t round_sem;
pthread_mutex_t mutexBuffer;

//prototypes of functions

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

typedef enum instruction{
    exec = 0,
    read,
    write,
    print,
    I_semaphore,
    end,
}inst;

typedef struct instructionBlock{
    inst instructionR;
    int  timeK;
}IB;

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
    char name[20];
    int  identifier;
    int pid;
    PCB_STATE states;
    bool isHigh;
    bool execIO;
    int quantum;
    int memory;
    char semaphore[5][1];
    IB  instructionB[1000];
    struct blocoControleProcesso *next;
}pcb;

void newNode(pcb *process, pcb *novo);
pcb *processFinish(pcb *process, int pid);
pcb *findProcess(pcb *process, int pid);
void memLoadFinish(int pid, memoryType *memoryTotal);
pcb *memLoadReq(pcb *process, memoryType *memoryTotal,int pid);
void processInterrupt(pcb *process);
void round_robin();
void semaphoreP(int mutex, pcb *process);
void semaphoreV(int mutex, pcb *process);
pcb *processCreate(pcb *NewProcess);
pcb *processExec(pcb *process);
pcb *openFile(char arch[30]);
pcb *startProcess(FILE *fp);

pcb *headHigh;
pcb *headLow;



void newNode(pcb *process, pcb *novo){ //é para add no final?
    while(process->next!=NULL){
        printf("%d - ",process->pid);
        process = process->next;
    }
    process->next = novo;
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

void processInterrupt(pcb *process){
    pcb *aux = process->next;
    process->next = NULL;
    process->states = BLOCKED;
    if (process->isHigh){
        headHigh = aux;
    }
    if (process->isHigh==false){
        headLow = aux;
    }
    if (process->execIO){
        newNode(headLow,process);
    }
    else{
        newNode(headHigh,process);
    }
    printf("Process PID %d BLOCKED\n",process->pid); 
}


void round_robin(){ //por hora o round robin só roda exec! Sera implementado na próxima etapa!
    // tamanho maximo de tempo = 1000 ou 2000 (depende da prioridade)
    //semaforo
    pcb *process;
    if (headHigh==NULL&&headLow==NULL){
        sem_wait(&round_sem);
    }
    if (headHigh!=NULL){
        process = headHigh;
    }
    else{
        process = headLow;
    }
    int arrival_time = g_clock;
    
    printf("Arrival_TIME %d \n", arrival_time);
    process = processExec(process);
    //pegue instruções ate chegar a 1000 ou 2000
    
    switch (process->isHigh)
    {
    case true:
        if (process->quantum>1000){
            g_clock +=1000;
            process->quantum -=1000 ;
            printf("PROCESS PID %d -1000 STATE RUNNING\n",process->pid);
            //goto final da lista
            processInterrupt(process);

        }
        else{
            g_clock += process->quantum;
            process->quantum = 0;
            printf("PROCESS PID %d FINISH\n",process->pid);
            if(process->next!=NULL){
                headHigh = process->next;
            }
            else if(process->next==NULL){
                headHigh = NULL;
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
            processInterrupt(process);

        }
        else{
            g_clock += process->quantum;
            if(process->next!=NULL){
                headLow = process->next;
            }
            else if(process->next==NULL){
                headLow = NULL;
            }
            //process finish
            printf("PROCESS PID %d FINISH\n",process->pid);
            processFinish(process,process->pid);
        }
        break;
    }
    sem_post(&round_sem);
    
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


//execute process
pcb *processExec(pcb *process){
    int i = 0;
    process->states = RUNNING;
    while(process->quantum<1000){
        switch(process->instructionB[i].instructionR){
            case exec:
                break;
            case read:
                break;
            case write:
                break;
            case print:
                break;
            case I_semaphore:
                break;
            case end:
                break;
        }
        i++;

    }
    return process;
}

//DiskRequest
/*void diskRequest(pcb *process){

}*/




pcb *openFile(char arch[30]){
    FILE *fp;
    fp = fopen(arch,"r");
    pcb *process = startProcess(fp);
    fclose(fp);
    return process;
}   

pcb *startProcess(FILE *fp){
    char ch;
    int i = 0;
    pcb *novoProcesso = (pcb *)malloc(sizeof(pcb));
    if(fscanf(fp,"%s\n",novoProcesso->name)==0){
        fprintf(stderr,"ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
    }
    if(fscanf(fp,"%d\n",&novoProcesso->identifier)==0){
        fprintf(stderr,"ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
    }
    if(fscanf(fp,"%d\n",&i)==0){
        fprintf(stderr,"ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
    }
    if(i == 0){
        novoProcesso->isHigh = false;
    }
    else{
        novoProcesso->isHigh = true;
    }
    i = 0;
    if(fscanf(fp,"%d\n",&novoProcesso->memory)==0){
        fprintf(stderr,"ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
    }
    while((ch=fgetc(fp)) != '\n'){
        if (isspace(ch)==0){
            novoProcesso->semaphore[i][0]=ch;
            i++;
        }
    }
    i = 0;
    char aux[6];
    fgetc(fp);
    while (!feof(fp)){
        if(fscanf(fp,"%s ",aux)==0){
            fprintf(stderr,"ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
        }
        if (strcmp(aux,"exec")==0){
            novoProcesso->instructionB[i].instructionR = exec;
        }
        else if (strcmp(aux,"read")==0){
            novoProcesso->instructionB[i].instructionR = read;
        }
        else if (strcmp(aux,"write")==0){
            novoProcesso->instructionB[i].instructionR = write;
        }
        else if (strcmp(aux,"print")==0){
            novoProcesso->instructionB[i].instructionR = print;
        }
        else{
            novoProcesso->instructionB[i].instructionR = I_semaphore;
        }
        if(fscanf(fp,"%d\n",&novoProcesso->instructionB[i].timeK)){
            fprintf(stderr,"ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
        }       
        i++;    
        //print timek

    }
    novoProcesso->next = NULL;
    novoProcesso->instructionB[i].instructionR = end;
    novoProcesso->pid = pid++;
    return novoProcesso;
       
}


#endif