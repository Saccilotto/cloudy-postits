#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <sys/types.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "stdbool.h"

#define MAX_NUMBER 300
#define MAX_CHAR 48

#define FATAL_ERROR(Message)	    \
do									\
{									\
    error (0, 0, (Message));	    \
    usage (EXIT_FAILURE);			\
} while (0) 

void * malloc (size_t size);

enum operating_mode
{
unknown,

/* Output characters that are in the given bytes. */
byte_status,

/* Output the given delimiter-separated fields. */
field_status
};

void
usage (int status) {
    if (status != 0) {
    printf("Try tag cloud app's --help command for more information on functionalities. \n");
    } else {
        printf ("\
        don`t forget to execute make command before executing tcg\n\
        Usages: ./tcg [OPTION] [FILE] \n\
                ./tcg [OPTION1] [OPTION2] [FILE]\n\
        ");
        printf ("\nThe exit format output is on CSV or HTML format.\
        It shows the result of the most ocurred words on the read file.\n");

        printf ("\
        \n\
        The following commands can be used:\n\
        -csv                                Most word occurencies output in CSV format \n\
        -html                               Most word occurencies output in HTML format \n\
        -html -color                        Most word occurencies output in HTML format and color \n\
        -csv -range [number](i.e: 5)        Most word occurencies output in CSV format with most used range of words\n\
        ");
        printf ("--help                       Show help description \n");
        printf ("--version                    Inform the current app version\n");
    }
    exit (status);
}

void
version (int status) {
  if (status != 0) {
    printf("Try using tagCloud --help for displaying help menu");
  } else {
      printf ("Version 0.1 \nThis file is free software; the Free Software Foundation\
            gives unlimited permission to copy and/or distribute it,\
            with or without modifications, as long as this notice is preserved.\n"
        );
    }
  exit (status);
}

char** read_from_file (char* filename, int *index) { 
    char **words = malloc (MAX_NUMBER * sizeof *words);
    char bufferer[MAX_CHAR] = {0};
    FILE *file = fopen (filename, "r");
    if (file == NULL) { 
        fprintf (stderr, "Can't open file\nTry using --help to display the help menu\n");
        exit(1);
    }
    *index = 0;

    while (fgets (bufferer, MAX_CHAR, file)) {
        char *p = bufferer;
        size_t len = strlen (p);
        while (len && (p[len-1] == '\r' || p[len-1] == '\n'))
            p[--len] = 0;

        words[(*index)++] = strdup (bufferer);

        if (*index == MAX_NUMBER) {
            fprintf (stderr, "warning: MAX_NUMBER words read.\n");
            break;
        }
    }
    fclose(file);
    return words;
}

struct allWords {
    char word[30];
    int size;
} fileWords[10001];

bool addWord(char* readWord, int cnt) {
    if (cnt > 10000) {
        printf("\n Number of characters exceeded the maximum number of 10000 \n");
        exit(EXIT_FAILURE);
    }
    
    if (cnt == 0) {
        strcpy(fileWords[0].word, readWord);
        fileWords[0].size = 1;
    }
    else {
        for (size_t i = 0; i < cnt; i++) {
            if (strcmp(fileWords[i].word , readWord) == 0 ) {
                fileWords[i].size = fileWords[i].size + 1;
                return true;
            }
        }
        strcpy(fileWords[cnt++].word, readWord);
        fileWords[cnt++].size = 1;
        return false;
    }
}

int readText (char** words, int lineNum) {
    int j = 0;
    int cnt = 0;
    char * pch;
    for (j = 0; j < lineNum; j++) {
        pch = strtok (words[j]," ;,.-");
        while (pch != NULL) {
            bool verifica = addWord(pch, cnt);
            if (verifica == false) {
                cnt ++;
            }
            pch = strtok (NULL, " ,.-");
        }
    }
    return cnt;
}

void writeCsv (int cnt) {
    FILE* fileCsv = fopen ("words_occurencies.csv", "w");

    fputs("word, Number of Occurencies \r\n", fileCsv);
    for (size_t i = 0; i < cnt; i++) {
        fprintf(fileCsv, "%s, %i\n", fileWords[i].word, fileWords[i].size);
    }
    fclose(fileCsv);
}

void writeCsvLimitwords (int cnt) {
    FILE* fileCsv = fopen ("words_occurencies_limited.csv", "w");

    fputs("word, Number of Occurencies \r\n", fileCsv);
    for (size_t i = 0; i < cnt; i++) {
        fprintf(fileCsv, "%s, %i\n", fileWords[i].word, fileWords[i].size);
    }
    fclose(fileCsv);
}

