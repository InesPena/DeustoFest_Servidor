#include "entrada/entrada.h"
#include "cliente/cliente.h"
#include "concierto/concierto.h"
#include "puesto/puesto.h"

#include "properties/properties.h"
#include "logger/logger.h"
#include "sqlite3/sqlite3.h"
#include "sockets/sockets.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu();
void menuPlan();

int elegirOpcion();
int costes();

int beneficio(sqlite3 *db, ListaEntradas l);
int ingresos(ListaEntradas pEntradas);

sqlite3 *db;

Cartelera *pCart;
Cartelera cart;

ListaEntradas lEntradas;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

SOCKET s;
char sendBuff[512];
char recvBuff[512];

/*
 * MAIN
 */

int main(int argc, char *argv[])
{
	sqlite3_open("sqlite3/deustoFest.sqlite", &db);
	establecerConexion(s, sendBuff, recvBuff);

	pCart = &cart;

	menu();
}

/*
 * MENU PRINCIPAL
 */

void menu()
{
	int op;

	Cliente *pCl;
	Cliente cl;
	pCl = &cl;

	do {
		obtenerEntradas(db, &lEntradas);
		obtenerCartelera(db, pCart);

		printf("\n\n\tADMINISTRADOR\n");
		printf("--------------------------------\n\n");
		printf("1. Planificar Festival\n");
		printf("2. Consultar programa\n");
		printf("3. Ver lista de puestos de comida/bebida\n");
		printf("4. Consultar datos de cliente\n");
		printf("5. Ver Estadísticas\n");
		printf("6. Volver atrás\n");

		op = elegirOpcion();

	    switch (op) {

	     	 case 1:
	     		menuPlan();
	     		break;

	         case 2:
	        	 imprimirCartelera(db, pCart, 2);
	        	 break;


	         case 3:
	        	 imprimirPuesto(db);
	             break;

	         case 4:
	        	 printf("\n\nInserte el dni del cliente...\n\n");
	        	 consultarDatosCliente(db, pCl);
	        	 break;

	         case 5:
	        	 printf("\n\tESTADÍSTICAS\n");
	        	 printf("--------------------------------\n\n");
	        	 printf("Asistencia = \t\t%.2f%% \n", porcentajeAsistencia(db));
	        	 printf("Ingreso total = \t%i\n", ingresos(lEntradas));
	        	 printf("Coste total = \t\t%i\n", costes(db));
	        	 printf("Beneficio total = \t%i\n", beneficio(db, lEntradas));
	        	 break;

	         case 6:
	        	 free(pCl->nombre);
	        	 free(pCl->mail);
	        	 free(pCl);
	         	 break;

	         default:
	        	 printf("\n¡ERROR! La opción seleccionada no existe\n");
	        	 break;
	    }

	} while (op != 6);
}

/**
 * FUNCIONES DE PLANIFICACIÓN
 */

void menuPlan()
{
	int op;

	Concierto *pCon;
	Concierto con;
	pCon = &con;

	Puesto *pPu;
	Puesto pu;
	pPu = &pu;

	do {

		printf("\n\tPLANIFICAR FESTIVAL\n");
		printf("--------------------------------\n\n");
		printf("1. Organizar nuevo concierto\n");
		printf("2. Cancelar concierto\n");
		printf("3. Añadir puesto\n");
		printf("4. Eliminar puesto\n");
		printf("5. Volver atrás\n");

		op = elegirOpcion();

		switch (op) {

			case 1:
				printf("\nInserte los siguientes datos...\n\n");
				pedirDatosConcierto(db, pCon);
				insertarConcierto(db, pCon);
				break;

		    case 2:
		    	obtenerCartelera(db, &cart);
		    	printf("\nInserte el código del concierto...\n\n");
		    	eliminarConcierto(db, pedirCodigoConcierto(cart));
		    	break;

		    case 3:
		    	printf("\nInserte los siguientes datos...\n\n");
		    	pedirDatosPuesto(db, pPu);
		    	insertarPuesto(db, pPu);
		    	break;

		    case 4:
		    	printf("\nInserte el código del puesto...\n");
		    	imprimirPuesto(db);
		    	eliminarPuesto(db, pedirCodigoPuesto());
		    	break;

		    case 5:
		    	free(pCon->artista);
		    	free(pCon);
		    	free(pPu->marca);
		    	free(pPu);
		    	break;

		    default:
		    	printf("\n¡ERROR! La opción seleccionada no existe\n");
		    	break;
		}

	} while (op != 5);
}

/*
 * OTRAS FUNCIONES
 */

int elegirOpcion()
{
	int op;

	printf("\nOpción: ");
	fflush(stdout);
	scanf("%i", &op);
	getchar();

	return op;
}

int ingresos(ListaEntradas pEntradas)
{
	int ingresos=0;

	for(int i=0; i<pEntradas.numEntradas;i++){
		ingresos += pEntradas.entradas[i].precio;
	}

	return ingresos;
}

int costes(sqlite3 *db)
{
	return costesConciertos(db) - costesPuestos(db);
}

int beneficio(sqlite3 *db, ListaEntradas l)
{
	return ingresos(l) - costes(db);
}
