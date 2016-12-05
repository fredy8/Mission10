#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int send_file(char* data) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    return -1;

  struct sockaddr_in dest;
  bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(9999);
  if (inet_aton("127.0.0.1", &dest.sin_addr.s_addr) == 0) {
    return -1;
  }

  if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) < 0)
    return -1;

  int n = write(sockfd, data, strlen(data));
  printf("%d\n", n);
  if (n < 0)
    return -1;
  
  return 0;
}

int main() {
  printf("Send file test\n");
  char* data = "Test file\nHello world\n";
  int err = send_file(data);
  if (err != 0) {
    printf("Error sending file: %d\n", err);
  } else {
    printf("File sent\n");
  }
}
