#ifndef CONCIERTO_CONCIERTO_H_
#define CONCIERTO_CONCIERTO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sqlite3/sqlite3.h"
#include "../logger/logger.h"

typedef struct {
	int cod;
	char *artista;
	int escenario;
	int dia;
	int coste;

} Concierto ;


void pedirDatosConcierto(sqlite3 *db, Concierto *c);

void insertarConcierto(sqlite3 *db, Concierto *c);

void eliminarConcierto(sqlite3 *db, int cod);

int costesConciertos(sqlite3 *db);

typedef struct {
	int numConciertos;
	Concierto *conciertos;

}Cartelera;

int pedirCodigoConcierto(Cartelera cat);

void obtenerCartelera(sqlite3 *db, Cartelera *cart);

void imprimirCartelera(sqlite3 *bd,Cartelera *cart, int op);

#endif /* CONCIERTO_CONCIERTO_H_ */
