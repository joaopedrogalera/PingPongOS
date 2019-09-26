#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE 32768

int tid;
task_t taskMain;
task_t *runningTask;

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void pingpong_init(){
  setvbuf(stdout, 0, _IONBF, 0);

  tid = 0;

  taskMain.prev = NULL;
  taskMain.next = NULL;
  taskMain.tid = 0;

  runningTask = &taskMain;
}

// Cria uma nova tarefa. Retorna um ID> 0 ou erro.
int task_create (task_t *task, void (*start_func)(void *), void *arg){
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

  tid++;
  task->prev = NULL;
  task->next = NULL;
  task->tid = tid;

  return(tid);
}

int task_switch (task_t *task){
  task_t *taskAux;
  taskAux = runningTask;
  runningTask = task;
  swapcontext(&(taskAux->contextTask),&(runningTask->contextTask));
  return(0);
}

void task_exit (int exitCode){
  task_t *taskAux;
  taskAux = runningTask;
  runningTask = &taskMain;
  swapcontext(&(taskAux->contextTask),&(taskMain.contextTask));
}

int task_id (){
  return (runningTask->tid);
}
