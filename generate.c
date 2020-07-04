#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SELF 20

int temp[100][4];

int main() {
    int i, j, N = 10, M = 0, d, s, e;
    for(e = 0; e < 30; e++) {
        M = 0;
        for(i = 0; i < N; i++) for(j = i+1; j < N; j++) {
            d = rand()%4, s = rand()%2;
            if(d) {
                temp[M][0] = i+1;
                temp[M][1] = j+1;
                temp[M][2] = d;
                temp[M][3] = s;
                M++;
            }
        }
        printf("%d %d\n", N, M);
        for(i = 0; i < M; i++) {
            printf("%d %d %d %d\n", temp[i][0], temp[i][1], temp[i][2], temp[i][3]);
        }
    }

}