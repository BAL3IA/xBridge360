// xBridge360 - Servidor

#include "xbridge360.h"
#include "libserver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#define DATA_LEN 20

int main() {

    printf("Resolvendo endereço local...\n");
    struct addrinfo endereco = {0};

    endereco.ai_family = AF_INET6;
    endereco.ai_socktype = SOCK_STREAM;
    endereco.ai_flags = AI_PASSIVE;

    struct addrinfo *end_ptr;
    getaddrinfo(0, "8080", &endereco, &end_ptr);

    printf("Criando soquete...\n");
    SOCKET soqueteAberto;
    soqueteAberto = socket(end_ptr->ai_family, end_ptr->ai_socktype, end_ptr->ai_protocol);

    if (!ISVALIDSOCKET(soqueteAberto)) {

        fprintf(stderr, "socket() falhou. (%d)\n", GETSOCKETERRNO());
        return 1;
    } 

    int opcao = 0;
    if (setsockopt(soqueteAberto, IPPROTO_IPV6, IPV6_V6ONLY, (void*) &opcao, sizeof(opcao))) {

        fprintf(stderr, "setsockopt() falhou. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding no endereço...\n");
    if (bind(soqueteAberto, end_ptr->ai_addr, end_ptr->ai_addrlen)) {

        fprintf(stderr, "bind() falhou. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    freeaddrinfo(end_ptr);

    printf("Ouvindo...\n");
    if (listen(soqueteAberto,10) < 0) {

        fprintf(stderr, "listen() falhou. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    fd_set master;
    FD_ZERO (&master);
    FD_SET(soqueteAberto, &master);
    SOCKET soqueteMax = soqueteAberto;

    printf("Aguardando conexões...\n");

    while(1) {
 
        fd_set le;
        le = master;
        if (select(soqueteMax+1, &le, 0, 0, 0) < 0) {

            fprintf(stderr, "Falha em select() (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        SOCKET i;
        for (i = 1; i <= soqueteMax; ++i) {

            if (FD_ISSET(i, &le)) {

                if (i == soqueteAberto) {

                    struct sockaddr_storage enderecoCliente;
                    socklen_t cliente_len = sizeof(enderecoCliente);
                    SOCKET soqueteCliente = accept(soqueteAberto, (struct sockaddr*) &enderecoCliente, &cliente_len);
                    if (!ISVALIDSOCKET(soqueteCliente)) {
                        fprintf(stderr, "Falha em accept(). (%d)\n", GETSOCKETERRNO());
                        return 1;
                    }

                    FD_SET(soqueteCliente, &master);
                    if (soqueteCliente > soqueteMax) soqueteMax = soqueteCliente;

                    char endBuffer[100];
                    getnameinfo((struct sockaddr*) &enderecoCliente, cliente_len, endBuffer, sizeof(endBuffer), 0, 0, NI_NUMERICHOST);
                    printf("Nova conexão de %s\n", endBuffer);

                }

                else {

                    unsigned char leitura[100];
                    memset(leitura, 0, 100);
                    int bytesRecebidos = recv(i, leitura, 100, 0);
                    if (bytesRecebidos < 1) {
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }  

                    if (cmdSAIDA(leitura)) { 
                        printf("Comando de saída recebido.\n");
                        break;
                    }
                    
                    printf("Recebidos %d bytes: ", bytesRecebidos);  

                    for (int j = 0; j < bytesRecebidos; ++j) {
                        printf("%02x ", leitura[j]); 
                    }
                    printf("\n");
                    
                }
            }
        }
    }

    printf("Fechando soquetes...\n");
    CLOSESOCKET(soqueteAberto);

    return 0;

}

