#include "entrada.h"
#include "../sqlite3/sqlite3.h"

#define PRECIO_CAMP 35
#define PRECIO_BUS 47

/*
 * Calcula el precio de la entrada en función de el tipo de entrada,
 * si se ha elegido bus y si se ha elegido camping
 */

int calularPrecio(Entrada *e, int precioEnt)
{
	int precioTotal = 0;

	precioTotal += precioEnt;
	if (e->camping == 1)  precioTotal += PRECIO_CAMP;
	if (e->bus == 1) precioTotal += PRECIO_BUS;

	return precioTotal;
}

/*
 * Proceso para la compra de entradas
 */

void compraEntradas(Entrada *e, Cliente *c)
{
	int op;
	int precioEnt;

	e->camping = 0;
	e->bus = 0;

	printf("\n\n\tENTRADAS\n");
	printf("--------------------------------\n\n");
	printf("1. Entrada Día 22............75€\n");
	printf("2. Entrada Día 23............80€\n");
	printf("3. Abono completo...........142€\n");

	op = elegirOpcion();

	if (op == 1) precioEnt = 75;
	if (op == 2) precioEnt = 80;
	if (op == 3) precioEnt = 142;

	char op2;
	char op3;

	printf("¿Desa reservar una plaza de camping? (s/n) ");
	fflush(stdout);
	scanf("%c",&op2);
	getchar();
	if (op2 == 's') e->camping = 1;

	printf("¿Desa reservar una plaza de autobus? (s/n) ");
	fflush(stdout);
	scanf("%c",&op3);
	getchar();

	if (op3 == 's') e->bus = 1;

	e->precio = calularPrecio(e, precioEnt);

	printf("\nIntroduzca sus datos personales...\n\n");

	pedirDatosCliente(c);

	strcpy(e->dni, c->dni);
}

/*
 * Inserta una fila en la tabla entrada
 */

void insertEntrada(sqlite3 *db, Entrada *e)
{
	sqlite3_stmt *stmt;
	int result;

	char sql[] = "INSERT INTO ENTRADA(DNI, CAMPING, BUS, PRECIO) VALUES (?, ?, ?, ?)";

	sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

	sqlite3_bind_text(stmt, 1, e->dni, strlen(e->dni), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, e->camping);
	sqlite3_bind_int(stmt, 3, e->bus);
	sqlite3_bind_int(stmt, 4, e->precio);

	result = sqlite3_step(stmt);

	char buffer[100];
	sprintf(buffer, "INSERT INTO ENTRADA(DNI, CAMPING, BUS, PRECIO) VALUES ('%s', %i, %i, %i)", e->dni, e->camping, e->bus, e->precio);

	if (result != SQLITE_DONE){
		log(buffer, ERROR);
		printf("\nError al realizar la compra\n");
	} else {
		log(buffer, INFO);
		printf("\nCompra completada\n");
	}

	sqlite3_finalize(stmt);
}

float porcentajeAsistencia(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	int result;

	char sql[] = "SELECT COUNT(ENTRADA.COD) FROM ENTRADA";

	sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

	result = sqlite3_step(stmt);
	int entradas = sqlite3_column_int(stmt, 0);

	log(sql, INFO);

	sqlite3_finalize(stmt);

	return ((float)entradas/(float)MAX_ENTRADAS)*100;
}

void obtenerEntradas(sqlite3 *db, ListaEntradas *le)
{
	sqlite3_stmt *stmt;
	int result,pos = 0;

	char sql[] = "SELECT COUNT(*) FROM ENTRADA";
	sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
	result = sqlite3_step(stmt);
	le->numEntradas = sqlite3_column_int(stmt,0);
	sqlite3_finalize(stmt);

	char sql2[] = "SELECT * FROM ENTRADA";
	sqlite3_prepare_v2(db,sql2,strlen(sql2),&stmt,NULL);

	do{
		result = sqlite3_step(stmt);

		if(result == SQLITE_ROW){
			le->entradas[pos].cod = sqlite3_column_int(stmt, 0);
			strcpy(le->entradas[pos].dni, (char*)sqlite3_column_text(stmt,1));
			le->entradas[pos].camping = sqlite3_column_int(stmt,2);
			le->entradas[pos].bus = sqlite3_column_int(stmt,3);
			le->entradas[pos].precio = sqlite3_column_int(stmt,4);
			pos++;
		}

	}while(result == SQLITE_ROW);

	log(sql2, INFO);

	sqlite3_finalize(stmt);
}
