// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Interface do núcleo para as aplicações

#ifndef __PINGPONG__
#define __PINGPONG__

#define _XOPEN_SOURCE 600	// para evitar erros POSIX no MacOS X

#include "datatypes.h"		// estruturas de dados necessárias

// funções gerais ==============================================================

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void pingpong_init () ;

// gerência de tarefas =========================================================

// Cria uma nova tarefa. Retorna um ID> 0 ou erro.
int task_create (task_t *task,			// descritor da nova tarefa
                 void (*start_func)(void *),	// funcao corpo da tarefa
                 void *arg) ;			// argumentos para a tarefa

// Termina a tarefa corrente, indicando um valor de status encerramento
void task_exit (int exitCode) ;

// alterna a execução para a tarefa indicada
int task_switch (task_t *task) ;

// retorna o identificador da tarefa corrente (main eh 0)
int task_id () ;

// suspende uma tarefa, retirando-a de sua fila atual, adicionando-a à fila
// queue e mudando seu estado para "suspensa"; usa a tarefa atual se task==NULL
void task_suspend (task_t *task, task_t **queue) ;

// acorda uma tarefa, retirando-a de sua fila atual, adicionando-a à fila de
// tarefas prontas ("ready queue") e mudando seu estado para "pronta"
void task_resume (task_t *task) ;

// operações de escalonamento ==================================================

// libera o processador para a próxima tarefa, retornando à fila de tarefas
// prontas ("ready queue")
void task_yield () ;

// define a prioridade estática de uma tarefa (ou a tarefa atual)
void task_setprio (task_t *task, int prio) ;

// retorna a prioridade estática de uma tarefa (ou a tarefa atual)
int task_getprio (task_t *task) ;

// operações de sincronização ==================================================

// a tarefa corrente aguarda o encerramento de outra task
int task_join (task_t *task) ;

// operações de gestão do tempo ================================================

// suspende a tarefa corrente por t segundos
void task_sleep (int t) ;

// retorna o relógio atual (em milisegundos)
unsigned int systime () ;

// operações de IPC ============================================================

// semáforos

// cria um semáforo com valor inicial "value"
int sem_create (semaphore_t *s, int value) ;

// requisita o semáforo
int sem_down (semaphore_t *s) ;

// libera o semáforo
int sem_up (semaphore_t *s) ;

// destroi o semáforo, liberando as tarefas bloqueadas
int sem_destroy (semaphore_t *s) ;

// mutexes

// Inicializa um mutex (sempre inicialmente livre)
int mutex_create (mutex_t *m) ;

// Solicita um mutex
int mutex_lock (mutex_t *m) ;

// Libera um mutex
int mutex_unlock (mutex_t *m) ;

// Destrói um mutex
int mutex_destroy (mutex_t *m) ;

// barreiras

// Inicializa uma barreira
int barrier_create (barrier_t *b, int N) ;

// Chega a uma barreira
int barrier_join (barrier_t *b) ;

// Destrói uma barreira
int barrier_destroy (barrier_t *b) ;

// filas de mensagens

// cria uma fila para até max mensagens de size bytes cada
int mqueue_create (mqueue_t *queue, int max, int size) ;

// envia uma mensagem para a fila
int mqueue_send (mqueue_t *queue, void *msg) ;

// recebe uma mensagem da fila
int mqueue_recv (mqueue_t *queue, void *msg) ;

// destroi a fila, liberando as tarefas bloqueadas
int mqueue_destroy (mqueue_t *queue) ;

// informa o número de mensagens atualmente na fila
int mqueue_msgs (mqueue_t *queue) ;

//==============================================================================

// Redefinir funcoes POSIX "proibidas" como "FORBIDDEN" (gera erro ao compilar)

// POSIX threads
#define pthread_create			FORBIDDEN
#define pthread_join			FORBIDDEN
#define pthread_exit			FORBIDDEN

// POSIX semaphores
#define sem_init			FORBIDDEN
#define sem_post			FORBIDDEN
#define sem_wait			FORBIDDEN
#define sem_trywait			FORBIDDEN

// POSIX message queues
#define mq_open				FORBIDDEN
#define mq_send				FORBIDDEN
#define mq_receive			FORBIDDEN
#define mq_close			FORBIDDEN

// POSIX condvars
#define pthread_cond_init		FORBIDDEN
#define pthread_cond_wait		FORBIDDEN
#define pthread_cond_signal		FORBIDDEN
#define pthread_cond_timedwait		FORBIDDEN

// POSIX barriers
#define pthread_barrier_init		FORBIDDEN
#define pthread_barrier_wait		FORBIDDEN
#define pthread_barrier_destroy		FORBIDDEN

// POSIX mutexes
#define pthread_mutex_init		FORBIDDEN
#define pthread_mutex_lock		FORBIDDEN
#define pthread_mutex_unlock		FORBIDDEN
#define pthread_mutex_timedlock		FORBIDDEN
#define pthread_mutex_trylock		FORBIDDEN
#define pthread_mutex_destroy		FORBIDDEN

// POSIX RW-locks
#define pthread_rwlock_init		FORBIDDEN
#define pthread_rwlock_rdlock		FORBIDDEN
#define pthread_rwlock_wrlock		FORBIDDEN
#define pthread_rwlock_unlock		FORBIDDEN
#define pthread_rwlock_tryrdlock	FORBIDDEN
#define pthread_rwlock_tryrwlock	FORBIDDEN
#define pthread_rwlock_timedrdlock	FORBIDDEN
#define pthread_rwlock_timedrwlock	FORBIDDEN
#define pthread_rwlock_destroy		FORBIDDEN

// POSIX spinlocks
#define pthread_spin_init		FORBIDDEN
#define pthread_spin_lock		FORBIDDEN
#define pthread_spin_unlock		FORBIDDEN
#define pthread_spin_trylock		FORBIDDEN
#define pthread_spin_destroy		FORBIDDEN

#endif
