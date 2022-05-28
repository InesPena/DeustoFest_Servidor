#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

#define MAX_LOG 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef enum
{
	INFO,
	ERROR_

} TIPO;

void log(char msg[MAX_LOG], TIPO tipo);

char* now();

#endif /* LOGGER_LOGGER_H_ */
