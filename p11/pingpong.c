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
int intEn = 0; //Controla interrupções

//Função para tratar interrupções
void sigTreat(int signum){
  //Se for uma tarefa de usuario (tid>1), reduz uma unidade do quantum. Ao chegar a 0, retorna ao dispatcher
  if((runningTask->tid)!=1&&intEn){
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
  task_t *search,*searchAux;

  //Enquando houverem tarefas nas filas
  while(readyTasks!=NULL||sleepingTasks!=NULL){
    if(sleepingTasks!=NULL){
      //Percore as tarefas dormindo, verifica as que já podem ser acordadas e as coloca na fila de prontas
      search = sleepingTasks;
      do{
        searchAux = search->next;
        if(systime()>=(search->wakeTime)){
          queue_append((queue_t**) &readyTasks,queue_remove((queue_t**) &sleepingTasks,(queue_t*) search));
        }

        search = searchAux;
      }while(search!=sleepingTasks&&sleepingTasks!=NULL);
    }
    //Solicita ao scheduler a próxima tarefa, tira da fila e executa
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
  sysTime = 0; //isso tem que ser feito antes de entrar no dispatcher
  intEn = 1; //Libera preempção por tempo
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
  //Coloca as tarefas que estavam esperando a conclusão da atual na fila de prontas
  while((runningTask->waiting)!=NULL){
    queue_append((queue_t**) &readyTasks,queue_remove ((queue_t**) &(runningTask->waiting),(queue_t*) (runningTask->waiting)));
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
  //Se task tiver sido encerrada, retorna -1
  if((task->exitCode)>=0){
    return(-1);
  }
  //Se task não tiver sido encerrada, coloca a tarefa atual na espera de task
  queue_append((queue_t**) &(task->waiting),(queue_t*)runningTask);
  task_switch(&taskDispatcher);

  return(task->exitCode);
}

void task_sleep (int t){
  //Calcula o tempo de espera e coloca na fila de espera
  runningTask->wakeTime = systime() + t*1000;
  queue_append((queue_t**) &sleepingTasks,(queue_t*)runningTask);
  task_switch(&taskDispatcher);
}

int sem_create (semaphore_t *s, int value){
  if(s==NULL){
    return(-1);
  }
  s->value = value;
  s->waiting = NULL;
  return(0);
}

int sem_down (semaphore_t *s){
  if(s==NULL){
    return(-1);
  }
  (s->value)--;
  if((s->value)<0){
    queue_append((queue_t**) &(s->waiting), (queue_t*) runningTask);
    task_switch(&taskDispatcher);
  }
  return(0);
}

int sem_up (semaphore_t *s){
  if(s==NULL){
    return(-1);
  }
  (s->value)++;
  if((s->waiting)!=NULL){
    queue_append((queue_t**) &readyTasks, queue_remove ((queue_t**) &(s->waiting),(queue_t*) (s->waiting)));
  }
  return(0);
}

int sem_destroy (semaphore_t *s){
  if(s==NULL){
    return(-1);
  }
  while((s->waiting)!=NULL){
    queue_append((queue_t**) &readyTasks,queue_remove((queue_t**) &(s->waiting),(queue_t*) (s->waiting)));
  }
  s = NULL;
  return(0);
}

int barrier_create (barrier_t *b, int N){
  if(b==NULL){
    return(-1);
  }
  b->value = N;
  b->status = 0;
  b->waiting = NULL;
  if(sem_create(&(b->s),1)==-1){
    return(-1);
  }
  return(0);
}

int barrier_join (barrier_t *b){
  if(b==NULL||(b->status)==-1){
    return(-1);
  }
  sem_down(&(b->s));
  (b->value)--;
  if((b->value)>0){
    queue_append((queue_t**)&(b->waiting),(queue_t*)runningTask);
    sem_up(&(b->s));
    task_switch(&taskDispatcher);
  }
  else{
    while ((b->waiting)!=NULL){
      queue_append((queue_t**)&readyTasks,queue_remove((queue_t**)&(b->waiting),(queue_t*)(b->waiting)));
    }
    sem_up(&(b->s));
  }
  return(0);
}

int barrier_destroy (barrier_t *b){
  if(b==NULL||(b->status)==-1){
    return(-1);
  }
  while((b->waiting)!=NULL){
    queue_append((queue_t**) &readyTasks,queue_remove((queue_t**) &(b->waiting),(queue_t*) (b->waiting)));
  }
  b->status = -1;
  return(0);
}
