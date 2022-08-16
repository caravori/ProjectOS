#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "process.h"

pthread_t threads[2];

sem_t semaphore;
sem_t round_sem;


void criar_processo(memoryType *memoryTotal);
void free_memory(pcb *highPriorityList, pcb *lowPriorityList);
void *menu();


int main (){
    setlocale(LC_ALL, "Portuguese");
    pid = 0;
    g_clock  = 0;
    g_memory = 0;
    trilhaAtual = 0;
    headHigh = NULL;
    headLow = NULL;
    sem_init(&semaphore, 0,1);
    sem_init(&round_sem, 0,1);
    pthread_mutex_init(&lock, NULL);

    
    //create a thread for the menu
    pthread_create(&threads[1], NULL, &menu, NULL);
    pthread_create(&threads[0], NULL, round_robin, NULL);


    //create a thread for round robbin
    pthread_join(threads[1], NULL);
    pthread_join(threads[0], NULL);
    
    
    return 0;
}

void *menu(void){
    memoryType *memoryTotal = malloc(sizeof(memoryType)*MAX_MEMORY);
    int op = 100, i = 0;
    while(op != 2){
        printf("\n---------------------------------------------\n\t\tMenu\n\n1-Criar Processo\n0-Iniciar Simulacao\n2-Sair\nDigite uma das opções acima: ");
        if(scanf("%d",&op)!=1){
            fprintf(stderr,"ERROR AT SCAN");
        }
        switch (op){
        case 1:
            criar_processo(memoryTotal);
            break;
        case 0: printf("\n\n");
                while (i < g_memory){
                    printf("\tBloco de Memória %d ocupado para o PID %d\n",i,memoryTotal[i].pid);
                    i++;
                }
                printf("\n\n");
                
            break;
        case 2:
            break;
        default:
            printf("\nENTRADA INVÁLIDA !!\n");
            break;
        }
    }
    free(memoryTotal);
    return NULL;
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

void criar_processo(memoryType *memoryTotal){
    char file_name[30];
    //scan file_name
    printf("\nDigite o nome do arquivo: ");
    if(scanf("%s",file_name)!=1){
        fprintf(stderr,"ERROR AT SCAN");
    }
    //open file
    pcb *process = openFile(file_name);
    
    if(process == NULL){
        printf("\nArquivo não encontrado");
        exit(0);
    }

    process = memLoadReq(process,memoryTotal,process->pid);
    if(headHigh==NULL){
        headHigh = process;
    }
    else{
        newNode(headHigh,process);
    }
    pthread_mutex_unlock(&lock);
}


/*pcb *criar_processo(pcb *highPriorityList,pcb *lowPriorityList, memoryType *memoryTotal){
    int quantum, memory;
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
    }while((memory%4&&memory<4) != 0);
    
    pcb *aux = processCreate(pid,quantum,isHigh,memory);
    
    if (aux->isHigh==true){
        if(highPriorityList==NULL){
            highPriorityList = aux;
            printf("\n--Primeiro Nó de alta prioridade CRIADO COM SUCESSO!--\n");
        }
        else{
           highPriorityList = newNode(highPriorityList,aux);
           printf("\n--Nó de alta prioridade adicionado com SUCESSO!--\n");
        }
        highPriorityList = memLoadReq(highPriorityList,memoryTotal,aux->pid);
        return(highPriorityList);
    }
    else{
        if(lowPriorityList==NULL){
            lowPriorityList = aux;
            printf("\n--Primeiro Nó de baixa prioridade CRIADO COM SUCESSO!--\n");
        }
        else{
            lowPriorityList = newNode(lowPriorityList,aux);
            printf("\n--Nó de baixa prioridade adicionado com SUCESSO!--\n");

        }
        lowPriorityList = memLoadReq(lowPriorityList,memoryTotal,aux->pid);
        return(lowPriorityList);
    }
}*/