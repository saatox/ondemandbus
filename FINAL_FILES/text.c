#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>  
#include <time.h>  

#define MAX 60
#define DISTANCE 2147483647
#define MPM 500


int count=0;
double array[MAX][MAX];
int patrol[MAX][MAX][MAX];
double rqtime[MAX];
double acctime[MAX];
int route[MAX];

double initial_worst=0;
double worst_time=0;

void my_init() {
    srand(time(NULL));
}
void insertionSort(double arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        // Move elements of arr[0..i-1],
        // that are greater than key,
        // to one position ahead of their
        // current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


int main(void){
    int i=0;
    my_init();
    double temp=0;
    int first=0;
    int pickup=0, dropoff=0;
    double new_time=0;
    int passengers=0;

    scanf("%d", &count);
    printf("%d\n ", count);
    int new[count];

    for(i=0; i<count; i++){
        for(int j=0; j<count; j++){
            scanf(" %lf  ", &array[i][j]);
        }
    }

    

    for(i=0; i<count; i++){
        for(int j=0; j<count; j++){
            printf("%lf ", array[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    for(i=0; i<count; i++){
        for(int j=0; j<count; j++){
            for (int m=0; m<count; m++){
                scanf(" %d  ", &patrol[i][j][m]);
                if(patrol[i][j][m]==1000){
                    break;
                }
            }
            
        }
    }
    for(i=0; i<count; i++){
        for(int j=0; j<count; j++){
            for (int m=0; m<count; m++){
                printf("%d ", patrol[i][j][m]);
                if(patrol[i][j][m]==1000){
                    printf("\n");
                    break;
                }
            }
        }
    }
    printf("\n");

    scanf(" %d  ", &passengers);
    printf("%d\n ", passengers);

    for(i=0; i<passengers; i++){
        rqtime[i]=rand()%500;
    }
    insertionSort(rqtime, passengers);


    for(i=0; i<passengers; i++){
        pickup=rand()%count;
        dropoff=rand()%count;
        while(pickup==dropoff){
            pickup=rand()%count;
            dropoff=rand()%count;
        }
        printf("%d %d %f\n", pickup, dropoff, rqtime[i]);
    }
    printf("\n");

    return 0;

}