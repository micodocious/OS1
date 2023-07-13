/*
 * Mico Santiago
 * NOV 20, 2022
 * CS 344 Operating Systems
 * Oregon State University
 * 
 * How to compile:
 * $ gcc -std=c99 -pthread -o line_processor line_processor.c 
 * Using makefile:
 * $ make all
 *
 * objectives: write a program that creates 4 threads to process input from stdin
 * thread 1 - [IN] input thread
 * thread 2 - [LST] line seperator thread. replaces every line seperator in the input by a space
 * thread 3 - [PST] plus sign thread. replaces every par of plus signs i.e., "++" by a "^".
 * thread 4 - [OUT] output thread. write data to stdout as lines of 80 chars
 *
 *  defines
 *  size 100 //size buffers
 *  num_items 50 //number of items produced. should be less then buffer
 *
 * initialize 
 *   buffer 1, shared resource between [IN] and [LST]
 *   number of items in buffer
 *   index where [IN] will put the next item
 *   index where [LST] will pick up the next item
 *   initialize the condition var for the mutex for buffer 1
 *   Initialize the condition var for buffer 1
 *  
 *   buffer 2, shared resource between the [LST] and [PST] 
 *   number of items in buffer
 *   index where [LST] will put the next item
 *   index where [PST] will pick up the next item
 *   initialize the condition var for the mutex for buffer 2
 *   Initialize the condition variable for buffer 2
 *   
 *   buffer 3, shared resource between [PST] and [OUT]
 *   number of items in buffer
 *   index where [LST] will put the next item
 *   index where [OUT] will pick up the next item
 *   initialize the condition var for the mutex for buffer 3
 *   Initialize the condition var for buffer 3
 *  
 * 
 *  main
 *  1 initialize pthread_t
 *  2 create threads with pthread_create
 *    2.1 *get input (void) return NULL
 *      2.1.1 put in buffer 1 (void)
 *    2.2 line seperator thread (void) return NULL
 *      2.2.1 get from buffer 1 (void)
 *      2.2.2 put in buffer 2 (void)
 *    2.3 plus_sign_thread (void) return NULL
 *      2.3.1 get from buffer 2 (void)
 *      2.3.2 put in buffer 3 (void)
 *    2.4 write output (void) return NULL
 *      2.4.1 get from buffer 3 (double)
 *  3 wait for threads to terminate and join threads with pthread_join
 *    3.1 input_t, 2_t, 3_t
 *  
 *
 */


#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
/* 1 includes */
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h> // must link with -lm
/* 2. defines */
// Size of the buffers
// An input line will never be longer than 1000 characters (including the line separator).
#define SIZE 1000
// Number of items that will be produced. This number is less than the size of the buffer. Hence, we can model the buffer as being unbounded.
// The input for the program will never have more than 49 lines before the stop-processing line.
#define NUM_ITEMS 49

/* 3 external declarations */
/* 4 typedefs */
/* 5 global variable declarations */
// Buffer 1, shared resrc between input thread and square-root thread
char buffer_1[NUM_ITEMS][SIZE];
// Number of items in the buffer
int count_1 = 0;
// Index where the input thread will put the next item
int prod_idx_1 = 0;
// Index where the square-root thread will pick up the next item
int con_idx_1 = 0;
// Initialize the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;


// Buffer 2, shared resrc between square root thread and output thread
char buffer_2[NUM_ITEMS][SIZE];
// Number of items in the buffer
int count_2 = 0;
// Index where the square-root thread will put the next item
int prod_idx_2 = 0;
// Index where the output thread will pick up the next item
int con_idx_2 = 0;
// Initialize the mutex for buffer 2
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

// Buffer 3, shared resrc between Plus Sign thread and output thread
char buffer_3[NUM_ITEMS][SIZE];
// Number of items in the buffer
int count_3 = 0;
// Index where the square-root thread will put the next item
int prod_idx_3 = 0;
// Index where the output thread will pick up the next item
int con_idx_3 = 0;
// Initialize the mutex for buffer 2
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;

