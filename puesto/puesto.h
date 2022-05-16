#ifndef PUESTO_PUESTO_H_
#define PUESTO_PUESTO_H_

#include "../sqlite3/sqlite3.h"
#include "../logger/logger.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PUESTOS 20

typedef struct
{
	int cod;
	char *marca;
	int coste;

} Puesto;

int pedirCodigoPuesto();

void pedirDatosPuesto(sqlite3 *db, Puesto *p);

void insertarPuesto(sqlite3 *db, Puesto *p);

void eliminarPuesto(sqlite3 *db, int cod);

void imprimirPuesto(sqlite3 *db);

int costesPuestos(sqlite3 *db);

#endif /* PUESTO_PUESTO_H_ */
