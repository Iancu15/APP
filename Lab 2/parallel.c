#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
    FILE *file;
    char file_path[50];
    char ch;
    int i;
    int char_freqs[26];

    for (i = 0; i < 26; i++) {
        char_freqs[i] = 0;
    }

    #pragma omp parallel shared(char_freqs) private(i, file_path, file, ch)
    {
        #pragma omp for reduction(+:char_freqs)
        for (i = 1; i <= 100; i++) {
            sprintf(file_path, "files/f%d", i);
            file = fopen(file_path, "r");
            while (!feof(file)) {
                ch = fgetc(file);
                if (ch >= 'A' && ch <= 'Z') {
                    char_freqs[ch - 'A'] += 1;
                }
            }
        }
    }

    for (i = 0; i < 26; i++) {
        printf("%c: %d\n", 'A' + i, char_freqs[i]);
    }
}