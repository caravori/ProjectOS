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
    * IDEIA PARA O RR, um algoritmo padrao que recebe a lista. MAS a gente usa duas threads? uma pra baixa prioridade e outra pra alta prioridade e quisifoda HDAUHSUHASD pode ser que de 
    * com isso a lista de maior prioridade roda 2x e a de menor uma vez so, faz sentido? 

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


void round_robinho(pcb *process){
    // tamanho maximo de tempo = 1000 ou 2000 (depende da prioridade)
    //semaforo
    process->states = RUNNING;
    int arrival_time = g_clock;
    //pegue instruções ate chegar a 1000 ou 2000
    switch (process->isHigh)
    {
    case true:
        if (process->quantum>=1000){
            g_clock +=1000;
            process->quantum = process->quantum-1000;
            //goto final da lista
            processInterrupt(process);

    }
        break;
    
    case false:
        if (process->quantum>=2000){
            g_clock +=2000;
            process->quantum = process->quantum-2000;
            //goto final da lista 
            processInterrupt(process)

        }
        break;
    }
    
}

void round_robin(pcb *process_list){
    //Round Robin deixa um processo executar por um quantum time
    //depois, passa para o próximo processo

    //Quantum Time: tempo que cada processo fica com a cpu

    //Burst time: tempo pro processor ser finalizado

    //Turnaround Time: diferença entre o tempo para ser completado
    //(Burst Time) e o tempo que surgiu (Arrival Time)

    //Waiting Time: é a diferença entre Turnaround Time e BurstTime

    //Arrival Time: quando, na ordem, o processo surgiu

    //Completion Time: é o tempo em que o processo está completo

    //Remain Time: é o tempo que resta para um processo terminar

    //(OBS: não o tempo que leva, mas sim quando, na marcação de tempo)

    int i, size, *burst_time, *turnaround_time, wait_time, remain, 
    *arrival_time, *completion_time, *remain_time, flag = 0;

    size = size_queue(process_list);
    remain = size;

    burst_time = malloc(sizeof(int) * size);
    turnaround_time = malloc(sizeof(int) * size);
    arrival_time = malloc(sizeof(int) * size);
    completion_time = malloc(sizeof(int) * size);
    remain_time = malloc(sizeof(int) * size);

    while(process_list->next != NULL){
        //Como faço para o processo executar?
        for(g_clock = 0; i = 0; remain != 0){
            if(remain_time[i] <= process_list->quantum && remain_time[i] > 0){
                
                g_clock += remain_time[i];
                remain_time[i] = 0;
                flag = 1;

            }
            else if(remain_time[i] > 0){

                remain_time[i] -= process_list->quantum;
                g_clock += process_list->quantum;
            
            }

            if(remain_time[i] == 0 && flag == 1){
                
                remain--;
                printf("P[%d\t|\t%d\t|\t%d\n", i+1, g_clock - arrival_time[i],
                g_clock - arrival_time[i] - burst_time[i]);

                wait_time += g_clock - arrival_time[i] - burst_time[i];
                turnaround_time += g_clock - arrival_time[i];

                flag = 0;
            }

            if(i == size - 1)
                i = 0;
            else if(arrival_time[i+1] <= g_clock)
                i++;
            else
                i = 0;
        }

        printf("\nMédia de Waiting Time = %f\n", wait_time*1.0/size);
        printf("Média de Turnaround Time = %f", turnaround_time[i]*1.0/size);
    }

}

int size_queue(pcb *process_list){
    int cont = 0;
    while(process_list->next != NULL){
        cont++;
        process_list = process_list->next;
    }
    return cont;
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