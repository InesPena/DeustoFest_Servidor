#include "manager/properties/properties.h"
#include "manager/sqlite3/sqlite3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "structures/concierto/concierto.h"
#include "structures/cliente/cliente.h"
#include "structures/entrada/entrada.h"
#include "structures/puesto/puesto.h"
#include "menu/menu.h"

sqlite3 *db;
Cartelera *pCart;
Cartelera cart;
ListaEntradas lEntradas;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET conn_socket; //el que lleva la conexion
	SOCKET comm_socket; //el que lo comunica
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512]; // lo que yo envio, lo que yo recibo
	sqlite3 *db;
	char opcion, conCamping, conBus;
	Cartelera cart;
	int i, precio;
	char dni[10], nom[20], email[50];
	sqlite3_open("sqlite3/deustoFest.sqlite", &db);
	ListaEntradas le;

	printf("\nInitialising Winsock...\n"); // inicializa la libreria
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation creacion del socket( la primera estructura
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");
	// cual es la ip y cual es el puerto
	server.sin_addr.s_addr = inet_addr(SERVER_IP); //INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n"); //DEJAR EL SOCKET EN ESPERA

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);
	int fin = 0;

	do {
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		sscanf(recvBuff, "%c", &opcion);
		switch (opcion) {
		case '1':
			obtenerCartelera(db, &cart);
			sprintf(sendBuff, "%d", cart.numConciertos);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			for (i = 0; i < cart.numConciertos; i++) {
				sprintf(sendBuff, "%d;%s;%d;%d;%d", cart.conciertos[i].cod,
						cart.conciertos[i].artista,
						cart.conciertos[i].escenario, cart.conciertos[i].dia,
						cart.conciertos[i].coste);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			break;
		case '2':
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%c", &conCamping);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%c", &conBus);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%d", &precio);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sprintf(dni, "%s", recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sprintf(nom, "%s", recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sprintf(email, "%s", recvBuff);
			Entrada *pEnt;
			Entrada ent;
			pEnt = &ent;
			pEnt->camping = conCamping;
			pEnt->bus = conBus;
			strcpy(pEnt->dni, dni);
			pEnt->precio = precio;
			insertEntrada(db, pEnt);

			Cliente cl;
			Cliente *pcl = &cl;
			strcpy(pcl->dni, dni);
			strcpy(pcl->nombre, nom);
			strcpy(pcl->mail, email);
			insertCliente(db, pcl);
			break;
		case '3':
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sprintf(dni, "%s", recvBuff);
			obtenerEntradas(db, &le);
			printf("Número de entradas: %d\n", le.numEntradas);
			fflush(stdout);
			eliminarEntrada(db, &le, dni);
			sprintf(sendBuff, "Entrada eliminada correctamente");
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			break;
		case '4':
			printf("OPCION : %c\n", opcion);
			fflush(stdout);
			break;
		}
	} while (opcion != '4');

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

	return 0;
	//-----------------------------------------------------------
}

