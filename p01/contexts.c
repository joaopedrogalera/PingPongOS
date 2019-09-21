#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

#define STACKSIZE 32768		/* tamanho de pilha das threads */
#define _XOPEN_SOURCE 600	/* para compilar no MacOS */

ucontext_t ContextPing, ContextPong, ContextMain;

/*****************************************************/

void BodyPing (void * arg)
{
   int i ;

   printf ("%s iniciada\n", (char *) arg) ;
   
   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", (char *) arg, i) ;
      swapcontext (&ContextPing, &ContextPong);
   }	
   printf ("%s FIM\n", (char *) arg) ;

   swapcontext (&ContextPing, &ContextMain) ;
}

/*****************************************************/

void BodyPong (void * arg)
{
   int i ;

   printf ("%s iniciada\n", (char *) arg) ;

   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", (char *) arg, i) ;
      swapcontext (&ContextPong, &ContextPing);
   }
   printf ("%s FIM\n", (char *) arg) ;

   swapcontext (&ContextPong, &ContextMain) ;
}

/*****************************************************/

int main (int argc, char *argv[])
{
   char *stack ;

   printf ("Main INICIO\n");

   getcontext (&ContextPing);

   stack = malloc (STACKSIZE) ;
   if (stack)
   {
      ContextPing.uc_stack.ss_sp = stack ;
      ContextPing.uc_stack.ss_size = STACKSIZE;
      ContextPing.uc_stack.ss_flags = 0;
      ContextPing.uc_link = 0;
   }
   else
   {
      perror ("Erro na criação da pilha: ");
      exit (1);
   }

   makecontext (&ContextPing, (void*)(*BodyPing), 1, "    Ping");

   getcontext (&ContextPong);

   stack = malloc (STACKSIZE) ;
   if (stack)
   {
      ContextPong.uc_stack.ss_sp = stack ;
      ContextPong.uc_stack.ss_size = STACKSIZE;
      ContextPong.uc_stack.ss_flags = 0;
      ContextPong.uc_link = 0;
   }
   else
   {
      perror ("Erro na criação da pilha: ");
      exit (1);
   }

   makecontext (&ContextPong, (void*)(*BodyPong), 1, "        Pong");

   swapcontext (&ContextMain, &ContextPing);
   swapcontext (&ContextMain, &ContextPong);

   printf ("Main FIM\n");

   exit (0);
}
