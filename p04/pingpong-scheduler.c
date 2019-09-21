// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Teste do escalonador por prioridades dinâmicas

#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

task_t Pang, Peng, Ping, Pong, Pung ;

void Body (void * arg)
{
  int i ;

  printf ("%s iniciando com prioridade %d\n", (char *) arg, task_getprio(NULL)) ;

  for (i=0; i<10; i++)
  {
    printf ("%s %d\n", (char *) arg, i) ;
    task_yield ();
  }
  printf ("%s FIM\n", (char *) arg) ;
  task_exit (0) ;
}

int main (int argc, char *argv[])
{
  printf ("Main INICIO\n");

  pingpong_init () ;

  task_create (&Pang, Body, "    Pang") ;
  task_setprio (&Pang, 0);

  task_create (&Peng, Body, "        Peng") ;
  task_setprio (&Peng, 2);

  task_create (&Ping, Body, "            Ping") ;
  task_setprio (&Ping, 4);

  task_create (&Pong, Body, "                Pong") ;
  task_setprio (&Pong, 6);

  task_create (&Pung, Body, "                    Pung") ;
  task_setprio (&Pung, 8);

  task_yield () ;

  printf ("Main FIM\n");
  exit (0) ;
}
