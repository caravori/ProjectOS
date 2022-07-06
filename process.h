#ifndef PROCESS_H
#define PROCESS_H
#include "clock.h"

#define MAX_MEMORY 1073741824
#define LOW_PRIO   2000
#define HIGH_PRIO  1000
#define PAGE_SIZE  4

typedef struct blocoControleProcesso{
    int pointer;
    char procesState[32];
}


void processInterrupt(){ //Final de quantum time

}   

void processCreate();
void processFinish();
void semaphoreP();
void semaphoreV();
void memLoadReq();
void memLoadFinish();


#endif