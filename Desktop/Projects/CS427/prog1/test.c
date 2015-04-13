#include <stdio.h>

char buffer[8];
char bob[9];


void print_bits(unsigned int x)
{
    int i;
    for(i=8*sizeof(x)-1; i>=0; i--) {
        (x & (1 << i)) ? putchar('1') : putchar('0');
    }
    printf("\n");
}



int main(void){
//int key = 8;
//int y = (key << 1) | (key >> (sizeof(int)*8 - 1));


//print_bits(key);
//printf("%lu\n", sizeof(int));
//print_bits(key >> (31));
//print_bits(y);
FILE *plaintext;
plaintext = fopen("plaintext.txt", "r");
fread(buffer, 8, 1, plaintext);
fclose(plaintext);
printf("bob: %s\n", buffer);
return 0;
}