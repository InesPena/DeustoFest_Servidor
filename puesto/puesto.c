#include "puesto.h"

#define MAX_LINE 20

/*
 * Pide el código de un puesto por consola
 */

int pedirCodigoPuesto()
{
	int cod;
	char str[MAX_LINE];

	printf("Cod. Puesto: ");
	fflush(stdout);
	fgets(str, MAX_LINE, stdin);
	sscanf(str, "%i", &cod);

	return cod;
}

/*
 *Pide los datos relativos a un puesto por consola
 */

void pedirDatosPuesto(sqlite3 *db, Puesto *p)
{
	char str[MAX_LINE];

	printf("Nombre de la marca: ");
	fflush(stdout);
	fgets(str, MAX_LINE, stdin);
	sscanf(str, "%s", str);
	p->marca = malloc((strlen(str) + 1)*sizeof(char));
	strcpy(p->marca, str);

	printf("Coste del puesto: ");
	fflush(stdout);
	fgets(str, MAX_LINE, stdin);
	sscanf(str, "%i", &p->coste);
}

/*
 * Inserta una nueva fila en la tabla puesto de la bd
 */

void insertarPuesto(sqlite3 *db, Puesto *p)
{
	sqlite3_stmt *stmt;
	int result;

	char sql[] = "INSERT INTO PUESTO(MARCA, COSTE) VALUES (?,?)";

	sqlite3_prepare_v2(db, sql,strlen(sql) + 1, &stmt, NULL);

	sqlite3_bind_text(stmt, 1, p->marca, strlen(p->marca), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, p->coste);

	result = sqlite3_step(stmt);

	char buffer[100];
	sprintf(buffer, "INSERTAR INTO PUESTO(MARCA, COSTE) VALUES ('%s', %i)", p->marca, p->coste);

	if(result != SQLITE_DONE){
		log(buffer, ERROR);
		printf("\nError añadiendo puesto\n");
	}else{
		log(buffer, INFO);
		printf("\nPuesto añadido correctamente\n");
	}

	sqlite3_finalize(stmt);
}

/*
 * Elimina uns puesto de la bd
 */

void eliminarPuesto(sqlite3 *db, int cod)
{

	sqlite3_stmt *stmt;
	int result;

	char sql[] = "DELETE FROM PUESTO WHERE COD = ?";

	sqlite3_prepare_v2(db, sql,	strlen(sql), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, cod);

	result = sqlite3_step(stmt);

	char buffer[100];
	sprintf(buffer, "DELETE FROM PUESTO WHERE COD = %i", cod);

	if(result != SQLITE_DONE){
		log(buffer, ERROR);
		printf("\nError eliminando puesto\n");
	}else{
		log(buffer, INFO);
		printf("\nPuesto eliminado correctamente\n");
	}

	sqlite3_finalize(stmt);
}

void imprimirPuesto(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	int result;

	char sql[] = "SELECT * FROM PUESTO";
	sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);

	printf("\n\tPUESTOS\n");
	do{
		result = sqlite3_step(stmt);
		if(result == SQLITE_ROW){
			printf("\t%i - %s - %i\n", sqlite3_column_int(stmt,0), (char*)sqlite3_column_text(stmt,1), sqlite3_column_int(stmt,2));
		}
	}while(result == SQLITE_ROW);

	printf("\n");

	log(sql, INFO);

	sqlite3_finalize(stmt);
}

int costesPuestos(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	int result;
	int coste;

	char sql[] = "SELECT SUM(COSTE) FROM PUESTO";
	sqlite3_prepare_v2(db, sql, strlen(sql)+1, &stmt, NULL);

	result = sqlite3_step(stmt);

	if(result == SQLITE_ROW){
		coste = sqlite3_column_int(stmt, 0);
	}

	log(sql, INFO);

	sqlite3_finalize(stmt);

	return coste;
}
