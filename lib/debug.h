/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * 
 * Description : Declaration de macros pour avoir un debogage plus efficace avec la transmission UART. Permet de passer 
 * plusieurs variables du meme style qu'un printf("%d %d %d", var1 , var2 , var3 ...)
 *  
 */

#include "uart.h"
#include <stdio.h>
#include <string.h>
#ifdef DEBUG
UART uart;
int n;
char buffer[1000];
#define DEBUG_PRINT(x, ...)                \
    n = sprintf(buffer, x, ##__VA_ARGS__); \
    for (int i = 0; i < n; ++i)            \
    {                                      \
        uart.transmissionUART(buffer[i]);  \
    }

#define DEBUG_PRINT_INT(x) uart.transmissionUART((char)x)
#else
#define DEBUG_PRINT(x, ...) \
    do                      \
    {                       \
    } while (0);
#endif