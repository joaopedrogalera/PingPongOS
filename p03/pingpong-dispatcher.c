#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

task_t Pang, Peng, Ping, Pong, Pung ;

void Body (void * arg)
{
   int i ;

   for (i=0; i<4; i++)
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
   task_create (&Peng, Body, "        Peng") ;
   task_create (&Ping, Body, "            Ping") ;
   task_create (&Pong, Body, "                Pong") ;
   task_create (&Pung, Body, "                    Pung") ;

   task_yield () ;

   printf ("Main FIM\n");
   exit (0);
}
