/*
 * One Time Pads (OTP)
 * keygen.c
 * Mico Santiago
 * 11/22/2022
 * 
 * Description
 * -----------
 * This program creates a key file of specified length. The characters in the file generated 
 * will be any of the 27 allowed characters (26 capital letters and 1 space), generated using 
 * the standard Unix randomization methods.
 *
 * Usage
 * -----
 * $ keygen 256 > mykey
 *
 */
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define upper_alphabet 28
#define lower_alphabet 1
#define base 10

/*
 * Function: num_to_char
 * ---------------------
 * Maps a number between 1 and 27 to a respective char. Does not user ASCII conversion because
 * of space character.
 */
char num_to_char(int num){
  char c; 
  if(num==1){c='A';return c;}
  if(num==2){c='B';return c;}
  if(num==3){c='C';return c;}
  if(num==4){c='D';return c;}
  if(num==5){c='E';return c;}
  if(num==6){c='F';return c;}
  if(num==7){c='G';return c;}
  if(num==8){c='H';return c;}
  if(num==9){c='I';return c;}
  if(num==10){c='J';return c;}
  if(num==11){c='K';return c;}
  if(num==12){c='L';return c;}
  if(num==13){c='M';return c;}
  if(num==14){c='N';return c;}
  if(num==15){c='O';return c;}
  if(num==16){c='P';return c;}
  if(num==17){c='Q';return c;}
  if(num==18){c='R';return c;}
  if(num==19){c='S';return c;}
  if(num==20){c='T';return c;}
  if(num==21){c='U';return c;}
  if(num==22){c='V';return c;}
  if(num==23){c='W';return c;}
  if(num==24){c='X';return c;}
  if(num==25){c='Y';return c;}
  if(num==26){c='Z';return c;}
  if(num==27){c=' ';return c;}

  return c;  
}


int main(int argc, char *argv[]){
  //use computer time as seed
  srand(time(0));
  // convert string to int
  char *end;
  int num=strtol(argv[1],&end,base);
  // for putting an extra line on the end
  int key_length=atoi(argv[1]);
  char *final_buffer=malloc(key_length+1);
  //gets random letter from num_to_char
  for(int i=0;i<num;i++){
    int rand_var=(rand()%(upper_alphabet-lower_alphabet))+lower_alphabet;
    final_buffer[i]=num_to_char(rand_var);
  }
  printf("%s\n",final_buffer);
  return EXIT_SUCCESS;
}