/* 6 function prototypes */
/* Program Flow */
void string_replace(char *item, char *target, char *replacement);
void *get_input(void *args);
  void put_buff_2(char* item);
void *line_seperator_thread(void *args);
  void get_buff_1(char *item);
  void put_buff_1(char *item);
void *plus_sign_thread(void *args);
  void get_buff_2(char *item);
  void put_buff_3(char* item);
void *write_output(void *args);
  void get_buff_3(char* item);

/*
A program with a pipeline of 3 threads that interact with each other as producers and consumers.
- Input thread is the first thread in the pipeline. It gets input from the user and puts it in a buffer it shares with the next thread in the pipeline.
- Thread 2, called the Line Separator Thread, replaces every line separator in the input by a space.
- Thread 3, called the Plus Sign thread, replaces every pair of plus signs, i.e., "++", by a "^".
- Thread 4, called the Output Thread, write this processed data to standard output as lines of exactly 80 characters. 
*/

int main(){
    srand(time(0));
    pthread_t input_t, line_seperator_thread_t, plus_sign_thread_t, output_t;
    // Create the threads
    pthread_create(&input_t, NULL, get_input, NULL);
    pthread_create(&line_seperator_thread_t, NULL, line_seperator_thread, NULL);
    pthread_create(&plus_sign_thread_t, NULL, plus_sign_thread, NULL);
    pthread_create(&output_t, NULL, write_output, NULL);
    // Wait for the threads to terminate
    pthread_join(input_t, NULL);
    pthread_join(line_seperator_thread_t, NULL);
    pthread_join(plus_sign_thread_t, NULL);
    pthread_join(output_t, NULL);
    return EXIT_SUCCESS;
}
/*
 * Function: string_replace
 * ------------------------
 * Taken from my variable expansion of $$ in smallsh. 
 * Inspired by user Gaurang Tandon
 * [ https://stackoverflow.com/questions/779875/what-function-is-to-replace-a-substring-from-a-string-in-c ]
 * Replaces a sub string in c
 */
void string_replace(char *item, char *target, char *replacement){
  while(strstr(item,target)!=NULL){
    char buffer[SIZE];
    char *ptr;
    // grab pointer of the location of item
    ptr=strstr(item,target);
    // copy chars from start of input to $$
    strncpy(buffer, item,ptr-item);
    buffer[ptr-item]='\0';
    // copy the other input to the buffer
    sprintf(buffer+(ptr-item),"%s%s",replacement,ptr+strlen(target));
    // copy buffer back into input
    strcpy(item,buffer);
  }
}

/* 2.1
 * Function: *get_input
 * --------------------
 *
 */
void *get_input(void *args){
  char item[SIZE];  
  for (int i = 0; i < NUM_ITEMS; i++){
    fgets(item,SIZE,stdin);// Get the user input
    put_buff_1(item);
    if(strcmp(item,"STOP\n")==0){break;}
  }
    return NULL;
}

/* 2.1.1
 * Funtcio: put_buff_1
 * -------------------
 */
void put_buff_1(char *item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_1);
  // Put the item in the buffer
  strcpy(buffer_1[prod_idx_1], item);
  // Increment the index where the next item will be put.
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_1);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}

/* 2.2
 * Function: *line_seperator_thread 
 * --------------------------------
 * replaces every line separator in the input by a space.
 */
void *line_seperator_thread(void *args){
  char item[SIZE];
  for (int i = 0; i < NUM_ITEMS; i++){
    get_buff_1(item);
    if(strcmp(item,"STOP\n")==0){
      put_buff_2(item);
      break;
    }
    string_replace(item,"\n"," ");
    put_buff_2(item);
  }
  return NULL;
}


/* 2.2.1
 * Function: get_buff_1
 * --------------------
 *
 */
