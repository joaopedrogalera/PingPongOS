#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

#define MAXTASK 1000

task_t task ;

void BodyTask (void * arg)
{
   printf ("Estou na tarefa %5d\n", task_id()) ;
   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   int i ;

   printf ("Main INICIO\n");

   pingpong_init () ;

   // cria MAXTASK tarefas, ativando cada uma apos sua criacao
   for (i=0; i<MAXTASK; i++)
   {
     task_create (&task, BodyTask, NULL) ;
     task_switch (&task) ;
   }

   printf ("Main FIM\n");

   exit (0);
}
