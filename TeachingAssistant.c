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

int main( int argc, char **argv ){

	return 0;
}

void* azioniAssistente() {

	printf( "Stiamo vedendo gli studenti \n" );

	while( 1 ) {

		if ( numeroStudentiInAttesa > 0 ) {

			taDormeFlag = 0;
			sem_wait( &semStudenti );
			pthread_mutex_lock( &mutexThread );

			int tempoAiuto = rand() % 5;

			printf( "Studente aiutato per %d secondi. Lo studente ha aspettato per %d.\n", tempoAiuto, (numeroStudentiInAttesa - 1) );
			printf( "Lo studente %d sta ricevendo aiuto.\n",sediaAttesa[posizioneTaSuccessiva] );

			sediaAttesa[posizioneTaSuccessiva]=0;
			numeroStudentiInAttesa--;
			posizioneTaSuccessiva = ( posizioneTaSuccessiva + 1 ) % NUMERO_SEDIE_ATTESA;

			sleep( tempoAiuto );

			pthread_mutex_unlock( &mutexThread );
			sem_post( &semAssistente );

		}
		else {

			if ( taDormeFlag == 0 ) {

				printf( "Nessun studente sta aspettando. \n Dormendo...\n" );
				taDormeFlag = 1;

			}

		}

	}

}
