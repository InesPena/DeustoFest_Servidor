#ifndef CLIENTE_CLIENTE_H_
#define CLIENTE_CLIENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sqlite3/sqlite3.h"
#include "../logger/logger.h"

typedef struct{

	char dni[10];
	char *nombre;
	char *mail;

} Cliente;

void pedirDatosCliente(Cliente *c);

void insertCliente(sqlite3 *db, Cliente *c);

void imprimirClientes(sqlite3 *db);

void consultarDatosCliente(sqlite3 *db, Cliente *c);

#endif
