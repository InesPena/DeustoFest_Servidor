#include "sockets.h"
#include "../entrada/entrada.h"
#include "../cliente/cliente.h"

// #define _WIN32_WINNT 0x0501
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6001

#include <winsock2.h>

void establecerConexion(SOCKET comm_socket, char sendBuff[], char recvBuff[])
{
	WSADATA wsaData;
	SOCKET conn_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		//return -1;
	}
	printf("Initialised\n");

	/* CREAR SOCKET */

	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		//return -1;
	}
	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	/* BIND */

	if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		//return -1;
	}
	printf("Bind done.\n");

	/* ESCUCHAR */

	if (listen(conn_socket, 1) == SOCKET_ERROR)
	{
		printf("Listen failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		//return -1;
	}

	/* ACEPTAR */

	printf("Waiting for incoming connections...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

	if (comm_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		//return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

	closesocket(conn_socket);
}

void protocoloServidor(SOCKET comm_socket, char sendBuff[], char recvBuff[])
{
	recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

	do {
		if (strcmp(recvBuff, "COMANDO 1") == 0)
		{

		}

		if (strcmp(recvBuff, "COMPRA") == 0)
		{
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

			int camp = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int bus = atoi (recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precio= atoi(recvBuff);
			int dni = to_string(recvBuff);
			int nom = recvBuff;
			int email = recvBuff;

			Entrada *pEnt;
			Entrada ent;
			pEnt = &ent;
			pEnt->camping = camp;
			pEnt->bus = bus;
			pEnt->dni = dni;
			pEnt->precio = precio;
			insertEntrada(db, pEnt);

			Cliente *pCl;
			Cliente cl;
			pCl = &cl;
			pCl->dni = dni;
			pCl->nombre = nom;
			pCl->mail = email;
			insertCliente(db,pCl);

			sprintf(sendBuff, "%s", "Compra completada");
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
		}

		if (strcmp(recvBuff, "COMANDO 3") == 0)
		{

		}

	} while (1);

	closesocket(comm_socket);
	WSACleanup();
}


