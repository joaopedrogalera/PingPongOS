// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Estruturas de dados internas do sistema operacional

#ifndef __DATATYPES__
#define __DATATYPES__
#include <ucontext.h>

// Estrutura que define uma tarefa
typedef struct task_t
{
  struct task_t *prev, *next;
  int tid;
  ucontext_t contextTask;
  int prio; //indica a prioridade da tarefa
  int staticPrio; //indica a prioridade estatica da tarefa
  int timeQuantum; //indica o quantum de tempo. Definido pelo dispatcher
  unsigned int creationTime; //Define o momento que a tarefa foi criada
  unsigned int processorTime;  //Define o tempo de processador decorrido
  unsigned int processorStart; //Define o tempo da ultima entrada em processador
  int activations;
} task_t ;

// estrutura que define um semáforo
typedef struct
{
  // preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif
