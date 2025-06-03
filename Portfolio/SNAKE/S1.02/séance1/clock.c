/**
* @author Armel KERMANAC'H 1C1
* @version 1.0
* @date 18/11/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    clock_t begin = clock();
    for(int i=0; i<100000;i++){
        for(int j=0;j<100000;j++){
        }
    }
    clock_t end = clock();
    double tmpsCPU = ((end - begin)*1.0) / CLOCKS_PER_SEC;
    printf("Temps CPU = %.3f secondes\n",tmpsCPU);
    
    return EXIT_SUCCESS;
}