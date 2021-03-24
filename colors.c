#include <stdio.h>
#include "colors.h"

void Blue(){
    printf("\033[1;34m");
}

void Reset(){
    printf("\033[0m");
}

void Green(){
    printf("\033[0;32m");
}

void Yellow(){
    printf("\033[0;33m");
}

void Red(){
    printf("\033[0;31m");
}
