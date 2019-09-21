#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pingpong.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

task_t Pang, Peng, Ping, Pong, Pung ;
barrier_t b ;

void Body (void * arg)
{
   int sleepTime ;

   sleepTime = random() % 20 ;
   printf ("%5d ms: %s dorme %ds\n", systime(), (char *) arg, sleepTime) ;
   task_sleep (sleepTime) ;

   printf ("%5d ms: %s chega na barreira\n", systime(), (char *) arg) ;
   barrier_join (&b) ;
   printf ("%5d ms: %s passa da barreira\n", systime(), (char *) arg) ;

   printf ("%5d ms: %s FIM\n", systime(), (char *) arg) ;
   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   pingpong_init () ;

   printf ("%5d ms: Main INICIO\n", systime()) ;

   barrier_create (&b, 6) ; // main + pang + peng + ... + pung = 6

   task_create (&Pang, Body, "    Pang") ;
   task_create (&Peng, Body, "        Peng") ;
   task_create (&Ping, Body, "            Ping") ;
   task_create (&Pong, Body, "                Pong") ;
   task_create (&Pung, Body, "                    Pung") ;

   printf ("%5d ms: Main chega na barreira\n", systime()) ;
   barrier_join (&b) ;
   printf ("%5d ms: Main passa da barreira\n", systime()) ;

   printf ("%5d ms: Main FIM\n", systime()) ;
   task_exit (0) ;

   exit (0) ;
}
