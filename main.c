#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "process.h"

pthread_t threads[2];

void menu();
pcb criar_processo();
void free_memory(pcb *highPriorityList, pcb *lowPriorityList);

int main (void){
    int op = 100;
    int flagH = 0;
    int flagL = 0;
    g_clock  = 0;
    g_memory = 0;
    sem_init(&semaphore, 0,1);
    sem_init(&round_sem, 0,1);
    pthread_mutex_init(&mutexBuffer, NULL);
    
    memoryType *memoryTotal = malloc(sizeof(memoryType));

    pcb *aux;
    pcb *highPriorityList = NULL;
    pcb *lowPriorityList = NULL;
    pcb *proxima_tarefa
    //com as duas listas criadas, crie duas threads para executar as tarefas,

    while(op! = 0){
        printf("\t\tMenu\n\t1-Criar Processo\t0-Iniciar Simulacao\nop: ");
        switch (op){
        case 1:
            criar_processo(highPriorityList,lowPriorityList);
            break;
        case 0:
            break;
        default:
            printf("\nENTRADA INVÁLIDA !!\n");
            break;
        }
    }

    while(1){
        if(highPriorityList==NULL){
                flagH = 1;
        }
        else if(flagH!=1){
            pthread_create(&threads[0], NULL, round_robin,highPriorityList);
            pthread_join(&thread[0],highPriorityList);

        }
        if(highPriorityList==NULL){
                flagH = 1;
        }
        else if(flagH!=1){
            pthread_create(&threads[1], NULL, round_robin,highPriorityList);
            pthread_join(&thread[1],highPriorityList);

        }
        if(lowPriorityList==NULL){
                flagL = 1;
        }
        else if(flagL!=1){
            pthread_create(&threads[2], NULL, round_robin,lowPriorityList);
            pthread_join(&thread[2],lowPriorityList);

        }
    }

    free_memory(highPriorityList,lowPriorityList);
    free(memoryTotal);
    
    return 0;
}


void free_memory(pcb *highPriorityList, pcb *lowPriorityList){
    pcb *aux = NULL;
    while(highPriorityList->next!=NULL){
        aux = highPriorityList;
        highPriorityList = highPriorityList->next;
        free(aux);
    }
    while(lowPriorityList->next!=NULL){
        aux = lowPriorityList;
        lowPriorityList = lowPriorityList->next;
        free(aux);
    }
    sem_close(&semaphore);
    sem_close(&round_sem);
}

pcb *criar_processo(pcb *highPriorityList,pcb *lowPriorityList, memoryType *memoryTotal){
    int pid,quantum, memory;
    bool isHigh;
    printf("\nPID: ");
    scanf("%d",&pid);
    setbuf(stdin,NULL);
    printf("\nQuantum time:");
    scanf("%d", &quantum);
    setbuf(stdin,NULL);
    printf("\neh alta prioridade? (true or false): ");
    scanf("%d", &isHigh);
    setbuf(stdin,NULL);
    
    do{
    printf("\nQuantidade de memoria em Kb (multiplo de 4): ");
    scanf("%d", &memory);
    setbuf(stdin,NULL);
    }while((memory/4) != 0);
    
    pcb *aux = processCreate(pid,quantum,isHigh);
    
    if (aux->isHigh==true){
        if(highPriorityList==NULL){
            highPriorityList = aux;
        }
        else{
           highPriorityList = newNode(highPriorityList,aux);
        }
        memLoadReq(highPriorityList,memoryTotal,aux->pid);
        return(highPriorityList);
    }
    else{
        if(lowPriorityList==NULL){
            lowPriorityList = aux;
        }
        else{
            lowPriorityList = newNode(lowPriorityList,aux);
        }
        memLoadReq(lowPriorityList,memoryTotal,aux->pid);
        return(lowPriorityList);
    }
}