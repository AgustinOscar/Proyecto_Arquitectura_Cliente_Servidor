/*
    DATOS DEL ALUMNO:
        Nombre: Reyes González Agustín Óscar.
        No. Cuenta: 417095215.
    
    DATOS DE LA ASIGNATURA:
        Asignatura: Arquitectura Cliente Servidor.
        Profesor: Ing. Carlos Alberto Román Zamitiz.
        Semestre: 2023-1.
        Grupo: 1.
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
#define NUM_CLIENTS 2    //Número de clientes que puede aceptar el servidor. (BACKLOG)
#define BUF_RECV    100  //Cantidad de bytes que se pueden recibir.
#define BUF_SEND    2048 //Cantidad de bytes que se pueden envíar.

//Función principal.
int main(int argc, char *argv[]){
    
    if ( argc != 2 ){ //Error.
        printf("Debe ingresar el puerto por el cuál se ejecutará el socket del servidor.\n");
        exit(0);
    }

    //File Descriptors.
    int server_fd; //File descriptor del socket que abre la conexión (Servidor).
    int client_fd; /*File descriptor del socket que se crea cuando un cliente se conecta. (Cliente).
                     Con este fd podemos leer y envíar datos al cliente.*/

    unsigned int len_client; /*Es un valor entero que se usará para guardar la longitud de la
                               estructura del 'sockaddr_in' del cliente. */

    /*
        Se crean las dos estructuras necesarias:
        * La primera llamada servidor que se le asociará el server_fd.
        * La segunda llamada cliente que se le asociará el client_fd.
    */
    struct sockaddr_in server;   //Información sobre mi direccion (servidor).
    struct sockaddr_in client;   //Información sobre la dirección del cliente.
    struct hostent* info_client; //Información del nodo remoto.

    //Datos de envío y recepción.
    int len_recv = 0; //Longitud de bytes recibidos.
    int len_send = 0; //Longitus de bytes transmitidos.

    char buff_recv[BUF_RECV]; //Bytes que lee el servidor por parte del cliente.
    char buff_send[BUF_SEND]; //Bytes que envía el servidor al cliente.

    //Bandera de comunicación.
    int flag_com = 1;

    //Creación del socket servidor.
    server_fd = socket(AF_INET, SOCK_STREAM, 0); //Args: Dominio, Tipo de socket, Protocolo (0 es el de TCP).

    if (server_fd == -1) {
        printf("[SERVER-ERROR]: No se pudo crear el socket.\n");
        exit(1);
    }

    else {
        printf("[SERVER]: Éxito, se creo el socket de manera satisfactoria.\n");
    }

    memset((char *) &server, 0, sizeof(server));      //Limpiamos la estructura del sockaddr_in.
    server.sin_family = AF_INET;                      //Asignamos tipo de IP (Familia).
    server.sin_addr.s_addr = INADDR_ANY;              //Asignamos dirección IP del servidor (cualquiera).
    server.sin_port = htons((u_short) atoi(argv[1])); //Asignamos el puerto convirtiendo el string de entrada en el formato necesario.
    memset(&(server.sin_zero), '\0', 8);

    //Asignamos la dirección IP y puerto al socket.
    if ((bind(server_fd, (struct sockaddr *) &server, sizeof(server))) != 0 ){
        fprintf(stderr, "[SERVER-ERROR]: Falló asignación al socket.\n");
        exit(1);
    }

    else {
        printf("[SERVER]: La asignación de IP y puerto se realizó de manera existosa.\n");
    }

    //El socket se establece en modo pasivo (en escucha).
    if (listen(server_fd, NUM_CLIENTS) != 0) {
        printf("[SERVER-ERROR]: Error en el listen del socket.\n");
        exit(1);
    }

    else {
        printf("[SERVER]: Socket a la escucha por el puerto: %d\n", ntohs(server.sin_port));
    }

    //Obtenemos la cantidad de bytes de la estructura cliente.
    len_client = sizeof(client);

    //Loop para aceptar conexiones de clientes.
    while(1) {

        puts("Servidor esperando una nueva conexión...\n");

        //Se acepa una nueva conexión.
        client_fd = accept(server_fd, (struct sockaddr *) &client, &len_client);

        if(client_fd < 0){
            printf("[SERVER-ERROR]: Conexión no aceptada.\n");
            exit(1);
        }

        else{

            puts("Nueva conexión recibida...\n");

            do {
                //Se lee el mensaje del cliente contenido en el buffer.
                len_recv = recv(client_fd, buff_recv, sizeof(buff_recv), 0);

                if(len_recv == -1){ //Error al leer comando.
                    printf("[SERVER-ERROR]: El búfer no pudo ser leído.\n");
                    close(client_fd);
                    //exit(1);
                }

                else { //Lectura de comando exitosa.
                    printf("[SERVER]: El comando leído es %s \n", buff_recv);
                    printf("La cantidad de bytes leídos es: %d\n", len_recv);
                    buff_recv[len_recv] = '\0';

                    if (strcmp(buff_recv,"salir\n") == 0) {
                        puts("Se ingresó la palabra clave de despedida.\n");
                        //close(client_fd);
                        //exit(0);
                        break;
                    }

                    /*
                        En este espacio se creará las funciones fork, pipe,
                        switch (en caso de ser necesario), etc.

                        La idea general es:
                            1. Crear un nuevo proceso con fork.
                            2. El proceso hijo se encargará de realizar
                               la ejecución del comando y enviar el
                               resultado a su padre por medio de un archivo.
                                2.1 Se ejecuta la función exec ().
                                2.2 Se almacena el resultado en un archivo.
                                2.3 El resultado se le envía al padre.
                            3. El padre envía el archivo al cliente.
                    */

                    printf("Ingresa la respuesta para el cliente: ");
                    fgets(buff_send, LINE_MAX, stdin);
                    printf("La respuesta para el cliente es: %s\n", buff_send);
                    send( client_fd, buff_send, strlen(buff_send), 0 );

                    //Se limpian los buffer.
                    bzero(buff_recv, len_recv);
                    bzero(buff_send, strlen(buff_send));
                }
            } while( strcmp(buff_recv,"salir\n") != 0 );

            printf("Se ha cerrado la conexion con el cliente...\n");
            close(client_fd);
            //exit(0);
        }
    }
    close(server_fd);
    shutdown(server_fd, SHUT_RDWR);
    puts("Fin del programa...");
    exit(0);
}
