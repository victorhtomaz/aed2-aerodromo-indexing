#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* separar_string(char **str, char *separador){
    char *inicio, *final;

    if (str == NULL || *str == NULL) {
        return NULL;
    }

    inicio = *str;
    final = strpbrk(inicio, separador);

    if (final != NULL) {
        *final = '\0';
        *str = final + 1;
    } else {
        *str = NULL;
    }

    return inicio;
}