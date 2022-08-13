#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void startProcess(FILE *fp);

typedef enum instruction{
    exec = 0,
    read,
    write,
    print,
    I_semaphore,
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
    char semaphore[5][1];
    IB   instructionB[1000];
}PPB;



void openFile(char arch[30]){
    FILE *fp;
    fp = fopen(arch,"r");
    startProcess(fp);
}

void startProcess(FILE *fp){
    char ch;
    int i = 0;
    PPB *novoProcesso = (PPB *)malloc(sizeof(PPB));
    fscanf(fp,"%s\n",novoProcesso->name);
    fscanf(fp,"%d\n",&novoProcesso->identifier);
    fscanf(fp,"%d\n",&novoProcesso->priority);
    fscanf(fp,"%d\n",&novoProcesso->segmentSize);
    while(ch=fgetc(fp) != '\n'){
        if (isspace(ch)==0){
            novoProcesso->semaphore[i][0]=ch;
            i++;
        }
    }
    i = 0;
    char aux[6];
    fgetc(fp);
    while (!feof(fp)){
        fscanf(fp,"%s ",aux);
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
        fscanf(fp,"%d\n",&novoProcesso->instructionB[i].timeK);       
        i++;    
        //print timek
        printf("%d\n",novoProcesso->instructionB[i-1].instructionR);

    }
       
}


#endif