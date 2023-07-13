/*
 * Mico Santiago
 * Oct 4, 2022
 * CS 344 Operating Systems
 * Oregon State University
 * 
 * How to compile:
 * gcc -std=c99 -Wall -Wextra -Wpedantic -Werror -o base64enc base64enc.c 
 * 
 *
 */

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>// *should* typedef uint8_t
// Check that uint8_t type exists
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif

#define arraylen(x) (sizeof x/sizeof* x)

/* Bit Manipulation algo
 * Input: 
 *     8 bits of 3
 * Output: 
 *     BASE64 (0-63) index
 * Example:
 * in[0]    in[1]    in[2]    out_idx[0] out_idx[1] out_idx[2] out_idx[3]
 * abcdefgh ijklmnop qrstuvwx 00abcdef   00ghijkl   00mnopqr   00stuvwx  
 * 
*/

int main(int argc,char* argv[]){
  (void)argc;
  int line_characters=4;
  int fileisread=0;
  FILE* file = fopen(argv[1],"rb");
  for(;;){
    uint8_t in[3]={0};
    size_t r;
    if(argv[1]!=NULL){
      // printf "some-word" | ./base64enc -
      if(strcmp(argv[1],"-")==0){
        r = fread(in, sizeof* in, arraylen(in), stdin);
      }
      // ./base64enc file1 file2
      else if(argv[2]!=NULL){
        fprintf(stderr, "Please give one file.");
        exit(1);
      }
      else if(!file){
        fprintf(stderr, "File opening failed.");
        exit(1);
      }
      else{// ./base64enc some-working-file
      r = fread(in, sizeof* in, arraylen(in),file);
      fileisread=1;
      }
    }
    // printf "some-word" | ./base64enc
    else{
      r = fread(in, sizeof* in, arraylen(in), stdin);
    }
    // ./base64enc <&-
    if(r<arraylen(in)){
      if(ferror(stdin)) err(errno, "fread");

      if(r==0) break;
    }
    uint8_t out_idx[4]={0};
    //1
    out_idx[0] = in[0]>>2;
    //2
    out_idx[1] = ((in[0] & 0x03) << 4) | in[1] >> 4;
    //3
    out_idx[2] = ((in[1] & 0x0F) << 2) | in[2] >> 6;
    //4
    out_idx[3] = (in[2]&0x3F);
    //out_idx is an index into our base64 conversion array
    char output[arraylen(out_idx)];
    static char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789+/=";
    //Loop is for padding characters to represent missing bytes
    for(int i=0;(size_t)i<arraylen(out_idx);++i){
      if((size_t)i*4+1>r*6){
        output[i] = '=';
        line_characters++;
      }
      else output[i] = alphabet[out_idx[i]];
    }
    //output
    fwrite(output, sizeof* output, arraylen(output), stdout);
    /*when r is less then length of the array, we know we
     *reach eof*/
    if(r<arraylen(in)){ 
      line_characters++;
      break;
    }
    if(line_characters==76){
      line_characters=0;
      putchar('\n');
    }
    line_characters=line_characters+4;
  }
  putchar('\n');
  if(fileisread==1) fclose(file);
  return EXIT_SUCCESS;
}

