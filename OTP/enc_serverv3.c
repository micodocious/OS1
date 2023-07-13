#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#define BUF_SIZE 500
#define BACKLOG 5

/*
 * Function: char_to_num
 * ---------------------
 * Maps a number between 1 and 27 to a respective char. Does not user ASCII conversion because
 * of space character.
 */
int char_to_num(char c){
 int n;
 if(c=='A'){n=0;return n;}
 if(c=='B'){n=1;return n;}
 if(c=='C'){n=2;return n;}
 if(c=='D'){n=3;return n;}
 if(c=='E'){n=4;return n;}
 if(c=='F'){n=5;return n;}
 if(c=='G'){n=6;return n;}
 if(c=='H'){n=7;return n;}
 if(c=='I'){n=8;return n;}
 if(c=='J'){n=9;return n;}
 if(c=='K'){n=10;return n;}
 if(c=='L'){n=11;return n;}
 if(c=='M'){n=12;return n;}
 if(c=='N'){n=13;return n;}
 if(c=='O'){n=14;return n;}
 if(c=='P'){n=15;return n;}
 if(c=='Q'){n=16;return n;}
 if(c=='R'){n=17;return n;}
 if(c=='S'){n=18;return n;}
 if(c=='T'){n=19;return n;}
 if(c=='U'){n=20;return n;}
 if(c=='V'){n=21;return n;}
 if(c=='W'){n=22;return n;}
 if(c=='X'){n=23;return n;}
 if(c=='Y'){n=24;return n;}
 if(c=='Z'){n=25;return n;}
 if(c==' '){n=26;return n;}
 return n;
}
/*
 * Function: num_to_char
 * ---------------------
 * Maps a number between 1 and 27 to a respective char. Does not user ASCII conversion because
 * of space character.
 */
char num_to_char(int num){
  char c; 
  if(num==0){c='A';return c;}
  if(num==1){c='B';return c;}
  if(num==2){c='C';return c;}
  if(num==3){c='D';return c;}
  if(num==4){c='E';return c;}
  if(num==5){c='F';return c;}
  if(num==6){c='G';return c;}
  if(num==7){c='H';return c;}
  if(num==8){c='I';return c;}
  if(num==9){c='J';return c;}
  if(num==10){c='K';return c;}
  if(num==11){c='L';return c;}
  if(num==12){c='M';return c;}
  if(num==13){c='N';return c;}
  if(num==14){c='O';return c;}
  if(num==15){c='P';return c;}
  if(num==16){c='Q';return c;}
  if(num==17){c='R';return c;}
  if(num==18){c='S';return c;}
  if(num==19){c='T';return c;}
  if(num==20){c='U';return c;}
  if(num==21){c='V';return c;}
  if(num==22){c='W';return c;}
  if(num==23){c='X';return c;}
  if(num==24){c='Y';return c;}
  if(num==25){c='Z';return c;}
  if(num==26){c=' ';return c;}

  return c;  
}
/*
 * Function: forward_signal
 * When forking, allow the child process to terminate when
 * it's done. 
 */
void forward_handler(int signal){
  (void)signal;
  int status=0;
  while((waitpid(-1,&status,WNOHANG))>0){}
}

/*
 * Function: forward_signal
 * Inspired by the user Nominal Animal: 
 * [ https://stackoverflow.com/questions/40477988/calling-kill-on-a-child-process-with-sigterm-terminates-parent-process-but-call ]
 * Preventing SIGINT in child process from propagating to and killing parent process.
 */
void forward_signal(void){
  struct sigaction act;
  memset(&act,0,sizeof act);
  sigemptyset(&act.sa_mask);
  act.sa_handler=forward_handler;
  act.sa_flags=SA_SIGINFO | SA_RESTART;
  sigaction(SIGCHLD,&act,NULL);
}

