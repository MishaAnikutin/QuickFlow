#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "../models/server.h"


#define PANIC(message) do { perror(message); exit(EXIT_FAILURE); } while (0)
#define CHECK(condition, message) do { if (condition) { PANIC(message); }} while (0)


typedef struct 
{
    struct 
    {
        int code;
        const char* message;
    } err;
    const char* data;
} response_t;


response_t
handle_request(char* request)
{
    response_t response = {
        .err = {.code = 0, .message = NULL},
        .data = NULL
    };

    printf("handled request: ");
    puts(request);

    // Тут будет парсинг запроса
    // Ловля его ошибок
    // И при их отсутствии - выполнение запроса
    
    response.data = "some kind of work brrrr brrrr..."; // пока заглушка

    return response;
}


void 
InitTCPServer(tcp_server_t** tcp_server) 
{
    *tcp_server = (tcp_server_t*) malloc(sizeof(tcp_server_t));

    CHECK(*tcp_server == NULL, "Ошибка выделения памяти для сервера");

    char* port_env = getenv("PORT");
    char* max_connections_env = getenv("MAX_CONNECTIONS");
    char* timeout_seconds_env = getenv("TIMEOUT_SECONDS");

    (*tcp_server)->max_connections = (max_connections_env != NULL) ? atoi(max_connections_env) : MAX_CONNECTIONS;
    (*tcp_server)->timeout_seconds = (timeout_seconds_env != NULL) ? atoi(timeout_seconds_env) : TIMEOUT_SECONDS;
    (*tcp_server)->port            = (port_env != NULL)            ? atoi(port_env)            : PORT;

    (*tcp_server)->address.s_addr = htonl(INADDR_ANY); 
    (*tcp_server)->socket = socket(AF_INET, SOCK_STREAM, 0);
    (*tcp_server)->protocol_type = IPPROTO_TCP;
}

void 
SetupTCPServer(tcp_server_t* tcp_server, int* server_fd, struct sockaddr_in* address) 
{
    CHECK((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1, "Ошибка при создании сокета");

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(tcp_server->port);

    CHECK(bind(*server_fd, (struct sockaddr *)address, sizeof(struct sockaddr_in)) == -1, "Ошибка при привязке сокета");
    CHECK(listen(*server_fd, tcp_server->max_connections) == -1, "Ошибка при прослушивании сокета");
}

void 
HandleConnections(int* server_fd, int new_socket, struct sockaddr_in address, int addrlen) 
{
    new_socket = accept(*server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen);
    CHECK(new_socket == -1, "Ошибка при принятии соединения");
    
    char buffer[MAX_BUFFER] = {0};

    read(new_socket, buffer, MAX_BUFFER);

    if (strncmp(buffer, "q();\n", 5) == 0) {
        close(new_socket);
        close(*server_fd);
        printf("вот и сказочке конец!");
    }

    response_t response = handle_request(buffer);

    if (response.err.code == -1) {
        close(new_socket);
        PANIC("Ошибка в обработке запроса");
    }

    const char* data = response.data;
    send(new_socket, data, strlen(data), 0);

    close(new_socket);
}
