#include <stdio.h>
#include "queue.h"

void queue_append (queue_t **queue, queue_t *elem){
  queue_t *last;

  //queue é ponteiro para o first, que é um ponteiro para o primeiro elemento
  //Faz os testes necessarios
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
    //Se a fila é vazia, o elemento é adicionado ao first e ao prev e next dele mesmo
    (*queue) = elem;
    elem->prev = elem;
    elem->next = elem;
  }
  else{
    //Pega o ultimo elemento e altera os ponteiros do ultimo e do ele adicionado
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

  //queue é ponteiro para o first, que é um ponteiro para o primeiro elemento
  //Faz os testes necessarios
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

    //Percorre a lista procurando o elemento. Se encontrar, seta ismember para 1
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

    //Se o elemento for o unico da fila, seta a fila como vazia
    if(elem->next == elem){
      (*queue) = NULL;
    }
    else{
      //Se o elemento for o primeiro da fila, seta o first para o elemento seguinte
      if(elem==(*queue)){
        (*queue)=elem->next;
      }
      //ajusta os ponteiros para os elementos anterior e proximo
      elem->prev->next = elem->next;
      elem->next->prev = elem->prev;
    }
    //ajusta os ponteiros do elemento removido
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

  //Percorre a lista e conta os elementos
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
    //Percorre a lista e imprime os elementos
    do{
      print_elem(elem);
      elem = elem->next;
    }while(elem!=queue);
  }
  printf("]\n");
}
