#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

#define NUMSTEPS 60

semaphore_t s_buffer, s_vaga, s_item;
task_t p1, p2, p3, c1, c2, c3;

typedef struct item_t{
  struct buffer_t *prev,*next;
  int valor;
}item_t;

item_t *buffer;

void produtor(void *arg){
  item_t *item;
  int i;
  printf("%s início\n",(char*)arg);
  for(i=0;i<NUMSTEPS/3;i++) {
    task_sleep(1);
    item = malloc(sizeof(item_t));
    item->prev = NULL;
    item->next=NULL;
    item->valor = rand()%99;

    sem_down(&s_vaga);

    sem_down(&s_buffer);
    queue_append((queue_t**)&buffer,(queue_t*)item);
    printf("%s produziu %d\n",(char*)arg,item->valor);
    sem_up(&s_buffer);

    sem_up(&s_item);
  }
  task_exit(0);
}

void consumidor(void *arg){
  item_t *item;
  int i;
  printf("%s início\n",(char*)arg);
  for(i=0;i<NUMSTEPS/2;i++) {
    sem_down(&s_item);
    sem_down(&s_buffer);
    item = (item_t*) queue_remove((queue_t**)&buffer,(queue_t*)buffer);
    printf("%s consumiu %d\n",(char*)arg,item->valor);
    free(item);
    sem_up(&s_buffer);
    sem_up(&s_vaga);

    task_sleep(1);
  }
  task_exit(0);
}

int main(){
  pingpong_init();
  printf("MAIN início\n");

  sem_create(&s_buffer,5);
  sem_create(&s_vaga,5);
  sem_create(&s_item,0);

  task_create(&p1,produtor,"p1");
  task_create(&p2,produtor,"p2");
  task_create(&p3,produtor,"p3");
  task_create(&c1,consumidor,"        c1");
  task_create(&c2,consumidor,"        c2");

  task_join(&p1);
  task_join(&p2);
  task_join(&p3);
  task_join(&c1);
  task_join(&c2);

  sem_destroy(&s_buffer);
  sem_destroy(&s_vaga);
  sem_destroy(&s_item);
  task_exit(0);
  exit(0);
}
