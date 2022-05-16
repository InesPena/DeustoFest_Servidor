#ifndef ENTRADA_ENTRADA_H_
#define ENTRADA_ENTRADA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sqlite3/sqlite3.h"
#include "../logger/logger.h"
#include "../cliente/cliente.h"

#define MAX_ENTRADAS 1000

typedef struct {
	int cod;
	char dni[10];
	int camping;
	int bus;
	int precio;

} Entrada;

typedef struct {
	int numEntradas;
	Entrada entradas[MAX_ENTRADAS];

} ListaEntradas;

void imprimirEntrada(Entrada e);

void compraEntradas(Entrada *e, Cliente *c);

void insertEntrada(sqlite3 *db, Entrada *e);

int calularPrecio(Entrada *e, int precioEnt);

float porcentajeAsistencia(sqlite3 *db);

void obtenerEntradas(sqlite3 *db, ListaEntradas *le);


#endif /* ENTRADA_ENTRADA_H_ */
