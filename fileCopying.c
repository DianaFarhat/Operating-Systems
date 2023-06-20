#include <unistd.h>
#include <stdio.h>

#define NAME_LENGTH 25

int main(void) {
    char c, inFile[NAME_LENGTH], outFile[NAME_LENGTH];
    FILE *in, *out;

    printf("Enter source file name: ");
    scanf("%s", inFile);

    printf("Enter destination file name: ");
    scanf("%s", outFile);

    if ((in = fopen(inFile, "r")) == NULL) {
        fprintf(stderr, "Cannot open %s for reading\n", inFile);
        return 1;
    }

    if ((out = fopen(outFile, "w")) == NULL) {
        fprintf(stderr, "Cannot open %s for writing\n", outFile);
        fclose(in);
        return 1;
    }

    while ((c = getc(in)) != EOF) {
        putc(c, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}
