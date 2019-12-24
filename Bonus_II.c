/**********************************************************************************************
 * Work in DS Bonus Test III
 * Arthor: Lunium Luk (GitHub): https://github.com/LuniumLuk/ZJU-DS-Projs-and-HWs.git
 * Final Edit Date: 2019/12/24
 * 
 * a program to solve 'Battle Over Cities - Hard Version'
 * by reading all the roads into the rank and use Insertion-Sort to make it unincreasing
 * then suppose each city being taken and use Kruskal algorithm to generate a Minimum Spinning Tree
 * to decide the max-cost of each city
 * problem: 
 * 1) when there's no spinning tree be made, put INFINITY to the max-cost of this city
 * 2) The BIGGEST problem: there has been a mistake when writing the Kruskal algorithm
 * when updating the visited[] of two disjoint connected components, there will cause a mistake that could not finish the update
 * which lead to serious problem passing the 3, 4 checkpoint. detailed problem please refer to the function comments below.
 * Anyway, to finally discover the problem, I write a simulation generator and generate cases from 5 cities to 20 cities, and use a
 * relatively "correct" Internet Version as comparison. And after 1-2 hours of search, a 10 cities version has found the problem,
 * after carefully examming what has lead to such difference, the mistake was found.
**********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define MAXN 501
#define MAXNS 250001
#define INFINITY 100000
#define min(a,b) (a)<(b)?(a):(b)

int visited[MAXN]; //Employed in Kruskal()
int VIP[MAXN]; //Store the Max-Cost of each city
int rank[MAXNS][3]; //Store the roads records, 0:weight, 1:city1, 2:city2
int N, M, COST;

void read();
void Kruskal( int V );
void Insertion_Sort();

int main() {
    int i, max = 0, count = 0;
    read();
    Insertion_Sort();
    for(i = 0; i < N; i++) {
        COST = 0;
        Kruskal(i);
        VIP[i] = COST;
    }
    for(i = 0; i < N; i++) {
        if(VIP[i] > max) {max = VIP[i], count = 1;}
        else if(VIP[i] == max) count++;
    }
    if(max == 0) printf("0");
    else for(i = 0; i < N; i++) if(VIP[i] == max) {
        printf("%d", i+1);
        if(--count) printf(" ");
    }
    printf("\n");
}

/*Read the records, <update: N, M, rank>*/
void read() {
    if(scanf("%d%d", &N, &M) == 2);
    else printf("failed to input data!\n");
    int i, V1, V2, d, s;
    for(i = 0; i < M; i++) {
        if(scanf("%d%d%d%d", &V1, &V2, &d, &s) == 4);
        else printf("failed to input data!\n");
        if(s) rank[i][0] = 0;
        else rank[i][0] = d;
        rank[i][1] = V1-1;
        rank[i][2] = V2-1;
    }
}

/*Kruskal Algorithm, V is the City that has been taken, <update: COST, visited>*/
void Kruskal( int V ) {
    int i, j, t1, t2, count = N-2, temp;
    for(i = 0; i < N; i++) visited[i] = 0;
    i = 0;
    while(count && i < M) {
        if(rank[i][1] != V && rank[i][2] != V) {
            t1 = rank[i][1];
            t2 = rank[i][2];
            if(visited[t1] == 0 && visited[t2] == 0) { //when both vertex has not been visited
                count--;
                visited[t1] = visited[t2] = t1+10;
                COST += rank[i][0];
            }
            //the folowing two case means one vertex has been visited, hence it is already in a connected component
            else if(visited[t1] == 0) {count--; visited[t1] = visited[t2]; COST += rank[i][0];}
            else if(visited[t2] == 0) {count--; visited[t2] = visited[t1]; COST += rank[i][0];}
            //the folowing case means that we have encountered two disjoint connected components
            else if(visited[t1] != visited[t2]) {
                temp = visited[t2]; //this is what has caused the problem
                //originally, I directedly employed visited[t2] in the comparison below
                //however, when visited[t2] itself has been updated, then any vertex > t2 will not be updated correctedly
                for(j = 0; j < N; j++) if(visited[j] == temp) {visited[j] = visited[t1];}
                count--;
                COST += rank[i][0];
            }
        }
        i++;
    }
    if(count) COST = INFINITY;
}

/*Insertion Sort on rank, <update: rank>*/
void Insertion_Sort() {
    int i, j, td, t1, t2;
    for(i = 0; i < M; i++) {
        td = rank[i][0], t1 = rank[i][1], t2 = rank[i][2];
        for(j = i-1; j >= 0; j--) {
            if(rank[j][0] > td) {
                rank[j+1][0] = rank[j][0];
                rank[j+1][1] = rank[j][1];
                rank[j+1][2] = rank[j][2];
            }
            else break;
        }
        rank[j+1][0] = td;
        rank[j+1][1] = t1;
        rank[j+1][2] = t2;
    }
}
