/*
 * menu.h
 *
 *  Created on: 28 may 2022
 *      Author: iness
 */

#ifndef MENU_MENU_H_
#define MENU_MENU_H_

#include "../structures/concierto/concierto.h"
#include "../structures/cliente/cliente.h"
#include "../structures/entrada/entrada.h"
#include "../structures/puesto/puesto.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu(sqlite3 *db, Cartelera *pCart, ListaEntradas lEntradas);
void menuPlan(sqlite3 *db, Cartelera *pCart);
int elegirOpcion();
int costes();
int beneficio(sqlite3 *db, ListaEntradas l);
int ingresos(ListaEntradas pEntradas);

#endif /* MENU_MENU_H_ */
