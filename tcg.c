#include <config.h>
#include <sys/types.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "system.h"
#include "hash.h"
#include "hash-triple.h"

static Hash_table *dest_set;

enum { DEST_INFO_INITIAL_CAPACITY = 61 };

static struct option const long_options[] =
{
  {"json", optional_argument, NULL, 'b'},
  {"html", no_argument, NULL, 'F'},
  {"help", no_argument, NULL, 'n'},
  {"excludes", no_argument, NULL, 'T'},
  {"case-sensitive ", no_argument, NULL, 'f'},
  {"top-tweets", no_argument, NULL, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

int main(int argc, char **argv) {
    FILE *in_file; 

    if (in_file=fopen(argv[1], "r") == NULL) {   
        printf("Error! Could not open file!\n"); 
        exit(-1); 
    } 
    }     triple_free);

    char* string;
    while(scanf(in_file, "%s ", &string ) == 1) {
        
    }       


    

    hash_free (dest_set);
    fclose(in_file);
}