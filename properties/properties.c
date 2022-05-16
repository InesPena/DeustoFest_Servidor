#include "properties.h"
#include "../sqlite3/sqlite3.h"

void crearProperties(Properties *properties)
{
	FILE *f = fopen("properties/file.properties", "w");

	for (int i = 0; i < properties->num; i++) {

		fprintf(f, "%s - ", properties->clave[i]);
		fprintf(f, "%s", properties->valor[i]);
	}

	fclose(f);
}

void cargarProperties(Properties *properties)
{
	FILE *f = fopen("properties/file.properties", "r");

	char buffer[2000];
	char *linea;
	char listaClave[100];

	while (fgets(buffer, 2000, f))
	{
		linea = strtok(buffer, "-");

		for(int i = 0; i < properties->num ; i++)
		{
			if(i % 2 == 0){
					//meter en array clave
					printf( "%s: \n", linea );
					listaClave[i] = linea;
				}
				else{
					//meter en array valor.
					printf( "%s\n", linea );
				}
				linea = strtok(NULL, "-");
			}
	}
	properties->clave = listaClave;
	fclose(f);

}


void actualizarProperties(Properties *properties)
{

	/*
	FILE *f = fopen("properties/file.properties", "w+");

	for (int i = 0; i < properties->num; i++) {

		fprintf(f, "%s: ", properties->clave[i]);
		if(i == 0){
			// fprintf(f, "%s\n", (int)time(NULL));
			fprintf(f, "%s\n", properties->valor[i]);
		}else{
			fprintf(f, "%s\n", properties->valor[i]);
		}

	}

	fclose(f);*/

}


