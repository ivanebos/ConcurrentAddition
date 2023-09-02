#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"

int logindex = 0;
int *logi = &logindex;
// mutex for logging functions required for thread-safe code

int no_of_threads = 0;
#define ARRAYSIZE  1000000
int no_of_items = 0;

//TO DO:  Here go your global variables

long arr[ARRAYSIZE];
long global_sum;

pthread_mutex_t tlock = PTHREAD_MUTEX_INITIALIZER;


//TO DO:  declare and initialize global mutex
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;


void init_arr() {
  srand(time(NULL));
  long i;
  for(i = 0; i < ARRAYSIZE; i++) {
    arr[i] = (long) (rand() % 10);
  }
}


// function doit ----------------------------------------
void* doit(void *params) {
  // TO DO: here goes your local variables
  // TO DO: get from param your thread index

  int from;
  int to;
  int k;
  int* thread_index;


  long local_sum = 0;

  thread_index = (int*) params;

  // TO DO: find your slice of the array -- from and to
  from = *thread_index * no_of_items;
  to = from + no_of_items-1;

  // TO DO: display the message about which part of the array this threads does
  // TO DO: log the message about which part of the array this threads does

  if (to > ARRAYSIZE){
    to = ARRAYSIZE-1;
  }
  msg("Thread %d processing the array from %d to %d",*thread_index,from,to);
  Msg("Thread %d processing the array from %d to %d",*thread_index,from,to);



  // TO DO: sum up your portion of the array arr[]

  // TO DO: display the message about your summation
  // TO DO: log the message about your summation

  for(k = from; k <= to; k++){
    local_sum += arr[k];



  }
  msg("Thread %d summation is %ld",*thread_index,local_sum);
  Msg("Thread %d summation is %ld",*thread_index,local_sum);


  // TO DO: lock your global mutex
  // TO DO: update the global sum
  // TO DO: unlock your global mutex
  // TO DO: exit the thread


  pthread_mutex_lock(&sum_mutex);
  global_sum += local_sum;
  pthread_mutex_unlock(&sum_mutex);



  pthread_exit(NULL);

}//end doit


// function main ----------------------------------------
int main(int argc,char *argv[]) {
  // TO DO: here go your locat variables
  int i,j;
  int* thread_index;
  long local_sum = 0;

  pthread_t* tid;



  //TO DO:  check args, argv if error, display message and terminate the program
  if (argc != 2){
      msg("wrong number of command line arguments");
      msg_exit("usage - assgn3 <number of threads>");

      //return 0;
    }
    //TO DO:  set the number of threats to be dispatched
    no_of_threads = atoi(argv[1]);

    //TO DO:  check it is between 2 and 10 inclusive, if not error and terminate
    if (no_of_threads < 2 || no_of_threads > 10){
      msg("wrong number of threads, must be 2 - 10");
      msg_exit("usage - assgn3 <number of threads>");

      //return 0;

    }


  create_log("assgn3.log");

  init_arr();

  //TO DO: determine no_of_items each thread is supposed to sum up
  if (ARRAYSIZE % no_of_threads == 0){
    //each thread has size/number_of_threads
    no_of_items = ARRAYSIZE/no_of_threads;

  }else{
    no_of_items = 1+(ARRAYSIZE/no_of_threads);
  }


  //TO DO: create dynamically array to hold the index of each dispatched thread
  //TO DO: create dynamically array to hold thread id of each dispatched thread

  thread_index = (int*)malloc(no_of_threads * sizeof(int));

  tid = (pthread_t*)malloc(no_of_threads * sizeof(pthread_t));



  //TO DO: in a loop, dispatch each thread to execute doit() (see pthread_create())
  //TO DO: and display and log message "dispatched thread XXX with tid = YYY"

  for(i = 0; i < no_of_threads; i++){

    thread_index[i] = i;
    pthread_create(&(tid[i]), NULL, doit, &thread_index[i]);
    msg("dispatched thread %d with tid = %u",thread_index[i],tid[i]);
    Msg("dispatched thread %d with tid = %u",thread_index[i],tid[i]);



//example:
//’dispatched thread 1 with tid = 47362362755392’
  }



  //TO DO:  wait for all threads to complete
  //TO DO:  display message "Overall sum = XXX"


  for(j = 0; j < no_of_threads; j++){
      pthread_join(tid[j], NULL);

  }
  msg("Overall sum = %ld", global_sum);



  //TO DO: sum up the array arr[]
  for(i = 0; i < ARRAYSIZE; i++){
    local_sum += arr[i];
  }

  //TO DO: and display the message "Check sum = XXX"
  msg("Check sum = %ld",local_sum);

  return 0;
}//end main
