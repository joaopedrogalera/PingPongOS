#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

task_t Ping, Pong ;

void BodyPing (void * arg)
{
   int i ;
   char* name = (char *) arg ;

   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", name, i) ;
      task_switch (&Pong);
   }
   printf ("%s FIM\n", name) ;
   task_exit (0) ;
}

void BodyPong (void * arg)
{
   int i ;
   char* name = (char *) arg ;

   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", name, i) ;
      task_switch (&Ping);
   }
   printf ("%s FIM\n", name) ;
   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   printf ("Main INICIO\n");

   pingpong_init () ;

   task_create (&Ping, BodyPing, "    Ping") ;
   task_create (&Pong, BodyPong, "        Pong") ;

   task_switch (&Ping) ;
   task_switch (&Pong) ;

   printf ("Main FIM\n");

   exit (0);
}