void html_no_color(int cnt) {
    FILE* fileHTML = fopen ("web_no_color.html", "w");
    if (cnt < 40) {
        if (cnt == 0) {
            printf("No words to be written.");
        }
        if (cnt == 1) {
            fprintf(fileHTML, "<html> \n");
            fprintf(fileHTML, "<title> Words and Number of Ocurrencies </title> \n");
            fprintf(fileHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"uncolored.css\">\n");
            fprintf(fileHTML, "</head>\n");
            fprintf(fileHTML, "<body> \n");
            fprintf(fileHTML, "<div class=\"words\"> \n");
            fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[0].word);
            fprintf(fileHTML, "</div>");
            fprintf(fileHTML, "</body>");
            fprintf(fileHTML, "</html>");       
        }
        if (cnt > 1) {
            fprintf(fileHTML, "<html> \n");
            fprintf(fileHTML, "<title> Words and Number of Ocurrencies </title> \n");
            fprintf(fileHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"uncolored.css\">\n");
            fprintf(fileHTML, "</head>\n");
            fprintf(fileHTML, "<body> \n");
            fprintf(fileHTML, "<div class=\"words\"> \n");
            for (size_t i = 1; i <= cnt/2; i++) {
                fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[i].word);
            }
            fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[0].word);
            for (size_t i = cnt/2+1; i < cnt; i++) {
                fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[i].word);
            }
            fprintf(fileHTML, "</div>");
            fprintf(fileHTML, "</body>");
            fprintf(fileHTML, "</html>"); 
        }
    }
    else {
        fprintf(fileHTML, "<html> \n");
        fprintf(fileHTML, "<title> Words and Number of Ocurrencies </title> \n");
        fprintf(fileHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"uncolored.css\">\n");
        fprintf(fileHTML, "</head>\n");
        fprintf(fileHTML, "<body> \n");
        fprintf(fileHTML, "<div class=\"words\"> \n");
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[15].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[12].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[16].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[17].word);
        fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[0].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[18].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[19].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[26].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[10].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[27].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[28].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[14].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[10].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[29].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[4].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[30].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[31].word);
        fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[1].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[32].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[5].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[33].word);
        fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[2].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[34].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[35].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[6].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[36].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[37].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[38].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[7].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[39].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[40].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[8].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[9].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[11].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[21].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[22].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[3].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[20].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[23].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[24].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[25].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[13].word);
    }
    fclose(fileHTML);
}

void with_color_html(int cnt) {
    FILE* fileHTML = fopen ("web_color.html", "w");
    if (cnt < 40) {
        if (cnt == 0) {
            printf("No words to be written.");
        }
        if (cnt == 1) {
            fprintf(fileHTML, "<html> \n");
            fprintf(fileHTML, "<title> Words and Number of Ocurrencies </title> \n");
            fprintf(fileHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"design.css\">\n");
            fprintf(fileHTML, "</head>\n");
            fprintf(fileHTML, "<body> \n");
            fprintf(fileHTML, "<div class=\"words\"> \n");
            fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[0].word);
            fprintf(fileHTML, "</div>");
            fprintf(fileHTML, "</body>");
            fprintf(fileHTML, "</html>");       
        }
        if (cnt > 1) {
            fprintf(fileHTML, "<html> \n");
            fprintf(fileHTML, "<title> Words and Number of Ocurrencies </title> \n");
            fprintf(fileHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"design.css\">\n");
            fprintf(fileHTML, "</head>\n");
            fprintf(fileHTML, "<body> \n");
            fprintf(fileHTML, "<div class=\"words\"> \n");
            for (size_t i = 1; i <= cnt/2; i++)
            {
                fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[i].word);
            }
            fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[0].word);
            for (size_t i = cnt/2+1; i < cnt; i++)
            {
                fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[i].word);
            }
            fprintf(fileHTML, "</div>");
            fprintf(fileHTML, "</body>");
            fprintf(fileHTML, "</html>"); 
        }
    }
    else {
        fprintf(fileHTML, "<html> \n");
        fprintf(fileHTML, "<title> Words and Number of Ocurrencies </title> \n");
        fprintf(fileHTML, "<link rel=\"stylesheet\" type=\"text/css\" href=\"design.css\">\n");
        fprintf(fileHTML, "</head>\n");
        fprintf(fileHTML, "<body> \n");
        fprintf(fileHTML, "<div class=\"words\"> \n");
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[15].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[12].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[16].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[17].word);
        fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[0].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[18].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[19].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[26].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[10].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[27].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[28].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[14].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[10].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[29].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[4].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[30].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[31].word);
        fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[1].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[32].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[5].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[33].word);
        fprintf(fileHTML, "<span class=\"text5\">%s</span> \n", fileWords[2].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[34].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[35].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[6].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[36].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[37].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[38].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[7].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[39].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[40].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[8].word);
        fprintf(fileHTML, "<span class=\"text3\">%s</span> \n", fileWords[9].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[11].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[21].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[22].word);
        fprintf(fileHTML, "<span class=\"text4\">%s</span> \n", fileWords[3].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[20].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[23].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[24].word);
        fprintf(fileHTML, "<span class=\"text1\">%s</span> \n", fileWords[25].word);
        fprintf(fileHTML, "<span class=\"text2\">%s</span> \n", fileWords[13].word);
        fprintf(fileHTML, "</div>");
        fprintf(fileHTML, "</body>");
        fprintf(fileHTML, "</html>");
    }
    fclose(fileHTML);
}

