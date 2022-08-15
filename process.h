#ifndef PROCESS_H
#define PROCESS_H

#define MAX_MEMORY 248509
#define LOW_PRIO 2000
#define HIGH_PRIO 1000
#define PAGE_SIZE 4

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <stdint.h>

#include <semaphore.h>

#include <ctype.h>

#include <string.h>


unsigned int g_clock;
int g_memory;
int pid;
int trilhaAtual;

sem_t semaphore;
sem_t round_sem;
pthread_mutex_t lock;

//prototypes of functions

/*
TODO
    * imaginar uma forma de criar memoria de 1gb com separações de 4k //done
    * Semaforo
    * Fazer todas as funções declaradas lá embaixo 
    * Criar um relógio // done
    * 
    * IDEIA PARA O RR, um algoritmo padrao que recebe a lista. MAS a gente usa duas threads? uma pra baixa prioridade e outra pra alta prioridade e quisifoda HDAUHSUHASD pode ser que de 
    * com isso a lista de maior prioridade roda 2x e a de menor uma vez so, faz sentido? 

*/

typedef enum instruction {
  exec = 0,
    read,
    write,
    print,
    I_semaphore,
    end,
}
inst;

typedef struct instructionBlock {
  inst instructionR;
  int timeK;
}
IB;

typedef struct memory {
  int pid;
  int32_t mem[1000]; //4kb de inteiros
}
memoryType; //1gb de memoria

typedef enum PCB_STATES {
  CREATED = 0,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
}
PCB_STATE;

typedef struct blocoControleProcesso {
  char name[20];
  int identifier;
  int pid;
  PCB_STATE states;
  bool isHigh;
  bool execIO;
  bool fim;
  int instrucao;
  int quantum;
  int memory;
  char semaphore[5][1];
  IB instructionB[1000];
  struct blocoControleProcesso * next;
}
pcb;

void newNode(pcb * process, pcb * novo);
pcb * processFinish(pcb * process, int pid);
pcb * findProcess(pcb * process, int pid);
void memLoadFinish(int pid, memoryType * memoryTotal);
pcb * memLoadReq(pcb * process, memoryType * memoryTotal, int pid);
void processInterrupt(pcb * process);
void * round_robin();
void semaphoreP(int mutex, pcb * process);
void semaphoreV(int mutex, pcb * process);
pcb * processCreate(pcb * NewProcess);
pcb * processExec(pcb * process);
pcb * openFile(char arch[30]);
pcb * startProcess(FILE * fp);
int diskRequest(int trilha);

pcb * headHigh;
pcb * headLow;

void newNode(pcb * process, pcb * novo) { //é para add no final?
  while (process -> next != NULL) {
    process = process -> next;
  }
  process -> next = novo;
}

pcb * processFinish(pcb * process, int pid) {
  pcb * head = process;
  pcb * prev = process;
  while (process -> pid != pid) {
    if (process -> next == NULL) {
      fprintf(stderr, "ERROR: PID DOES NOT EXIST\n");
      return head;
    }
    prev = process;
    process = process -> next;
  }
  if (process -> next == NULL||prev -> next == NULL) {
    //prev->next = NULL;
    printf("FREE AT PROCESS NULL %d\n", process -> pid);
    free(process);
    return prev;
  } else {
    prev -> next = process -> next;
    printf("FREE AT PROCESS %d\n", process -> pid);
    free(process);
    return prev;
  }

}

pcb * findProcess(pcb * process, int pid) {
  while (!(process -> pid == pid)) {
    process = process -> next;
    if (process -> next == NULL) {
      fprintf(stderr, "ERROR AT FINDING PROCESS <WRONG OR MISSING PID>");
      return process;
    }

  }
  return process;
}

//
void memLoadFinish(int pid, memoryType * memoryTotal) {
  //clean memoryTotal of pid 
  int i = 0;
  while (memoryTotal[i].pid != pid) {
    i++;
  }
  while (memoryTotal[i].pid == pid) {
    memoryTotal[i].pid = 0;
    i++;
  }
}

pcb * memLoadReq(pcb * process, memoryType * memoryTotal, int pid) {
  pcb * head = process;
  int num_of_mBlocks = (process -> memory / 4);
  while (num_of_mBlocks != 0) {
    memoryTotal[g_memory].pid = pid;
    g_memory++;
    num_of_mBlocks--;
  }
  return head;
}

void processInterrupt(pcb * process) {
  pcb * aux = process->next;

  process -> next = NULL;
  process -> states = BLOCKED;
  if (process->fim == true) {
    process -> states = TERMINATED;
    process = processFinish(process, process -> pid);
    process = NULL;
    headHigh = NULL;
    headLow = NULL;
  }
  if(process== NULL){
    return;
  }
  if (process -> isHigh) {
    headHigh = aux;
  }
  if (process -> isHigh == false) {
    headLow = aux;
  }
  if (process -> execIO) {
    printf("Process transfering to HEADLOW\n");
    process -> isHigh = false;
    if(headLow == NULL) {
      headLow = process;
    } else {
      newNode(headLow, process);
    }
  } else {
    printf("Process transfering to HEADHIGH\n");
    if(headHigh == NULL) {
      headHigh = process;
    } else {
      newNode(headHigh, process);
    }
  }
  printf("Process PID %d BLOCKED\n", process -> pid);
}

