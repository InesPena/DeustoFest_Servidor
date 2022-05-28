#include "logger.h"

void log(char msg[MAX_LOG], TIPO tipo)
{
	FILE *file;
	file = fopen("logger/deustoFest.log", "a+");

	fprintf(file, "\n%s", now());

	if (tipo == INFO) fprintf(file, "INFO: ");
	if (tipo == ERRO) fprintf(file, "ERROR: ");

	fprintf(file, msg);

	fclose(file);
}

/*
 * Devuelve un string con la fecha y hora actual
 */

char* now()
{
	time_t t;
	char *ch ;
	time(&t);
	ch = ctime(&t);

	return ch;
}



