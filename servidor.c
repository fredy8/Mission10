#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAXBUF (1<<16)
char buffer[MAXBUF];

int saveBuffer() {
  char file_name[60];
  sprintf(file_name, "data/%ld.txt", time(NULL));

  FILE *fp = fopen(file_name, "w+");
  if (fp == NULL)
    return -1;

  if (fputs(buffer, fp) == EOF)
    return -1;

  if (fclose(fp) == EOF)
    return -1;

  return 0;
}

int main() {
  int sockfd;
  struct sockaddr_in self;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Could not create socket");
    exit(errno);
  }

  bzero(&self, sizeof(self));
  self.sin_family = AF_INET;
  self.sin_port = htons(9999);
  self.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0) {
    perror("Could not bind the socket to the address");
    exit(errno);
  }

  if (listen(sockfd, 20) != 0) {
    perror("Could not set the socket to listen mode.");
    exit(errno);
  }

  printf("Server started\n");

  while (1) {
    int clientfd;
    struct sockaddr_in client_addr;
    unsigned int addrlen = sizeof(client_addr);

    clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
    printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    int bytes_written = 0, bytes_received = 0;
    while((bytes_received = recv(clientfd, buffer, MAXBUF, 0)) != 0) {
      printf("Received %d bytes\n", bytes_received);
      if (bytes_received < 0) {
        perror("Error receiving data.");
      }
    }

    if (saveBuffer() != 0)
      return -1;
    close(clientfd);
  }

  close(sockfd);
  return 0;
}
