
#ifndef PROPERTIES_PROPERTIES_H_
#define PROPERTIES_PROPERTIES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sqlite3/sqlite3.h"

typedef struct
{
	int num;
	char** clave;
	char** valor;
} Properties;

void crearProperties(Properties* properties);

void cargarProperties(Properties* properties);

void actualizarProperties(Properties* properties);

#endif /* PROPERTIES_PROPERTIES_H_ */
