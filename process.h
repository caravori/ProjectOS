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

pcb processCreate(){//TA FUDIDO HELP
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
    *arrival_time, *completion_time, *remain_time, time = 0, flag = 0;

    size = size_queue(process_list);
    remain = size;

    burst_time = malloc(sizeof(int) * size);
    turnaround_time = malloc(sizeof(int) * size);
    arrival_time = malloc(sizeof(int) * size);
    completion_time = malloc(sizeof(int) * size);
    remain_time = malloc(sizeof(int) * size);

    while(process_list->next != NULL){
        //Como faço para o processo executar?
        for(time = 0; i = 0; remain != 0){
            if(remain_time[i] <= process_list->quantum && remain_time[i] > 0){
                
                time += remain_time[i];
                remain_time[i] = 0;
                flag = 1;

            }
            else if(remain_time[i] > 0){

                remain_time[i] -= process_list->quantum;
                time += process_list->quantum;
            
            }

            if(remain_time[i] == 0 && flag == 1){
                
                remain--;
                printf("P[%d\t|\t%d\t|\t%d\n", i+1, time - arrival_time[i],
                time - arrival_time[i] - burst_time[i]);

                wait_time += time - arrival_time[i] - burst_time[i];
                turnaround_time += time - arrival_time[i];

                flag = 0;
            }

            if(i == size - 1)
                i = 0;
            else if(arrival_time[i+1] <= time)
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

int size_queue(pcb *process_list);
void processInterrupt(); //Final de quantum time
void processFinish();
void semaphoreP();
void semaphoreV();
void memLoadReq(pcb *process, int pid);
void memLoadFinish();


#endif