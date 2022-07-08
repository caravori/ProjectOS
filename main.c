#include <stdlib.h>
#include <pthread.h>
#include "process.h"

void menu();

int main (void){
    g_clock = 0;
    printf("Sizeof pcb:%ld, sizeof memory:%ld",sizeof(pcb),sizeof(memory));
    return 0;
}

void menu(){
    printf("\t\tMenu\n\t1-Criar Processo\n\t2-Finalizar processo\n");
}