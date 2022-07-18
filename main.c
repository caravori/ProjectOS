#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "process.h"

pthread_t threads[2];

sem_t semaphore;
sem_t round_sem;
pthread_mutex_t mutexBuffer;

pcb *criar_processo(pcb *highPriorityList,pcb *lowPriorityList, memoryType *memoryTotal);
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
    
    memoryType *memoryTotal = malloc(sizeof(memoryType)*MAX_MEMORY);

    pcb *highPriorityList = NULL;
    pcb *lowPriorityList = NULL;
    //com as duas listas criadas, crie duas threads para executar as tarefas,

    while(op != 0){
        printf("\t\tMenu\n\t1-Criar Processo\t0-Iniciar Simulacao\nop: ");
        if(scanf("%d",&op)!=1){
            fprintf(stderr,"ERROR AT SCAN");
        }
        switch (op){
        case 1:
            criar_processo(highPriorityList,lowPriorityList,memoryTotal);
            break;
        case 0:
            break;
        default:
            printf("\nENTRADA INVÁLIDA !!\n");
            break;
        }
    }

    while(1){
       /* if(highPriorityList==NULL){
                flagH = 1;
        }
        else if(flagH!=1){
            pthread_create(threads[0], NULL, round_robin,highPriorityList);
            pthread_join(threads[0],highPriorityList);

        }
        if(highPriorityList==NULL){
                flagH = 1;
        }
        else if(flagH!=1){
            pthread_create(&threads[1], NULL, &round_robin,&highPriorityList);
            pthread_join(&threads[1],&highPriorityList);

        }
        if(lowPriorityList==NULL){
                flagL = 1;
        }
        else if(flagL!=1){
            pthread_create(&threads[2], NULL, &round_robin,&lowPriorityList);
            pthread_join(&threads[2],&lowPriorityList);

        }*/

         if(highPriorityList==NULL){
            flagH = 1;
        }
        else if(flagH!=1){
            highPriorityList = round_robin(highPriorityList);
        }
        if(highPriorityList==NULL){
                flagH = 1;
        }
        else if(flagH!=1){
            highPriorityList = round_robin(highPriorityList);
        }
        if(lowPriorityList==NULL){
                flagL = 1;
        }
        else if(flagL!=1){
            lowPriorityList = round_robin(lowPriorityList);
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
    int isHigh;
    printf("\nPID: ");
    if (scanf("%d",&pid)!=1){
        fprintf(stderr,"ERROR AT SCAN");
    }
    setbuf(stdin,NULL);
    printf("\nQuantum time:");

    if(scanf("%d", &quantum)!=1){
        fprintf(stderr,"ERROR AT SCAN");
    }
    setbuf(stdin,NULL);
    printf("\neh alta prioridade? (1 or 0): ");
    if (scanf("%d", &isHigh)!=1){
        fprintf(stderr,"ERROR AT SCAN");
    }
    setbuf(stdin,NULL);
    
    do{
    printf("\nQuantidade de memoria em Kb (multiplo de 4): ");
    if (scanf("%d", &memory)!=1){
        fprintf(stderr,"ERROR AT SCAN");
    }
    setbuf(stdin,NULL);
    }while((memory%4) != 0);
    
    pcb *aux = processCreate(pid,quantum,isHigh,memory);
    
    if (aux->isHigh==true){
        if(highPriorityList==NULL){
            highPriorityList = aux;
        }
        else{
           highPriorityList = newNode(highPriorityList,aux);
        }
        highPriorityList = memLoadReq(highPriorityList,memoryTotal,aux->pid);
        return(highPriorityList);
    }
    else{
        if(lowPriorityList==NULL){
            lowPriorityList = aux;
        }
        else{
            lowPriorityList = newNode(lowPriorityList,aux);
        }
        lowPriorityList = memLoadReq(lowPriorityList,memoryTotal,aux->pid);
        return(lowPriorityList);
    }
}