void * round_robin() { //por hora o round robin só roda exec! Sera implementado na próxima etapa!
  // tamanho maximo de tempo = 1000 ou 2000 (depende da prioridade)
  //semaforo
  int arrival_time;
  pcb * process;
  while (true) {
    pthread_mutex_lock(&lock);
    if (headHigh == NULL && headLow == NULL) {
      printf("NO PROCESS\n"); 
      pthread_mutex_lock(&lock);  
    }
    if (headHigh != NULL) {
      printf("HIGH PROCESS\n");
      process = headHigh;
    } else {
      printf("LOW PROCESS\n");
      process = headLow;
    }
    arrival_time = g_clock;
    printf("QUANTUM ATUAL %d RR!\n", process -> quantum);
    printf("Arrival_TIME %d \n", arrival_time);
    //pegue instruções ate chegar a 1000 ou 2000
    if (process -> isHigh) {
      if (process -> quantum < 1000 && !process -> fim) {
        process = processExec(process);
      }
    } else {
      if (process -> quantum < 2000 && !process -> fim) {
        process = processExec(process);
      }
    }

    if (process -> fim == true && process -> quantum == 0) {
      printf("PROCESS PID %d FINISH\n", process -> pid);
      process = processFinish(process, process -> pid);
    } else {
      switch (process -> isHigh) {
      case true:
        printf("Process PID %d HIGH\n", process -> pid);
        if (process -> quantum > 1000) {
          g_clock += 1000;
          process -> quantum -= 1000;
          printf("PROCESS PID %d -1000 STATE RUNNING\n", process -> pid);
          //goto final da lista
          processInterrupt(process);

        } else {
          g_clock += process -> quantum;
          process -> quantum = 0;
          printf("PROCESS PID %d RUNNING HIGH\n", process -> pid);
          if (process -> next != NULL) {
            headHigh = process -> next;
          } else if (process -> next == NULL) {
            headHigh = NULL;
            printf("HEAD HIGH NULL\n");
          }
          processInterrupt(process);
        }
        break;

      case false:
        printf("Process PID %d LOW\n", process -> pid);
        if (process -> quantum > 2000) {
          g_clock += 2000;
          process -> quantum = process -> quantum - 2000;
          printf("PROCESS PID %d -2000 STATE RUNNING\n", process -> pid);
          //goto final da lista 
          processInterrupt(process);

        } else {
          g_clock += process -> quantum;
          process -> quantum = 0;
          if (process -> next != NULL) {
            headLow = process -> next;
          } else if (process -> next == NULL) {
            headLow = NULL;
          }
          //process finish
          printf("PROCESS PID %d RUNNING %d\n", process -> pid, process -> quantum);
          processInterrupt(process);
        }
        break;
      }

    }
    pthread_mutex_unlock(&lock);
  }

}

void semaphoreP(int mutex, pcb * process) {

  if (mutex > 0) {
    mutex--;
    pthread_mutex_unlock( & lock);
    //unlocked
  } else {

    pthread_mutex_lock( & lock);
    process -> states = BLOCKED;
    //lock;

  }
}

void semaphoreV(int mutex, pcb * process) {

  if (process -> states == BLOCKED) {
    mutex++;

  }

}

