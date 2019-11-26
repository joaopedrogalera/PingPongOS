#include "pingpong.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#define STACKSIZE 32768

int tid; //Salva numero de tarefas
task_t taskMain; //Task da tarefa Main
task_t taskDispatcher; //task do dispatcher
task_t *readyTasks = NULL; //Fila de tarefas prontas
task_t *runningTask; //Ponteiro para task em execução
task_t *sleepingTasks = NULL;
struct sigaction action; // estrutura que define um tratador de sinal
struct itimerval timer; // estrutura de inicialização do timer
unsigned int sysTime; //define o tempo do sistema em milisegundos

//Função para tratar interrupções
void sigTreat(int signum){
  //Se for uma tarefa de usuario (tid>1), reduz uma unidade do quantum. Ao chegar a 0, retorna ao dispatcher
  if((runningTask->tid)!=1){
    (runningTask->timeQuantum)--;
    if((runningTask->timeQuantum)==0){
      task_yield();
    }
  }
  sysTime++;
}

//Escalonador com envelhecimento
task_t *scheduler(){
  task_t *nextTask, *taskAux;
  int prio;
  nextTask = readyTasks;
  prio = readyTasks->prio;
  taskAux = readyTasks->next;
  if((readyTasks->prio)>-20){
    (readyTasks->prio)--;
  }
  //Percorre a fila e escolhe a tarefa de menor valor de prioridade
  while(taskAux!=readyTasks){
    if((taskAux->prio)<prio){
      nextTask=taskAux;
      prio = taskAux->prio;
    }
    if((taskAux->prio)>-20){
      (taskAux->prio)--;
    }
    taskAux=taskAux->next;
  }
  nextTask->prio = nextTask->staticPrio;
  return(nextTask);
}

//Função executada pela tarefa do dispatcher
void dispatcher_body(void* args){
  task_t *next;
  task_t *searchAux;

  //Enquando houverem tarefas na fila, solicita a tarefa ao escalonador, retira da fila e executa
  while(readyTasks!=NULL||sleepingTasks!=NULL){
    if(sleepingTasks!=NULL){
      searchAux = sleepingTasks;
      do{
        if(systime()>=(searchAux->wakeTime)){
          printf("%p\n",searchAux);
          queue_remove((queue_t**) &sleepingTasks,(queue_t*) searchAux);
          queue_append((queue_t**) &readyTasks,(queue_t*) searchAux);
        }

        searchAux = searchAux->next;
      }while(searchAux!=sleepingTasks);
    }
    if(readyTasks!=NULL){
      next = scheduler();
      queue_remove((queue_t**) &readyTasks,(queue_t*) next);
      next->timeQuantum = 20;
      task_switch(next);
    }
  }
  task_exit(0);
}

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void pingpong_init(){
  setvbuf(stdout, 0, _IONBF, 0);

  tid = 0; //Por enquanto, apenas main

  //Inicializa variaveis da Task Main
  taskMain.prev = NULL;
  taskMain.next = NULL;
  taskMain.tid = 0;
  taskMain.prio = 0;
  taskMain.staticPrio = 0;
  taskMain.creationTime = 0;
  taskMain.processorTime = 0;
  taskMain.processorStart = 0;
  taskMain.activations = 0;
  taskMain.timeQuantum = 20;
  taskMain.waiting = NULL;
  taskMain.exitCode = -1;
  //Cria a tarefa do dispatcher
  task_create(&taskDispatcher,dispatcher_body,NULL);

  //Main em execução
  runningTask = &taskMain;

  //Registra ação para SIGALARM
  action.sa_handler = sigTreat ;
  sigemptyset (&action.sa_mask) ;
  action.sa_flags = 0 ;
  if (sigaction (SIGALRM, &action, 0) < 0)
  {
    perror ("Erro em sigaction: ") ;
    exit (1) ;
  }

  timer.it_value.tv_usec = 1000 ;      // primeiro disparo, em micro-segundos
  timer.it_interval.tv_usec = 1000 ;   // disparos subsequentes, em micro-segundos

  // arma o temporizador ITIMER_REAL
  if (setitimer (ITIMER_REAL, &timer, 0) < 0)
  {
    perror ("Erro em setitimer: ") ;
    exit (1) ;
  }
  queue_append((queue_t**) &readyTasks,(queue_t*)&taskMain);
  sysTime = 0; //isso tem que ser feito antes de entrar no dispatcher
  task_yield ();
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

  //Soma 1 ao numero de tarefas criado e inicializa variaveis da tarefa (ver datatypes.h para detalhes das variaveis)
  tid++;
  task->prev = NULL;
  task->next = NULL;
  task->tid = tid;
  task->prio = 0;
  task->staticPrio = 0;
  task->creationTime = sysTime;
  task->processorTime = 0;
  task->activations = 0;
  task->waiting = NULL;
  task->exitCode = -1;

  #ifdef DEBUG
    printf("task_create: Criando tarefa %d\n",tid);
  #endif

  //Se for tarefa de usuario, coloca na fila de protas
  if(tid>1){
    queue_append((queue_t**) &readyTasks,(queue_t*)task);
  }

  return(tid);
}

