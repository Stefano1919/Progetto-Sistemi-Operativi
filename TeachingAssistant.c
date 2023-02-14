#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void* azioniStudente( void* idStudente );
void* azioniAssistente();

#define NUMERO_SEDIE_ATTESA 3
#define NUMERO_STUDENTI 5

sem_t semStudenti;
sem_t semAssistente;
pthread_mutex_t mutexThread;

int sediaAttesa[3];
int numeroStudentiInAttesa = 0;
int nuovaPosizione = 0;
int posizioneTaSuccessiva = 0;
int taDormeFlag = 0;