int
main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s;
  int charsRead,key_length,text_length;
  struct sockaddr_storage peer_addr;
  socklen_t peer_addr_len;
  ssize_t socket_connect;
  char *text_buffer;
  char *key_buffer;
  char *final_buffer;
  
  forward_signal();

  // check usage & args
  if (argc != 2) {
      fprintf(stderr, "Usage: %s port\n", argv[0]);
      exit(EXIT_FAILURE);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* STREAM socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo(NULL, argv[1], &hints, &result);
  if (s != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
      exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until we successfully bind(2).
     If socket(2) (or bind(2)) fails, we (close the socket
     and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next) {
      sfd = socket(rp->ai_family, rp->ai_socktype,
              rp->ai_protocol);
      if (sfd == -1)
          continue;

      if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
          break;                  /* Success */

      close(sfd);
  }

  freeaddrinfo(result);           /* No longer needed */

  if (rp == NULL) {               /* No address succeeded */
      fprintf(stderr, "Could not bind\n");
      exit(EXIT_FAILURE);
  }

  if(listen(sfd, BACKLOG)==-1){
    perror("listen");
    exit(1);
  }

  pid_t spawnpid;

  // Accept a connection, blocking if one is not available until one connects
  for (;;) {
    // Accept the connection request which creates a connection socket
    peer_addr_len = sizeof peer_addr;
    socket_connect = accept(sfd,(struct sockaddr *) &peer_addr, &peer_addr_len);
    if (socket_connect == -1){
      perror("error on accept");  
      continue;               /* Ignore failed request */
    }

    spawnpid=fork();
    int confirm=0;
    // Get the message from the client and display it
    switch(spawnpid){
      case -1:
        perror("fork() failed!");
		    exit(1);
		    break;
      case 0:
        // Confirm that you're using the right server-client
        /*charsRead=recv(socket_connect,&confirm,sizeof(int),0);
        if(confirm!=1994){
          fprintf(stderr,"Wrong Server!\n");
          exit(2);
        }*/
        charsRead = send(socket_connect, "enc", 3, 0); 

        // Read the client's message from the socket
        charsRead = recv(socket_connect, &text_length, sizeof(int), 0); 
        if (charsRead < 0){fprintf(stderr,"ERROR reading from socket");}
        //printf("SERVER: The text length from the client: \"%d\"\n", text_length);

        // Read the client's message from the socket
        charsRead = recv(socket_connect, &key_length, sizeof(int), 0); 
        if (charsRead < 0){fprintf(stderr,"ERROR reading from socket");}
        //printf("SERVER: The key length from the client: \"%d\"\n", key_length);
        
        text_buffer=malloc(text_length);
        key_buffer=malloc(key_length);

        // Read the client's message from the socket
        charsRead = recv(socket_connect, text_buffer, text_length, 0); 
        if (charsRead < 0){fprintf(stderr,"ERROR reading from socket");}
        // Read the client's message from the socket
        charsRead = recv(socket_connect, key_buffer, key_length, 0); 
        if (charsRead < 0){fprintf(stderr,"ERROR reading from socket");}
        
        //printf("SERVER: I received this from the client: \"%s\"\n", text_buffer);
        //printf("SERVER: I received this from the client: \"%s\"\n", key_buffer);
        /*for(int i=0;i<text_length-1;i++){
          printf("%c", text_buffer[i]);
        }
        //printf("\n");
        for(int i=0;i<text_length-1;i++){
          printf("%c", key_buffer[i]);
        }
        printf("\n");*/
        final_buffer = malloc(text_length);
        for(int i=0;i<text_length-1;i++){
          int text=char_to_num(text_buffer[i]);
          int key=char_to_num(key_buffer[i]);
          final_buffer[i]=num_to_char((text+key)%27);
        }
        // Send a Success message back to the client
        charsRead = send(socket_connect, final_buffer, strlen(final_buffer), 0); 
        if (charsRead < 0){fprintf(stderr,"ERROR reading from socket");}
        
        //free(text_buffer);
        //free(key_buffer);
        // Close the connection socket for this client
        close(socket_connect);
        exit(0);
    }
  }
  // Close the listening socket
  close(sfd);
  return 0;
}
