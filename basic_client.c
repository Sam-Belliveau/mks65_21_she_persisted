#include "pipe_networking.h"


int main() {
    int to_server;
    int from_server;

    from_server = client_handshake( &to_server );

    while(1)
    {
        char buffer[BUFFER_SIZE] = {};
        
        client_printf("Waiting for user input...\n");
        printf("rot13> ");
        if(fgets(buffer, BUFFER_SIZE, stdin))
        {
            write(to_server, buffer, BUFFER_SIZE);

            char returned[BUFFER_SIZE] = {};
            read(from_server, returned, BUFFER_SIZE);

            client_printf("Recieved: %s\n\n", returned);
        }
        else
        {
            client_printf("Exiting...\n");
            close(to_server);
            close(from_server);
            return 0;
        }
    }
}