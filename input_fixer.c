#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char expr[100];
    int len, i,j;

    printf("Enter an expression: ");
    fgets(expr, sizeof(expr), stdin);
    len = strlen(expr);
    
    for(i = 0; i < len; i++) {
        if(expr[i] == ' ') {
            for(j = i; j < len; j++) {
                expr[j] = expr[j+1];
            }
            len--;
            i--;
        }
    }


    for (i = 0; i < len; i++) {
        if (isdigit(expr[i])) {
            printf("%c", expr[i]);
            while (isdigit(expr[i+1])) {
                i++;
                printf("%c", expr[i]);
            }
            printf(" ");
        } else if (isalpha(expr[i])) {
            printf("%c", expr[i]);
            while (isalpha(expr[i+1])) {
                i++;
                printf("%c", expr[i]);
            }
            printf(" ");
        } else if (ispunct(expr[i])) {
            printf("%c ", expr[i]);
        } else if (isspace(expr[i])) {
            continue;
        }
    }

    return 0;
}
