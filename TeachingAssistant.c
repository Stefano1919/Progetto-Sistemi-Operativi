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

	int i;
	int numeroStudenti;

	if (argc > 1 ) {
		if ( isNumero( argv[1] ) == 1) {
			numeroStudenti = atoi( argv[1] );
		}
		else {
			printf("ERRORE!! Input non valido");
			return 0;
		}
	}
	else { numeroStudenti = NUMERO_STUDENTI;
	}

	int idStudentes[numeroStudenti];
	pthread_t students[numeroStudenti];
	pthread_t ta;

	sem_init( &semStudenti, 0, 0 );
	sem_init( &semAssistente, 0, 1 );

	
	pthread_mutex_init( &mutexThread, NULL );
	pthread_create( &ta, NULL, azioniAssistente, NULL );
	for( i = 0; i < numeroStudenti; i++ ) {
		idStudentes[i] = i + 1;
		pthread_create( &students[i], NULL, azioniStudente, (void*) &idStudentes[i] );
	}
	pthread_join(ta, NULL);
	for( i =0; i < numeroStudenti; i++ ) {
		pthread_join( students[i],NULL );
	}
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

void* azioniStudente( void* idStudente ) {
	int id_studente = *(int*)idStudente;
	while( 1 ) {
		if ( isInAttesa( id_studente ) == 1 ) { continue; }
		int time = rand() % 5;
		printf( "\tLo studente %d sta programmando per %d secondi.\n", id_studente, time );
		sleep( time );
		pthread_mutex_lock( &mutexThread );
		if( numeroStudentiInAttesa < NUMERO_SEDIE_ATTESA ) {
			sediaAttesa[nuovaPosizione] = id_studente;
			numeroStudentiInAttesa++;
			printf( "\t Lo studente %d prende posto. Lo studente aspetta = %d.\n", id_studente, numeroStudentiInAttesa );
			nuovaPosizione = ( nuovaPosizione + 1 ) % NUMERO_SEDIE_ATTESA;
			pthread_mutex_unlock( &mutexThread );
			sem_post( &semStudenti );
			sem_wait( &semAssistente );
}
		else { 
			pthread_mutex_unlock( &mutexThread );
			printf( "Lo studente %d aspetterà\n",id_studente );
		}
	}
}

int isNumero(char number[]){
	int i;
	for ( i = 0 ; number[i] != 0; i++ ) {
        if (!isdigit(number[i])){
            return 0;
	}
    }
    return 1;
}

int isInAttesa( int idStudente ) {
	int i;
	for ( i = 0; i < 3; i++ ) {
		if ( sediaAttesa[i] == idStudente ) { return 1; }
	}
	return 0;
}