//execute process
pcb * processExec(pcb * process) {
  int i, time;
  int quantumDisk;
  process -> states = RUNNING;
  process -> execIO = false;
  i = process -> instrucao;

  if (process -> isHigh) {
    time = 1000;
  } else {
    time = 2000;
  }
  while (process -> quantum < time) {
    printf("QUANTUM ATUAL %d\n", process -> quantum);
    if (process -> fim) {
      break;
    }
    printf("PROCESS PID %d - %d STATE RUNNING\n", process -> pid, process -> instructionB[i].instructionR);
    switch (process -> instructionB[i].instructionR) {
    case exec:
      printf("PROCESS PID %d - %d STATE EXEC\n", process -> pid, process -> instructionB[i].instructionR);
      if (process -> isHigh) {
        if (process -> instructionB[i].timeK >= 1000) {
          process -> quantum += 1000;
          process -> instructionB[i].timeK -= 1000;
        } else {
          process -> quantum += process -> instructionB[i].timeK;
          process -> instructionB[i].timeK = 0;
        }
      } else {
        if (process -> instructionB[i].timeK >= 2000) {
          process -> quantum += 2000;
          process -> instructionB[i].timeK -= 2000;
        } else {
          process -> quantum += process -> instructionB[i].timeK;
          process -> instructionB[i].timeK = 0;
        }
      }
      break;
    case read:
      printf("PROCESS PID %d - %d STATE READ\n", process -> pid, process -> instructionB[i].instructionR);
      process -> execIO = true;
      process -> quantum += 5000;
      quantumDisk = diskRequest(process -> instructionB[i].timeK);
      process -> quantum += quantumDisk;
      process -> instructionB[i].timeK = 0;

      break;
    case write:
      printf("PROCESS PID %d - %d STATE WRITE\n", process -> pid, process -> instructionB[i].instructionR);
      process -> execIO = true;
      process -> quantum += 5000;
      quantumDisk = diskRequest(process -> instructionB[i].timeK);
      process -> quantum += quantumDisk;
      process -> instructionB[i].timeK = 0;
      break;
    case print:
      printf("PROCESS PID %d - %d STATE PRINT\n", process -> pid, process -> instructionB[i].instructionR);
      if (process -> isHigh) {
        if (process -> instructionB[i].timeK >= 1000) {
          process -> quantum += 1000;
          process -> instructionB[i].timeK -= 1000;
        } else {
          process -> quantum += process -> instructionB[i].timeK;
          process -> instructionB[i].timeK = 0;
        }
      } else {
        if (process -> instructionB[i].timeK >= 2000) {
          process -> quantum += 2000;
          process -> instructionB[i].timeK -= 2000;
        } else {
          process -> quantum += process -> instructionB[i].timeK;
          process -> instructionB[i].timeK = 0;
        }
      }
      break;
    case I_semaphore:
      printf("PROCESS PID %d - %d STATE SEMAPHORE\n", process -> pid, process -> instructionB[i].instructionR);
      process -> quantum += 200;
      process -> instructionB[i].timeK = 0;
      break;
    case end:
      process -> fim = true;
      break;
    }
    i++;
    process -> instrucao = i;
  }
  return process;
}

//DiskRequest
int diskRequest(int trilha) {
  int custo = 0;
  //cada trilha custa 100
  if (trilhaAtual < trilha) {
    custo = trilha - trilhaAtual;
    custo = custo * 100;
    trilhaAtual = trilha;
  } else {
    custo = trilhaAtual - trilha;
    custo = custo * 100;
    trilhaAtual = trilha;
  }
  return custo;
}

pcb * openFile(char arch[30]) {
  FILE * fp;
  fp = fopen(arch, "r");
  pcb * process = startProcess(fp);
  fclose(fp);
  return process;
}

pcb * startProcess(FILE * fp) {
  char ch;
  int i = 0;
  pcb * novoProcesso = (pcb * ) malloc(sizeof(pcb));
  if (fscanf(fp, "%s\n", novoProcesso -> name) == 0) {
    fprintf(stderr, "ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
  }
  printf("PROCESS NAME %s\n", novoProcesso -> name);
  if (fscanf(fp, "%d\n", & novoProcesso -> identifier) == 0) {
    fprintf(stderr, "ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
  }
  printf("PROCESS IDENTIFIER %d\n", novoProcesso -> identifier);
  if (fscanf(fp, "%d\n", & i) == 0) {
    fprintf(stderr, "ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
  }
  if (i == 0) {
    novoProcesso -> isHigh = false;
  } else {
    novoProcesso -> isHigh = true;
  }
  i = 0;
  if (fscanf(fp, "%d\n", & novoProcesso -> memory) == 0) {
    fprintf(stderr, "ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
  }
  printf("PROCESS MEMORY %d\n", novoProcesso -> memory);
  while ((ch = fgetc(fp)) != '\n') {
    if (isspace(ch) == 0) {
      novoProcesso -> semaphore[i][0] = ch;
      i++;
    }
  }
  i = 0;
  char aux[6];
  fgetc(fp);
  while (!feof(fp)) {
    if (fscanf(fp, "%s ", aux) == 0) {
      fprintf(stderr, "ERROR AT SCANNING FILE <WRONG OR MISSING FILE>");
    }
    printf("%s\n", aux);
    if (strcmp(aux, "exec") == 0) {
      novoProcesso -> instructionB[i].instructionR = exec;
    } else if (strcmp(aux, "read") == 0) {
      novoProcesso -> instructionB[i].instructionR = read;
    } else if (strcmp(aux, "write") == 0) {
      novoProcesso -> instructionB[i].instructionR = write;
    } else if (strcmp(aux, "print") == 0) {
      novoProcesso -> instructionB[i].instructionR = print;
    } else {
      novoProcesso -> instructionB[i].instructionR = I_semaphore;
    }
    if (fscanf(fp, "%d\n", & novoProcesso -> instructionB[i].timeK) == 0) {
      novoProcesso -> instructionB[i].timeK = 0;
      i++;
      continue;
    }
    i++;
  }
  //print every instruction

  
  novoProcesso -> next = NULL;
  novoProcesso -> instructionB[i].instructionR = end;
  novoProcesso -> instructionB[i].timeK = 0;
  novoProcesso -> instrucao = 0;
  novoProcesso -> quantum = 0;
  novoProcesso -> fim = false;
  novoProcesso -> pid = pid++;
  return novoProcesso;

}

#endif