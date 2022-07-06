#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "process.h"

void menu();

int main (void){
    //pthread_t T_clock;
    //int clock_T = pthread_create(&T_clock,NULL,&clock,NULL);
    menu();
    u_int64_t *arr;
    arr = malloc(MAX_MEMORY);
    free(arr);
    //pthread_join(T_clock,NULL);
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}