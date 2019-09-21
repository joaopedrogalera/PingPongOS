#include <stdio.h>
#include "queue.h"

void queue_append (queue_t **queue, queue_t *elem){
  queue_t *last;

  if(queue==NULL){
    printf("A fila nao existe\n");
  }
  else if(elem==NULL){
    printf("Elemento nao existe\n");
  }
  else if((elem->prev!=NULL)||(elem->next!=NULL)){
    printf("Elemento ja esta em uma fila\n");
  }
  else if((*queue)==NULL){
    (*queue) = elem;
    elem->prev = elem;
    elem->next = elem;
  }
  else{
    last = (*queue)->prev;
    last->next = elem;
    (*queue)->prev = elem;
    elem->next = *queue;
    elem->prev = last;
  }
}

queue_t *queue_remove (queue_t **queue, queue_t *elem){
  queue_t *atual;
  char ismember = 0;
  if(queue==NULL){
    printf("A fila nao existe\n");
    return (NULL);
  }
  else if(elem==NULL){
    printf("Elemento nao existe\n");
    return (NULL);
  }
  else if((*queue)==NULL){
    printf("A fila esta vazia\n");
    return(NULL);
  }
  else{
    atual = *queue;
    do{
      if(atual==elem){
        ismember = 1;
      }
      atual = atual->next;
    }
    while(atual!=(*queue)&&!ismember);

    if(!ismember){
      printf("Elemento nao pertence a fila\n");
      return(NULL);
    }

    if(elem->next == elem){
      (*queue) = NULL;
    }
    else{
      if(elem==(*queue)){
        (*queue)=elem->next;
      }
      elem->prev->next = elem->next;
      elem->next->prev = elem->prev;
    }
    elem->prev = NULL;
    elem->next = NULL;

    return(elem);
  }
}

int queue_size (queue_t *queue){
  int size = 0;
  queue_t *elem;
  if(queue==NULL){
    return(0);
  }

  elem = queue;
  do{
    size += 1;
    elem = elem->next;
  }while(elem!=queue);

  return(size);
}

void queue_print (char *name, queue_t *queue, void print_elem (void*) ){
  queue_t *elem;
  printf("%s[",name);
  if(queue!=NULL){
    elem = queue;
    do{
      print_elem(elem);
      elem = elem->next;
    }while(elem!=queue);
  }
  printf("]\n");
}
