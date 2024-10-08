/* File:  
 *    omp_tokenize_r.c
 *
 * Purpose:
 *    Try to use threads to tokenize text input.  This version
 *    uses the thread safe tokenizer strtok_r.
 *
 * Compile:
 *    gcc -g -Wall -fopenmp -o omp_tokenize_r omp_tokenize_r.c
 * Usage:
 *    omp_tokenize_r <thread_count> < <input>
 *
 * Input:
 *    Lines of text
 * Output:
 *    For each line of input:
 *       the line read by the program, and the tokens identified by 
 *       strtok
 *
 * Algorithm:
 *    For each line of input, next thread reads the line and
 *    "tokenizes" it.
 *
 * IPP:  Section 5.10 (p. 258)
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

const int MAX_LINES = 1000;
const int MAX_LINE = 80;

void Usage(char* prog_name);
void Get_text(char* lines[], int* line_count_p);
void Tokenize(char* lines[], int line_count, int thread_count); 
void Tokenize_copy(char*  lines[], int line_count, int thread_count );

int main(int argc, char* argv[]) {
   int thread_count, i;
   char* lines[MAX_LINES];
   int line_count;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   printf("Enter text\n");
   Get_text(lines, &line_count);
   // Tokenize(lines, line_count, thread_count);
   Tokenize_copy(lines, line_count, thread_count);

   for (i = 0; i < line_count; i++)
      if (lines[i] != NULL) free(lines[i]);

   return 0;
}

void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
} 

void Get_text(char* lines[], int* line_count_p) {
   char* line = malloc(MAX_LINE*sizeof(char));
   int i = 0;
   char* fg_rv;

   fg_rv = fgets(line, MAX_LINE, stdin);
   while (fg_rv != NULL) {
      lines[i++] = line;
      line = malloc(MAX_LINE*sizeof(char));
      fg_rv = fgets(line, MAX_LINE, stdin);
   }
   *line_count_p = i;
}  


void Tokenize(
      char*  lines[]       /* in/out */, 
      int    line_count    /* in     */, 
      int    thread_count  /* in     */) {
   int my_rank, i, j;
   char *my_token, *saveptr;

#  pragma omp parallel num_threads(thread_count) \
      default(none) private(my_rank, i, j, my_token, saveptr) \
      shared(lines, line_count)
   {
      my_rank = omp_get_thread_num();
#     pragma omp for schedule(static, 1)
      for (i = 0; i < line_count; i++) {
         printf("Thread %d > line %d = %s", my_rank, i, lines[i]);
         j = 0; 
         my_token = strtok_r(lines[i], " \t\n", &saveptr);
         while ( my_token != NULL ) {
            printf("Thread %d > token %d = %s\n", my_rank, j, my_token);
            my_token = strtok_r(NULL, " \t\n", &saveptr);
            j++;
         } 
      if (lines[i] != NULL) 
         printf("Thread %d > After tokenizing, my line = %s\n",
            my_rank, lines[i]);
      } /* for i */
   }  /* omp parallel */

}  /* Tokenize */

void Tokenize_copy(char* lines[], int line_count, int thread_count) {
    int my_rank, i, j;
    char *my_token, *saveptr;
    char *copy;
    char *line_copy;

    #pragma omp parallel num_threads(thread_count) \
        default(none) private(my_rank, i, j, my_token, saveptr, copy, line_copy) \
        shared(lines, line_count)
    {
        my_rank = omp_get_thread_num();

        #pragma omp for schedule(static, 1)
        for (i = 0; i < line_count; i++) {
            // Criar uma cópia local da linha para evitar concorrência
            line_copy = strdup(lines[i]); // Copia a linha original

            copy = line_copy;
            printf("Thread %d > line %d = %s", my_rank, i, copy);
            j = 0;
            my_token = strtok_r(copy, " \t\n", &saveptr);
            while (my_token != NULL) {
                printf("Thread %d > token %d = %s\n", my_rank, j, my_token);
                my_token = strtok_r(NULL, " \t\n", &saveptr);
                j++;
            }

            if (line_copy != NULL) {
                printf("Thread %d > After tokenizing, my line = %s\n", my_rank, copy);
                free(line_copy); // Liberar a memória alocada
            }
        }
    }
}