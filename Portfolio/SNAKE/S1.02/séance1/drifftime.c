/**
* @author Armel KERMANAC'H 1C1
* @version 1.0
* @date 18/11/2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    time_t debut = time(NULL);
    for (int i=0;i<100000;i++){
        for(int j=0;j<100000;j++){
        }
    }
    time_t fin = time(NULL);
    printf("duree = %.3f secondes\n",difftime(fin,debut));

    return EXIT_SUCCESS;
}