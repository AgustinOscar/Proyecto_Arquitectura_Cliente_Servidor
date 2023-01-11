/*
    DATOS DEL ALUMNO:
        Nombre: Reyes González Agustín Óscar.
        No. Cuenta: 417095215.
    
    DATOS DE LA ASIGNATURA:
        Asignatura: Arquitectura Cliente Servidor.
        Profesor: Ing. Carlos Alberto Román Zamitiz.
        Semestre: 2023-1.
        Grupo: 2.
*/

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

//Parámetros del servidor.
#define SERV_PORT 8080               //Puerto del servidor.
#define SERV_HOST_ADDR "127.0.0.1"   //IP del servidor.
#define BUF_SIZE 100                 //Tamaño del buffer.
#define BACKLOG 5                    //Número máximo de conexiones permitidas.
#define QLEN 2


//Función principal.
int main(int argc, char *argv[]){
    /*
    Necesitamos 2 filedescriptor ya que tendremos 2
    estructuras sockaddr_in (una para el servidor y 
    otra para la conexión del cliente).
    */
    int server_fd, cliente_fd;

    /*
    Se crean las dos estructuras necesarias.
    La primera llamada servidor que se le asociará el server_fd.
    La segunda llamada cliente que se le asociará el cliente_fd.
    */
    struct sockaddr_in servidor;    //Información sobre mi direccion (servidor).
    struct sockaddr_in cliente;     //Información sobre la dirección del cliente.
    struct hostent* info_cliente;   //Información del nodo remoto.

    //Tamaño del servidor y del cliente.
    int sin_size_servidor;
    int sin_size_cliente;

    //Longitud de la dirección del cliente.
    int longClient;

    //Se crea un socket (TCP/IPv4).
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_fd == -1){
        puts("Error al crear socket.");
        exit(1);
    }

    else{
        puts("Socket creado de manera exitosa.");
    }
    
    memset((char *) &servidor, 0, sizeof(servidor)); //Se limpia información de la estructura del servidor.
    servidor.sin_family = AF_INET; //Asignamos tipo de IP (Familia).
    servidor.sin_addr.s_addr = INADDR_ANY; //Asignamos dirección IP del servidor.
    servidor.sin_port = htons((u_short) atoi(argv[1])); //Asignamos el puerto convirtiendo el string de entrada en el formato necesario.
    memset(&(servidor.sin_zero), '\0', 8);

    //Asignamos la dirección IP y puerto al socket.
    if ((bind(server_fd, (struct sockaddr *) &servidor, sizeof(servidor))) != 0 ){
        fprintf(stderr, "[Servidor-Error]: Falló asifnación al socket.\n");
        return -1;
    }

    //El socket se establece en modo pasivo (en escucha).
    listen(server_fd, QLEN);

    longClient = sizeof(cliente);

    while (1){
        break;
    }


    close(server_fd);
    shutdown(server_fd, SHUT_RDWR);
    puts("Fin del programa...");
    exit(0);
}

//   // while(1)
//   {
//     fd_c = accept(fd_s, (struct sockaddr *) &cliente, &longClient);

//     info_cliente = gethostbyaddr((char *) &cliente.sin_addr, sizeof(struct in_addr), AF_INET);
//     /* Lo siguiente es para obtener la fecha y hora y se impriman en pantalla */
//     time_t T = time(NULL);
//     struct tm tm = *localtime(&T);
//     printf("%02d/%02d/%04d %02d:%02d:%02d-Cliente conectado ", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
//     printf(" desde: %s\n\n", info_cliente -> h_name);

//     if( fork() == 0 )
//     {
//       char buf_peticion[256];
//       char buf_respuesta[256];
//       /* el hijo no necesita el file descriptor del socket servidor,
//       porque el no aceptara conexion de cliente, solo se comunica con el por lo que
//       solo necesita el file descriptor del socket cliente */
//       close( fd_s );
//       // printf("0. buf_peticion=%s\n", buf_peticion);
//       do
//       {
//         int n = recv(fd_c, buf_peticion, sizeof(buf_peticion), 0);
//         // printf("2. longitud de buf_peticion=%d\n", strlen(buf_peticion));
//         printf("3. El mensaje del cliente es %s\n", buf_peticion);
//         buf_peticion[n] = '\0';
//         // printf("strcmp(buf_peticion,\"adios\")=%d\n", strcmp(buf_peticion,"adios"));

//         fgets(buf_respuesta, LINE_MAX, stdin);
//         printf("El mensaje para cliente es %s\n", buf_respuesta);
//         send( fd_c, buf_respuesta, strlen(buf_respuesta), 0 );
//       }
//       while( strcmp(buf_peticion,"adios\n") != 0 );
//       printf("Se ha cerrado la conexion con el cliente... Este hijo termina\n");
//       close(fd_c);
//       exit( 0 );
//     }
//     else
//     {
//       /* El padre no necesita el file descriptor del socket cliente 
//       porque el no se hablara con los clientes, los que lo haran son los hijos.
//       Lo que si necesita el padre es mantener abierto el file descriptor del
//       socket servidor porque en el sig ciclo del while llamara a la funcion
//       accept() para aceptar a otro cliente */
//       close( fd_c );
//     }
//   }
//   close(fd_s);
//   shutdown( fd_s, SHUT_RDWR );
//   exit(0);
// }