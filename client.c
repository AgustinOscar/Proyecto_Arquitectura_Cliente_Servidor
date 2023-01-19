//Strings y símbolos estándar.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Errores.
#include <errno.h>

//Sockets.
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>

//Tiempo.
#include <time.h>
#include <fcntl.h>
#include <limits.h>


#define MAXDATASIZE 2048
#define MAXDATASIZE_RESP 20000

int main(int argc, char *argv[]) {
    
    if (argc != 3){ //Error en los parámetros de entrada.
        printf("[CLIENT-ERROR]: Debes ingresar los siguientes parámetros:\n");
        printf("1. IP del servidor.\n");
        printf("2. Puerto de conexión.\n\n");
        printf("Intente nuevamente... Fin del programa.\n");
        exit(1);
    }

    int sock_fd; //File descriptor del socket que utilizará el cliente.
    struct sockaddr_in serveraddr; //Información de la dirección de destino (servidor).  
    struct hostent *he;
    
    //Estos 2 son para el comando.
    char buf_command[MAXDATASIZE];
    int len_command;
    
    //Estos 2 son para la respuesta.
    char buf_response[MAXDATASIZE_RESP];
    int len_response;

    if ((he=gethostbyname(argv[1])) == NULL) { //Obtener información de host servidor.
        perror("gethostbyname");
        exit(1);
    }

    //Creación del socket.
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1) { //Se crea el socket de conexión.
        perror("socket");
        exit(1);
    }

    else {
        printf("[CLIENT]: Socket creado con satisfacción.\n");
    }

    //Asignamos IP y puerto al socket.
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;                       // Ordenación de bytes de la máquina 
    serveraddr.sin_port = htons( atoi(argv[2]) );          // short, Ordenación de bytes de la red 
    serveraddr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(serveraddr.sin_zero), '\0',8);                // poner a cero el resto de la estructura

    //Intentando conectar el socket cliente con el del servidor.
    if (connect(sock_fd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)) != 0) {
        perror("connect");
        exit(1);
    }

    printf("Conectado al servidor...\n");

    while( strcmp(buf_command,"salir\n" ) != 0 ) {
        bzero(buf_command, strlen(buf_command));
        bzero(buf_response, len_response);
        printf("\ncommand> ");
        fgets(buf_command, LINE_MAX, stdin);
        printf("El mensaje al servidor es %s\n", buf_command);
        send( sock_fd, buf_command, strlen(buf_command), 0 );

        if (strcmp(buf_command,"salir\n" ) != 0 ){
            len_response = recv ( sock_fd, buf_response, sizeof(buf_response), 0 );
            printf("Cantidad de bytes leidos=%d\n",len_response);
            printf("La respuesta del servidor es: %s\n", buf_response);
        }
    }
    close(sock_fd);
    exit(0);
}


















// /* Se lee el comando del teclado */
// fgets(comando,MAXDATASIZE-1,stdin);
// /* Se deja el ultimo caracter vacio */
// len_comando = strlen(comando) - 1;
// comando[len_comando] = '\0';
// /* Se imprime el comando que ingreso el cliente */
// printf("Comando: %s\n",comando);

// /* Se envia el comando al server */
// if(send(sockfd,comando, len_comando, 0) == -1) {
// perror("send()");
// exit(1);
// } else 
// printf("Comando enviado...\n");

// // Si el send no devuelve error continua y lo que falta por hacer
// // es leer la respuesta
// if ((numbytes=recv(sockfd, buf, MAXDATASIZE_RESP-1, 0)) == -1) {
// perror("recv");
// exit(1);
// }
// buf[numbytes] = '\0';
// printf("Recibido:\n%s\n",buf);

// // Si el recv no devuelve error continua y lo que falta por hacer
// // es cerrar el file descriptor del cliente
// close(sockfd);

// return 0;




// /* Test sequences */
// char buf_tx[] = "Hello server. I am a client";      
// char buf_rx[100];                     /* receive buffer */
 
 
// /* This clients connects, sends a text and disconnects */
// int main() 
// { 
//     int sockfd; 
//     struct sockaddr_in servaddr; 
    
//     /* Socket creation */
//     sockfd = socket(AF_INET, SOCK_STREAM, 0); 
//     if (sockfd == -1) 
//     { 
//         printf("CLIENT: socket creation failed...\n"); 
//         return -1;  
//     } 
//     else
//     {
//         printf("CLIENT: Socket successfully created..\n"); 
//     }
    
    
//     memset(&servaddr, 0, sizeof(servaddr));

//     /* assign IP, PORT */
//     servaddr.sin_family = AF_INET; 
//     servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS ); 
//     servaddr.sin_port = htons(PORT); 
  
//     /* try to connect the client socket to server socket */
//     if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
//     { 
//         printf("connection with the server failed...\n");  
//         return -1;
//     } 
    
//     printf("connected to the server..\n"); 
  
//     /* send test sequences*/
//     write(sockfd, buf_tx, sizeof(buf_tx));     
//     read(sockfd, buf_rx, sizeof(buf_rx));
//     printf("CLIENT:Received: %s \n", buf_rx);
   
       
//     /* close the socket */
//     close(sockfd); 
// } 