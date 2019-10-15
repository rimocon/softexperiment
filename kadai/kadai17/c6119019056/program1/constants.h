#include <netinet/in.h>

#define DEFAULT_PORT 51000
#define MAX_LEN_NAME 10
#define MAX_NUM_CLIENTS 5
#define MAX_LEN_BUFFER 256
#define MAX_LEN_ADDR 32
#define BROADCAST -1

#define MESSAGE_COMMAND 'M'
#define QUIT_COMMAND 'Q'

typedef struct {
  int cid;
  int sock;
  struct sockaddr_in addr;
  char name[MAX_LEN_NAME];
} CLIENT;

typedef struct {
  int cid;
  char command;
  char message[MAX_LEN_BUFFER];
} CONTAINER;
