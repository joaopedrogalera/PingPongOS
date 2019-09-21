// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Teste das operações de acesso a disco

#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"
#include "diskdriver.h"

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

#define NUMTASKS  16

// tarefas
task_t mover[NUMTASKS] ;	// tarefas movedoras de blocos
int numBlocks ;			// numero de blocos no disco
int blockSize ;			// tamanho de cada bloco (bytes)

void moverBody (void * arg)
{
  long myNumber ;
  int i, j, block_orig, block_dest ;
  char *buffer1, *buffer2 ;
  int blocksPerTask = (numBlocks/NUMTASKS/2) ;

  buffer1 = malloc (blockSize) ;
  buffer2 = malloc (blockSize) ;

  // define os blocos iniciais
  myNumber = (long) arg ;
  block_orig = myNumber * blocksPerTask ;
  block_dest = numBlocks - 1 - (myNumber * blocksPerTask) ;

  printf ("T%d will move %d blocks between %d and %d\n",
          task_id(), blocksPerTask, block_orig, block_dest) ;

  // move blocksPerTask blocos
  for (i = 0 ; i < blocksPerTask; i++)
  {
    // le o bloco b1 do disco
    printf ("T%02d will read block %d\n", task_id(), block_orig) ;
    if (disk_block_read (block_orig, buffer1) == 0)
      printf ("T%02d read  block %d\n", task_id(), block_orig) ;
    else
      printf ("T%02d error reading block %d\n", task_id(), block_orig) ;

    // le o bloco b2 do disco
    printf ("T%02d will read block %d\n", task_id(), block_dest) ;
    if (disk_block_read (block_dest, buffer2) == 0)
      printf ("T%02d read  block %d\n", task_id(), block_dest) ;
    else
      printf ("T%02d error reading block %d\n", task_id(), block_dest) ;

    // mostra o conteudo do bloco b1
    printf ("T%02d block %d contents: [", task_id(), block_orig) ;
    for (j = 0; j < blockSize; j++)
      printf ("%c", buffer1[j]) ;
    printf ("]\n") ;

    // mostra o conteudo do bloco b2
    printf ("T%02d block %d contents: [", task_id(), block_dest) ;
    for (j = 0; j < blockSize; j++)
      printf ("%c", buffer2[j]) ;
    printf ("]\n") ;

    // escreve o bloco b1 no disco
    printf ("T%02d will write block %d\n", task_id(), block_dest) ;
    if (disk_block_write (block_dest, buffer1) == 0)
      printf ("T%02d wrote block %d\n", task_id(), block_dest) ;
    else
      printf ("T%02d error writing block %d\n", task_id(), block_dest) ;

    // escreve o bloco b2 no disco
    printf ("T%02d will write block %d\n", task_id(), block_orig) ;
    if (disk_block_write (block_orig, buffer2) == 0)
      printf ("T%02d wrote block %d\n", task_id(), block_orig) ;
    else
      printf ("T%02d error writing block %d\n", task_id(), block_orig) ;

    // define os proximos blocos
    block_orig++ ;
    block_dest-- ;
  }
  printf ("T%d finished\n", task_id()) ;
  free (buffer1) ;
  free (buffer2) ;
  task_exit (0) ;
}

int main (int argc, char *argv[])
{
  long i ;

  printf ("Main INICIO\n") ;

  // inicializa o sistema operacional
  pingpong_init () ;

  // inicializa o gerente de disco
  if (diskdriver_init (&numBlocks, &blockSize) < 0)
  {
    printf ("Erro na abertura do disco\n") ;
    exit (1) ;
  }
  else
    printf ("Disco contem %d blocos de %d bytes cada\n",
            numBlocks, blockSize) ;

  // cria as tarefas
  for (i = 0; i < NUMTASKS; i++)
   task_create (&mover[i], moverBody, (long*) i) ;

  // aguarda todas as tarefas encerrarem
  for (i = 0; i < NUMTASKS; i++)
   task_join (&mover[i]) ;

  // encerra a thread main
  printf ("Main FIM\n") ;
  task_exit (0) ;

  exit(0) ;
}
