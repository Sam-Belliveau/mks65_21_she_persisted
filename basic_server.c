#include "pipe_networking.h"

static void handle_client(int from_client, int to_client);

int main() {

    int to_client;
    int from_client;

    while(1)
    {
        from_client = server_handshake( &to_client );

        if(fork() == 0)
        {
            handle_client(from_client, to_client);
            exit(0);
        }

        else
        {
            close(from_client); from_client = -1;
            close(to_client); to_client = -1;
        }
    }
}

static char rot13(char c)
{
    if('a' <= c && c <= 'z')
    {
        c -= 'a' - 13;
        if(c >= 26) c -= 26;
        c += 'a';
    }

    else if('A' <= c && c <= 'Z')
    {
        c -= 'A' - 13;
        if(c >= 26) c -= 26;
        c += 'A';
    }

    return c;
}

static void handle_client(int from_client, int to_client)
{
    int bytes_read;
    char buffer[BUFFER_SIZE];
    char *i;

    server_printf("Spawning thread to handle client\n");

    while(1)
    {   
        memset(buffer, '\0', BUFFER_SIZE);
        bytes_read = read(from_client, buffer, BUFFER_SIZE);

        server_printf("Recieved %d bytes \"%s\"\n", bytes_read, buffer);

        if(bytes_read)
        {
            for(i = buffer; *i; ++i)
            {
                *i = rot13(*i);
            }

            server_printf("Returning %d bytes \"%s\"\n", bytes_read, buffer);

            write(to_client, buffer, bytes_read);
        }

        else
        {
            server_printf("Shutting down thread handler...\n");
            break;
        }
    }

    close(from_client);
    close(to_client);
}