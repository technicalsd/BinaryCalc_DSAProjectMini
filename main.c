#include<stdio.h> 
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "main.h"
int main() { 
    int ch = 0, base = 0, choice = 0;
    char s[50] = {};
    list L;
    init(&L);
    printf("Welcome to Binary Calculator:- ");
    while(1) {

        init(&L);

        printf("\n\n");
        printf("0. Exit\n");
        printf("1. Calculator\n");
        
        printf("Choice: ");
        scanf("%d", &ch);
        getchar();
        printf("\n");

        if(ch <= 0 || ch > 2) {
            exit(0);
        }
        else if(ch == 1) {
            gets(s);      
            infix_eval(s);      
        }
        }
    return 0;
}