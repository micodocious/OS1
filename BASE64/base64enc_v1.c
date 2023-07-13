#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // *should* typedef uint8_t
// Check that uint8_t type exists
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif

#define MAINBUFFER 1000000


/* Bit Manipulation algo
 * Input: 
 *     8 bits of 3
 * Output: 
 *     BASE64 (0-63) index
 * Example:
 * in[0]    in[1]    in[2]    out_idx[0] out_idx[1] out_idx[2] out_idx[3]
 * abcdefgh ijklmnop qrstuvwx 00abcdef   00ghijkl   00mnopqr   00stuvwx  
 * 
 * static char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz"
                                  "0123456789+/=";
        uint8_t in[3] = "And";
        uint8_t out_idx[4];
        // 1
        out_idx[0] = in[0]>>2;
        //2
        out_idx[1] = ((in[0] & 0x03) << 4) | (in[1] >> 4);
        //3
        out_idx[2] = ((in[1] & 0x0F) << 2) | in[2]>>6;
        //4
        out_idx[3] = (in[2]&0x3F);
        
        for(int i=0; i<4; i++){
            printf("value of a: %d\n", out_idx[i]);
        }
 */
 
enum{SMALLBUFFER=3};
void processing(char* buffer,FILE* file){
  static char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789+/=";
  //remove this
  fprintf(stdout, "[test] processing buffer output: %s", buffer);
  //76 character wrap specification
  int line_characters=0;
  for(;;){
    uint8_t in[SMALLBUFFER]={0};
    uint8_t out_idx[4];
    //can either return from file or stdin
    size_t ret_code = fread(in, sizeof *in, SMALLBUFFER, file);
    if (ret_code == SMALLBUFFER) {
      puts("Array read successfully, contents: ");
    for (int n = 0; n < SMALLBUFFER; ++n)
        printf("%c ", in[n]);
      putchar('\n');
      }else{ // error handling
      if (feof(file))
        printf("Error reading file: unexpected end of file\n");
      else if (ferror(file)) {
        perror("Error reading file");
        }
      }
    if(line_characters>76){break;}
    line_characters++;
  }
}

int main(int argc, char* argv[]){
  char buffer[MAINBUFFER];
  FILE* file;
  /* 
   * The following deals with the commands:
   *     ./base64enc
   *     ./base64enc<<<input
   * Input: 
   *     No user input or redirection
   * Output: 
   *     Read from stdin
   */
  if(argc<2){
    fgets(buffer, MAINBUFFER, stdin);
    if(buffer[0] == '\n'){
      fprintf(stderr,"base64enc: Please enter text to encode\n");
      exit(0);
    }
    processing(buffer,file);
  }
  /*
   * The following deals with the commands:
   *     ./base64enc ""
   *     ./base64enc -
   *     ./base64enc validfile
   *     ./base64enc invalidfile
   * Input: 
   *     empty string, dash, valid file or just some text
   * Output:
   *     empty string gives warning and closes
   *     dash reads from stdin
   *     validfile reads from file 
   *     invalidfile gives warning and closes
   */
  if(argc>1){
    if(strcmp(argv[1],"")==0){
      fprintf(stderr,"base64enc: %s: No such file or directory", argv[1]);
      exit(0);
    }
    if(strcmp(argv[1],"-")==0){
      fgets(buffer, MAINBUFFER, stdin);
      if(buffer[0] == '\n'){
        fprintf(stderr,"base64enc: Please enter text to encode\n");
        exit(0);
      }
    //delete and send contents to precess
      fprintf(stdout,"[test] the buffer contains: %s\n", buffer);
    }
    if((file=fopen(argv[1],"rb"))){
      //delete and send contents to precess
      fprintf(stdout,"[test] success in opening file\n");\
      exit(0);
    }
    //delete and send contents to precess
    fprintf(stderr,"base64enc: Please enter a valid file to encode\n");
  }
  return EXIT_SUCCESS;
}
