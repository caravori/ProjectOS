#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <stdio.h>
#include <string.h>

void startProcess(FILE *fp);

typedef enum instruction{
    exec = 0,
    read,
    write,
    print,
    I_semaphoreP,
    I_semaphoreV,
}inst;

typedef struct instructionBlock{
    inst instructionR;
    int  timeK;
}IB;

typedef struct pseudoProcessBlock{
    char name[20];
    int  identifier;
    int  priority;
    int  segmentSize;
    char semaphore[5][10];
    IB   instructionB[1000];
}PPB;



void openFile(char arch[30]){
    FILE *fp;
    fp = fopen(arch,"r");
    startProcess(fp);
}

void startProcess(FILE *fp){
    int i = 0;
    PPB *novoProcesso = malloc(sizeof(PPB));
    fscanf(fp,"%s\n",novoProcesso->name);
    fscanf(fp,"%d\n",&novoProcesso->identifier);
    fscanf(fp,"%d\n",&novoProcesso->priority);
    fscanf(fp,"%d\n",&novoProcesso->segmentSize);
    do{
        fscanf(fp,"%s ",novoProcesso->semaphore[i]);
        i++;
    }while(!strcmp("%s\n",novoProcesso->semaphore[i-1]));
    i = 0;
    char aux[6];
    while (!feof(fp)){
        fscanf(fp,"%s ",aux);
        if (strcmp(aux,"exec")){
            novoProcesso->instructionB[i].instructionR = 0;
        }
        else if (strcmp(aux,"read")){
            novoProcesso->instructionB[i].instructionR = 1;
        }
        else if (strcmp(aux,"write")){
            novoProcesso->instructionB[i].instructionR = 2;
        }
        else if (strcmp(aux,"print")){
            novoProcesso->instructionB[i].instructionR = 3;
        }
        
    }
       
}




#endif