void get_buff_1(char *item){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  strcpy(item,buffer_1[con_idx_1]);
  // Increment the index from which the item will be picked up
  con_idx_1 = con_idx_1 + 1;
  count_1--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}

/* 2.2.2
 * Function: put_buff_2
 * --------------------
 *
 */
void put_buff_2(char* item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_2);
  // Put the item in the buffer
  strcpy(buffer_2[prod_idx_2], item);
  // Increment the index where the next item will be put.
  prod_idx_2 = prod_idx_2 + 1;
  count_2++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_2);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}

/* 2.3
 * Function: put_buff_3
 * --------------------
 * Get the next item from buffer 2
 */
void *plus_sign_thread(void *args){
  char item[SIZE];

  for (int i = 0; i < NUM_ITEMS; i++){
    get_buff_2(item);
    if(strcmp(item,"STOP\n")==0){
      put_buff_3(item);
      break;
    }
    string_replace(item,"++","^");
    put_buff_3(item);
  }
  return NULL;
}

/* 2.3.1
 * Function: get_buff_2
 * --------------------
 * Get the next item from buffer 2
 */
void get_buff_2(char *item){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_2);
  while (count_2 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_2, &mutex_2);
  strcpy(item, buffer_2[con_idx_2]);
  // Increment the index from which the item will be picked up
  con_idx_2 = con_idx_2 + 1;
  count_2--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}


/* 2.3.2
 * Function: put_buff_3
 * --------------------
 * Get the next item from buffer 2
 */
void put_buff_3(char* item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_3);
  // Put the item in the buffer
  strcpy(buffer_2[prod_idx_3], item);
  // Increment the index where the next item will be put.
  prod_idx_3 = prod_idx_3 + 1;
  count_3++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_3);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
}



/* 2.4
 * Function: write_output
 * ----------------------
 * Function that the output thread will run. 
 * Consume an item from the buffer shared with the square root thread.
 * Print the item.
 */
void *write_output(void *args){
  char item[SIZE];
  char buffer[SIZE*SIZE];
  char final[SIZE*SIZE];
  int first_pass=0;
  for (int i = 0; i < NUM_ITEMS; i++){
    get_buff_3(item);
    if(strcmp(item,"STOP\n")==0){break;}
    if(first_pass==0){
      snprintf(final,sizeof final,"%s", item);
      first_pass=1;
    }
    else{
      strcpy(buffer, final);
      snprintf(final,sizeof final,"%s%s", buffer, item);
    }
  fflush(stdout);
  }
  int line=1;
  int pass=0;
  while((strlen(final)/line)>=80){
    if(pass==1){
      for(int i=80*0;i<=80*1-0;i++){
        printf("%c", final[i]);
        fflush(stdout);
      }
      pass=1;
    }
    else{
      for(int i=80*(line-1);i<=80*line-1;i++){
        printf("%c", final[i]);
        fflush(stdout);
      }
    }
    line++;
    if(strlen(final)/line>=80){printf("\n");}
  } 
  //The above code is basically the code below
  //but in a loop.

  /*printf("\n\n");
  for(int i=80*0;i<=80*1-0;i++){
      printf("%c", final[i]);
  }
  printf("\n");
  
  for(int i=80*1;i<=80*2-1;i++){
      printf("%c", final[i]);
  }
  printf("\n");
  for(int i=80*2;i<=80*3-1;i++){
      printf("%c", final[i]);
  }
  printf("\n");
  for(int i=80*3;i<=80*4-1;i++){
      printf("%c", final[i]);
  }*/

  //printf("\n%lu",strlen(buffer));
  //printf("\n%lu",strlen(final));
  
  return NULL;
}

/* 2.4.1
 * Function: get_buff_3
 * --------------------
 *
 */
void get_buff_3(char *item){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_3);
  while (count_3 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_3, &mutex_3);
  strcpy(item, buffer_2[con_idx_3]);
  // Increment the index from which the item will be picked up
  con_idx_3 = con_idx_3 + 1;
  count_3--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
}



