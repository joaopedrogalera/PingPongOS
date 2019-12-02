#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

semaphore_t *s_buffer, *s_vaga, *s_item;
int buffer[5];
char bufferIndex;

void produtor(){
  int item;
  while (1) {
    task_sleep(1);
    item = rand()%99;
    sem_down(s_vaga);
    sem_down(s_buffer);
    buffer[bufferIndex] = item;
    bufferIndex++;
    sem_up(s_buffer);
    sem_up(s_item);
  }
}

void consumidor(){
  while(1){
    sem_down(s_item);
    sem_down(s_buffer);

    sem_up(s_buffer);
    sem_up(s_vaga);

    task_sleep(1);
  }
}

int main(){
  bufferIndex = 0;
  sem_create(s_buffer,5);
  sem_create(s_vaga,5);
  sem_create(s_item,0);

  sem_destroy(s_buffer);
  sem_destroy(s_vaga);
  sem_destroy(s_item);
  return(0);
}
