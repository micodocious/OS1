#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500


int
main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s, numbytes;
  int charsRead,charsWritten;
  size_t len;
  ssize_t nread;
  char text_buffer[BUF_SIZE];
  char key_buffer[BUF_SIZE];
  char *final_buffer;
  struct sockaddr_in peer_addr;
  
  //cmd line error checking 
  if (argc < 4) {
      fprintf(stderr, "Usage: %s [text] [key] [port]...\n", argv[0]);
      exit(EXIT_FAILURE);
  }

  /* Obtain address(es) matching host/port. */

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Stream socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;          /* Any protocol */

  s = getaddrinfo("localhost", argv[3], &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until we successfully connect(2).
     If socket(2) (or connect(2)) fails, we (close the socket
     and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype,
                 rp->ai_protocol);
    if (sfd == -1)
        continue;
    if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
         break;                  /* Success */

     close(sfd);
  }

  freeaddrinfo(result);           /* No longer needed */
  
  // text file
  FILE *text_fp=fopen(argv[1],"rb");    // Get from file
  size_t text_pos = ftell(text_fp);    // Current position
  fseek(text_fp, 0, SEEK_END);         // Go to end
  size_t text_length = ftell(text_fp);      // read the position which is the size
  fseek(text_fp, text_pos, SEEK_SET);  // restore original position
  memset(text_buffer, '\0', sizeof text_buffer);       // Clear out the buffer array
  // Get input from the user, trunc to buffer - 1 chars, leaving \0
  fgets(text_buffer, sizeof text_buffer - 1, text_fp);
  text_buffer[strcspn(text_buffer, "\n")] = '\0';      // Remove the trailing \n that fgets adds
  fclose(text_fp);

  // key file
  FILE *key_fp=fopen(argv[2],"rb");    // Get from file
  size_t key_pos = ftell(key_fp);    // Current position
  fseek(key_fp, 0, SEEK_END);         // Go to end
  size_t key_length = ftell(key_fp);      // read the position which is the size
  fseek(key_fp, key_pos, SEEK_SET);  // restore original position
  memset(key_buffer, '\0', sizeof key_buffer);       // Clear out the buffer array
  // Get input from the user, trunc to buffer - 1 chars, leaving \0
  fgets(key_buffer, sizeof key_buffer - 1, key_fp);
  key_buffer[strcspn(key_buffer, "\n")] = '\0';      // Remove the trailing \n that fgets adds
  fclose(key_fp);
  
  //error checking for length of key vs. length of text
  if(text_length>key_length){
    fprintf(stderr,"CLIENT: Key Length is too short.\n");
    exit(1);
  }
  int flag=0;
  for(int i=0;i<strlen(text_buffer);i++){
    if(text_buffer[i]=='$'||text_buffer[i]=='*'||
       text_buffer[i]=='!'||text_buffer[i]=='('||
       text_buffer[i]=='#'||text_buffer[i]=='-'){
      flag=1;
      break;
    }
  }
  if(flag==1){
    fprintf(stderr, "CLIENT: Contains bad characters!\n");
    exit(1);
  }
  // confirm if enc_client -> enc_server
  char confirm_enc_client[4];
  memset(confirm_enc_client, '\0', 2);
  charsWritten = recv(sfd, confirm_enc_client, 3, 0);
  if(strcmp(confirm_enc_client,"enc")!=0){
    fprintf(stderr, "CLIENT: Incorrect Server.\n");
    exit(2);
  }

  // send text length to server 
  charsWritten = send(sfd, &text_length, sizeof(int), 0); 
  if (charsWritten < 0){
    fprintf(stderr,"CLIENT: ERROR writing text length to socket");
  }
  // send key length to server 
  charsWritten = send(sfd, &key_length, sizeof(int), 0); 
  if (charsWritten < 0){
    fprintf(stderr,"CLIENT: ERROR writing key length to socket");
  }

  // Send message to server
  // Write textfile to the server
  charsWritten = send(sfd, text_buffer, text_length, 0); 
  if (charsWritten < 0){
    fprintf(stderr,"CLIENT: ERROR writing text buffer to socket");
  }
  if (charsWritten < strlen(text_buffer)){
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  }
  // Write key to the server
  charsWritten = send(sfd, key_buffer, key_length, 0); 
  if (charsWritten < 0){
    fprintf(stderr,"CLIENT: ERROR writing key buffer to socket");
  }
  if (charsWritten < strlen(key_buffer)){
    printf("CLIENT: WARNING: Not all data written to socket!\n");
  }

  // Get return message from server
  // Clear out the buffer again for reuse
  final_buffer=malloc(text_length); 
  // Read data from the socket, leaving \0 at end
  charsRead = recv(sfd, final_buffer, text_length, 0); 
  if (charsRead < 0){
    fprintf(stderr,"CLIENT: ERROR reading final buffer from socket\n");
  }
  fprintf(stdout,"%s\n", final_buffer);
  // Close the socket
	close(sfd);
  return 0;
}