int task_switch (task_t *task){
  #ifdef DEBUG
    printf("task_switch: trocando contexto %d->%d\n",runningTask->tid,task->tid);
  #endif
  //Salva o contexto atual na struct da tarefa em execução, altera o ponteiro runningTask para a task e restaura o contexto salvo em task
  task_t *taskAux;
  taskAux = runningTask;
  runningTask = task;

  //Altera os tempos de processador e ativações das tarefas
  taskAux->processorTime = taskAux->processorTime + (sysTime - taskAux->processorStart);
  runningTask->processorStart = sysTime;
  (runningTask->activations)++;

  swapcontext(&(taskAux->contextTask),&(runningTask->contextTask));
  return(0);
}

void task_exit (int exitCode){
  #ifdef DEBUG
    printf("task_exit: tarefa %d sendo encerrada\n",runningTask->tid);
  #endif

  printf("Task %d exit: execution time %d ms, processor time %d ms, %d activations\n",runningTask->tid,sysTime-(runningTask->creationTime),(runningTask->processorTime),(runningTask->activations));
  (runningTask->exitCode) = exitCode;

  while((runningTask->waiting)!=NULL){
    queue_append((queue_t**) &readyTasks,queue_remove (&(runningTask->waiting), (runningTask->waiting)));
  }
  //Se for tarefa de usuario retorna para o dispatcher. Se não, retorna para main
  if((runningTask->tid)!=1){
    runningTask = &taskDispatcher;
  }
  else{
    exit(0);
  }

  setcontext(&(runningTask->contextTask));
}

int task_id (){
  //Retorna o tid da task em execução
  return (runningTask->tid);
}

void task_yield (){
  //Se a task em execução for uma tarefa de usuario, coloca na fila de prontas e muda o contexto para o dispatcher
  if((runningTask->tid)!=1){
    queue_append((queue_t**) &readyTasks,(queue_t*)runningTask);
  }
  task_switch(&taskDispatcher);
}

void task_setprio (task_t *task, int prio){
  if(task!=NULL){
    task->prio = prio;
  }
  else{
    runningTask->prio = prio;
  }
}

int task_getprio (task_t *task){
  if(task!=NULL){
    return(task->prio);
  }
  else{
    return(runningTask->prio);
  }
}

unsigned int systime (){
  return(sysTime);
}

int task_join (task_t *task){
  if((task->exitCode)>=0){
    return(-1);
  }
  queue_append((queue_t**) &(task->waiting),(queue_t*)runningTask);
  task_switch(&taskDispatcher);

  return(task->exitCode);
}

void task_sleep (int t){
  runningTask->wakeTime = systime() + t*1000;
  printf("%p\n",runningTask);
  queue_append((queue_t**) &sleepingTasks,(queue_t*)runningTask);
  task_switch(&taskDispatcher);
}