void sort_words_occurencies(int cnt) {
    int i, j, aux;
    if (cnt == 0) {
        return;
    }
    for( i=0; i<cnt; i++ ) {
        for( j=i; j<cnt; j++ ) {
            if( fileWords[i].size < fileWords[j].size ) {
                 fileWords[10001] = fileWords[i];
                 fileWords[i] = fileWords[j];
                 fileWords[j] = fileWords[10001];
            }
        }
    }
}


int
main (int argc, char *argv[]) {
    bool ok;
    char letters[100];
    char* words[20000];
    bool isCsv;
    bool isHtml;
    bool isHtmlColor;
    bool isCsvRange;
    int i = 0;
    int opt;
    int sizeWord;
    int numwords;
    size_t nfiles;
    char* filename;
    FILE *file;
    struct fstatus *fstatus;

    setvbuf (stdout, NULL, _IOLBF, 0);
    int ret;
    ret = strcmp(argv[1] , "--help");
    if (argc > 1) {
        if (strcmp(argv[1] , "--help") == 0) {
            opt = 0;
        }
        if (strcmp(argv[1] , "--version") == 0) {
            opt = 1;
        }
        if (argc >= 2) {          
            if (strcmp(argv[1] , "-csv") == 0) {
                opt = 2;
            }
            if (strcmp(argv[1] , "-html") == 0) {
                opt = 3;
            }
            if (strcmp(argv[1] , "-html") == 0 && strcmp(argv[2] , "-color") == 0) {
                opt = 4;
            }
            if (strcmp(argv[1] , "-csv") == 0 && strcmp(argv[2] , "-range") == 0 && argc >= 4) {
                opt = 5;
            }            
        }       
    }

    switch (opt) {
        case 0: 
        usage(0);
        break;

        case 1: version(0);
        break;

        case 2: isCsv = true;
        break;

        case 3: isHtml = true;
        break;

        case 4: isHtmlColor = true;
        break;

        case 5: isCsvRange = true;
        break;  

        default:
        usage(1);
    }

    if (isCsv == true) {   
        filename = argv[2];
        char **words =  read_from_file(filename, &numwords);

        int count_all = readText(words, numwords);
        
        sort_words_occurencies(count_all);
        writeCsv(count_all);

        for (i = 0; i < numwords; i++)
        free (words[i]);
        free (words);

        printf("CSV Tag cloud output sucessfull!! \n");
    }
    if (isHtml == true) {   
        filename = argv[2];
        char **words = read_from_file(filename, &numwords);

        int count_all = readText(words, numwords);

        sort_words_occurencies(count_all);
        
        html_no_color(count_all);

        for (i = 0; i < numwords; i++)
        free (words[i]);
        free (words);

        printf("HTML Tag cloud output sucessfull!!\n");
    }
    if (isHtmlColor == true) {   
        filename = argv[3];
        char **words =  read_from_file(filename, &numwords);

        int count_all = readText(words, numwords);

        sort_words_occurencies(count_all);
        
        with_color_html(count_all);

        for (i = 0; i < numwords; i++)
        free (words[i]);
        free (words);

        printf("colorfull HTML Tag cloud output sucessfull!!\n");
    }
    if (isCsvRange == true) {   
        filename = argv[4];
        char **words =  read_from_file(filename, &numwords);

        int count_all = readText(words, numwords);
        int range = atoi(argv[3]);
        sort_words_occurencies(count_all);
        if (range < count_all)
        {
            count_all = range;
        }
        printf("%i",count_all);
        writeCsvLimitwords(count_all);

        for (i = 0; i < numwords; i++)
        free (words[i]);
        free (words);

        printf("CSV Tag cloud with limited words range output sucessfull!! \n");
    }
}
