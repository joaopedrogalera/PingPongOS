#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

#define NUMTASKS 30
#define NUMSTEPS 10000000

task_t task[NUMTASKS] ;
semaphore_t  s ;
long int soma = 0 ;

void taskBody(void *id)
{
   int i ;

   for (i=0; i< NUMSTEPS; i++)
   {
      sem_down (&s) ;
      soma += 1 ;
      sem_up (&s) ;
   }

   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   int i ;
   
   printf ("Main INICIO\n") ;

   pingpong_init () ;

   sem_create (&s, 1) ;

   printf ("%d tarefas somando %d vezes cada, aguarde...\n",
           NUMTASKS, NUMSTEPS) ;

   for (i=0; i<NUMTASKS; i++)
     task_create (&task[i], taskBody, "Task") ;

   for (i=0; i<NUMTASKS; i++)
     task_join (&task[i]) ;

   sem_destroy (&s) ;

   if (soma == (NUMTASKS*NUMSTEPS))
     printf ("Soma deu %ld, valor correto!\n", soma) ;
   else
     printf ("Soma deu %ld, mas deveria ser %d!\n",
             soma, NUMTASKS*NUMSTEPS) ;

   task_exit (0) ;

   exit (0) ;
}
