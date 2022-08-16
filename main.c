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
char  PROCESS_ARG[100][100];
int  PROCESS_ARG_COUNT;


void criar_processo(memoryType *memoryTotal,char file_name[100]);
void free_memory(pcb *highPriorityList, pcb *lowPriorityList);
void *menu();


int main (int argc, char *argv[]){
    setlocale(LC_ALL, "Portuguese");
    pid = 0;
    g_clock  = 0;
    g_memory = 0;
    trilhaAtual = 0;
    headHigh = NULL;
    headLow = NULL;
    strcpy(PROCESS_ARG[0], "NULL");
    PROCESS_ARG_COUNT = 0;
    sem_init(&semaphore, 0,1);
    sem_init(&round_sem, 0,1);
    pthread_mutex_init(&lock, NULL);

    //transfer arguments to PROCESS_ARG
    for(int i = 1; i < argc; i++){
        strcpy(PROCESS_ARG[i-1], argv[i]);
        PROCESS_ARG_COUNT++;
    }

    
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
    char file_name[100];
    if (strcmp(PROCESS_ARG[0], "NULL") != 0){
        for(i = 0; i < PROCESS_ARG_COUNT; i++){
            criar_processo(memoryTotal,PROCESS_ARG[i]);
        }
    }
    while(op != 2){
        printf("\n---------------------------------------------\n\t\tMenu\n\n1-Criar Processo\n0-Iniciar Simulacao\n2-Sair\nDigite uma das opções acima: ");
        if(scanf("%d",&op)!=1){
            fprintf(stderr,"ERROR AT SCAN");
        }
        switch (op){
        case 1:
            printf("\nDigite o nome do arquivo: ");
            if(scanf("%s",file_name)!=1){
            fprintf(stderr,"ERROR AT SCAN");
            }
            criar_processo(memoryTotal,file_name);
            break;
        case 0: printf("\n\n");
                while (i < g_memory){
                    printf("\tBloco de Memória %d ocupado para o PID %d\n",i,memoryTotal[i].pid);
                    i++;
                }
                printf("\n\n");
                
            break;
        case 2:
            free(memoryTotal);
            exit(0);
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

void criar_processo(memoryType *memoryTotal,char file_name[100]){
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