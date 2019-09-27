#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 32768

int tid; //Salva numero de tarefas
task_t taskMain; //Task da tarefa Main
task_t *runningTask; //Ponteiro para task em execução

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void pingpong_init(){
  setvbuf(stdout, 0, _IONBF, 0);

  tid = 0; //Por enquanto, apenas main

  //Inicializa variaveis da Task Main
  taskMain.prev = NULL;
  taskMain.next = NULL;
  taskMain.tid = 0;

  //Main em execução
  runningTask = &taskMain;
}

// Cria uma nova tarefa. Retorna um ID> 0 ou erro.
int task_create (task_t *task, void (*start_func)(void *), void *arg){

  //Criação padrão de novo contexto
  char *stack;

  getcontext(&(task->contextTask));

  stack = malloc(STACKSIZE);
  if (stack)
  {
     (task->contextTask).uc_stack.ss_sp = stack ;
     (task->contextTask).uc_stack.ss_size = STACKSIZE;
     (task->contextTask).uc_stack.ss_flags = 0;
     (task->contextTask).uc_link = 0;
  }
  else
  {
     return(-1);
  }

  makecontext(&(task->contextTask),(void*)(*start_func),1,arg);

  //Soma 1 ao numero de tarefas criado e inicializa variaveis da tarefa
  tid++;
  task->prev = NULL;
  task->next = NULL;
  task->tid = tid;

  #ifdef DEBUG
    printf("task_create: Criando tarefa %d",tid);
  #endif

  return(tid);
}

int task_switch (task_t *task){
  #ifdef DEBUG
    printf("task_switch: trocando contexto %d->%d",runningTask->tid,task->tid);
  #endif
  //Salva o contexto atual na struct da tarefa em execução, altera o ponteiro runningTask para a task e restaura o contexto salvo em task
  task_t *taskAux;
  taskAux = runningTask;
  runningTask = task;
  swapcontext(&(taskAux->contextTask),&(runningTask->contextTask));
  return(0);
}

void task_exit (int exitCode){
  //Salva o contexto atual na struct da tarefa em execução, altera o ponteiro runningTask para a task Main e restaura o contexto salvo na task Main
  #ifdef DEBUG
    printf("task_exit: tarefa %d sendo encerrada",runningTask->tid);
  #endif
  task_t *taskAux;
  taskAux = runningTask;
  runningTask = &taskMain;
  swapcontext(&(taskAux->contextTask),&(taskMain.contextTask));
}

int task_id (){
  //Retorna o tid da task em execução
  return (runningTask->tid);
}
