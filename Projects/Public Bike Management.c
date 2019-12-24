#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define INFINITY 1000000
#define SIMPLE 0
#define COMPLEX 1
#define SPECIAL 2
typedef int Distance;
typedef int Station;
clock_t start,stop;
double ticks,totaltime,duration;         //global variables

/********************************************************************/
/*	struct GNode													*/
/*	store problem related data and Graph for City Map				*/
/*	G is typed Distance** and need to allocate space before using	*/
/********************************************************************/
typedef struct GNode *PtrToGNode;
struct GNode{
    int Max_Capacity, Num_Stations, Num_Roads, *Num_Bikes;
	Station Problem_Station; //The station that needs adjustment
    Distance **G; //Weighted Graph that stores the distance between each pair of stations
};
typedef PtrToGNode MGraph;

MGraph Read_Map(); //function to read the Graph in given way
Station* Bike_Management( MGraph Graph ); //Choose the best path in given way
int Bikes_Required( MGraph Graph, Station *path ); //Calculate the number of bikes needs to be taken out from the PBMC
int Bikes_Returned( MGraph Graph, Station *path ); //Calculate the number of bikes taken back to the PBMC
MGraph Generate_TestCase(int Num, int type); //Generate Random cases

int main()
{
	int mode = 0, scale, totalrun;
	printf("This is the test Sample for Public Bike Management Program.\nPlease Select the mode you want to run in:\n");
	printf("----------------------------------------------------------------------------------------------------------------\n");
	printf("0: normal mode(manual), 1: generate simple case, 2: generate complex case, 3: generate special case, OTHERS: quit\n");
	scanf("%d", &mode);
	while(mode >= 0 && mode <= 3) {
		MGraph Graph = NULL;
		if(mode == 0) {
			Graph = Read_Map(); //Read the Graph
			totalrun = 1;
		}
		else if(mode == 1) {
			printf("Then please input the Scale of the generated data and the total Run Times:\n");
			printf("Recomended Scale range: ( 1 --- 20000 )\n");
			scanf("%d %d", &scale, &totalrun);
			Graph = Generate_TestCase(scale, SIMPLE);
			printf("The Generated Data could lead to Program Collapse. Run again if such occasion occurred.\n");
			printf("When input a large Sacle of data, it could take up to 1 minute to funish the run.\n");
		}
		else if(mode == 2) {
			printf("Then please input the Scale of the generated data and the total Run Times:\n");
			printf("Recomended Scale range: ( 1 --- 5000 )\n");
			scanf("%d %d", &scale, &totalrun);
			Graph = Generate_TestCase(scale, COMPLEX);
			printf("The Generated Data could lead to Program Collapse. Run again if such occasion occurred.\n");
			printf("When input a large Sacle of data, it could take up to 1 minute to funish the run.\n");
		}
		else {
			printf("Then please input the Scale of the generated data (must be multiple of 3) and the total Run Times:\n");
			printf("Recomended Scale range: ( 1 --- 600 )\n");
			scanf("%d %d", &scale, &totalrun);
			Graph = Generate_TestCase(scale, SPECIAL);
			printf("The Generated Data could lead to Program Collapse. Run again if such occasion occurred.\n");
			printf("When input a large Sacle of data, it could take up to 1 minute to funish the run.\n");
		}
		
		start = clock();  
		Station* path = NULL;
		for(int i = 0; i < totalrun; i++) {
			free(path);
			path = Bike_Management(Graph); //Select the best path to adjust bikes
		}
		stop = clock();                        //stop the timer
		ticks = (double)(stop - start);        //return the number of clock ticks of elapsed processor time
		totaltime = ticks / CLK_TCK;           //CLK_TCK is a const which transform the ticks into millisecond and get the totaltime
		duration = totaltime / totalrun;       //calculate the running time of one test
		
		printf("%d %d", Bikes_Required(Graph, path), path[0]); //outprint the result
		int i = 1;
		while(path[i] != -1) {
			printf("->%d", path[i]);
			i++;
		}
		printf(" %d\n", Bikes_Returned(Graph, path));
		//outprint timer result
		if(mode > 0) {
			printf("Ticks:%lf\n",ticks);
			printf("Totaltime:%lf\n",totaltime);
			printf("Duration:%.15f\n",duration);  
		}
		printf("----------------------------------------------------------------------------------------------------------------\n");
		printf("0: normal mode(manual), 1: generate simple case, 2: generate complex case, 3: generate special case, other: quit\n");
		scanf("%d", &mode);
	}

	return 0;
}

MGraph Generate_TestCase(int Num, int type) {
    int i, j;
	MGraph Graph = (MGraph)malloc(sizeof(struct GNode));
	Graph->Max_Capacity = 10, Graph->Num_Stations = Num, Graph->Problem_Station = Num, Graph->Num_Roads = Num;
	Graph->Num_Stations++;
	Graph->Num_Bikes = (int*)calloc(Graph->Num_Stations,sizeof(int)); //allocate the space to store the number of bikes in each station
	Graph->G = (Distance**)calloc(Graph->Num_Stations,sizeof(Distance*)); //allocate the space for roads from each station to another
	for(i = 0; i < Graph->Num_Stations; i++) Graph->G[i] = (Distance*)calloc(Graph->Num_Stations,sizeof(Distance));
	for(i = 0; i < Graph->Num_Stations; i++) { //Initialize the Weighted Graph
		for(int j = 0; j < Graph->Num_Stations; j++) {
			Graph->G[i][j] = INFINITY;
		}
	}
	Graph->Num_Bikes[0] = 0; //the PBMC station is put 0
	for(i = 1; i < Graph->Num_Stations; i++){ //Input the number of bikes in each station 
		Graph->Num_Bikes[i] = (int)(rand()%11);
	}
	if(type == 2) {
		if((Graph->Num_Stations-1)%3 != 0) {
			printf("Not a Special Case, please input a multiple of 3.\n");
		}
		for(i = 0; i < (Graph->Num_Stations-1)/3; i++) {
			Graph->G[i*3][i*3+1] = Graph->G[i*3+1][i*3] = rand()%10 + 3;
			Graph->G[i*3][i*3+2] = Graph->G[i*3+2][i*3] = rand()%10 + 3;
			Graph->G[i*3+3][i*3+2] = Graph->G[i*3+2][i*3+3] = rand()%10 + 3;
			Graph->G[i*3+3][i*3+1] = Graph->G[i*3+1][i*3+3] = rand()%10 + 3;
		}
		return Graph;
	}
	for(i = 0; i < Graph->Num_Roads; i++) { //Input the values in the Weighted Graph
		Graph->G[i+1][i] = Graph->G[i][i+1] = rand()%10 + 3;
	}
	if(type == 1) {
		Distance Weight;
		for(i = 0; i < Graph->Num_Stations; i++) {
			for(j = i+1; j < Graph->Num_Stations; j++) {
				Weight = rand()%10 + 3;
				if(Weight > 1 && Weight < 7)
					Graph->G[j][i] = Graph->G[i][j] = Weight;
			}
		}
	}
	return Graph;
}

MGraph Read_Map() {
	int i;
	MGraph Graph = (MGraph)malloc(sizeof(struct GNode));
	scanf("%d %d %d %d", &Graph->Max_Capacity, &Graph->Num_Stations, &Graph->Problem_Station, &Graph->Num_Roads);
	Graph->Num_Stations++; //add the PBMC station
	Graph->Num_Bikes = (int*)calloc(Graph->Num_Stations,sizeof(int)); //allocate the space to store the number of bikes in each station
	Graph->G = (Distance**)calloc(Graph->Num_Stations,sizeof(Distance*)); //allocate the space for roads from each station to another
	for(i = 0; i < Graph->Num_Stations; i++) Graph->G[i] = (Distance*)calloc(Graph->Num_Stations,sizeof(Distance));
	for(i = 0; i < Graph->Num_Stations; i++) { //Initialize the Weighted Graph
		for(int j = 0; j < Graph->Num_Stations; j++) {
			Graph->G[i][j] = INFINITY;
		}
	}
	Graph->Num_Bikes[0] = 0; //the PBMC station is put 0
	for(i = 1; i < Graph->Num_Stations; i++){ //Input the number of bikes in each station 
		scanf("%d", &Graph->Num_Bikes[i]);
	}
	Station Source, Target;
	Distance Distance_Between;
	for(i = 0; i < Graph->Num_Roads; i++) { //Input the values in the Weighted Graph
		scanf("%d %d %d", &Source, &Target, &Distance_Between);
		Graph->G[Source][Target] = Distance_Between;
		Graph->G[Target][Source] = Distance_Between;
	}
	return Graph;
}

Station* Bike_Management( MGraph Graph ) {
	Station V, *Stack, *count, *dist, *previous, *path = NULL, *path_temp = NULL;
	//Allocate space for these arrays
	Stack = (Station*)calloc(Graph->Num_Stations,sizeof(Station)); //Each station that updated the distance array will be pushed into this Stack
	count = (Station*)calloc(Graph->Num_Stations,sizeof(Station)); //Document the minimum stations passed from PBMC to each station
	dist = (Station*)calloc(Graph->Num_Stations,sizeof(Station)); //Document the minimum distance from PBMC to each station
	previous = (Station*)calloc(Graph->Num_Stations,sizeof(Station)); //Document the previous station in the current minimum path of each station
	int front = 0; //Front of the Stack
	for(int i = 0; i < Graph->Num_Stations; i++){ //Initialize these arrays
		dist[i] = INFINITY;
		previous[i] = -1;
		count[i] = 0;
	}
	dist[0] = 0; //Initialize the PBMC station
	previous[0] = -1;
	count[0] = 0;
	Stack[front++] = 0; //Push the PBMC station into the Stack
	while(front) {
		V = Stack[--front]; //Pop the Stack
		for(int i = 0; i < Graph->Num_Stations; i++) { //Scan all V's connected stations
			if(Graph->G[V][i] < INFINITY) {
				if(dist[V] + Graph->G[V][i] < dist[i]) { //Case that takes shorter path
					dist[i] = dist[V] + Graph->G[V][i]; //Update the distance of that stations from PBMC
					count[i] = count[V] + 1;
					previous[i] = V; //Update the previous stations of station i
					Stack[front++] = i; //Push station i into the Stack
					if(i == Graph->Problem_Station) {
						free(path); //for a shorter path has just been found, free the previous path and establish a new path
						path = (Station*)calloc(count[Graph->Problem_Station] + 2,sizeof(Station));
						Station Way_Point = Graph->Problem_Station;
						for(int i = count[Graph->Problem_Station]; i >= 0; i--){
							path[i] = Way_Point;
							Way_Point = previous[Way_Point];
						}
						path[count[Graph->Problem_Station]+1] = -1;
					}
				}
				else if(dist[V] + Graph->G[V][i] == dist[i]) { //Case a path of equal length has been found 
					previous[i] = V; //Same update as last case
					count[i] = count[V] + 1;
					Stack[front++] = i;
					if(i == Graph->Problem_Station) { //Case when i also happens to be the Problem Station
						free(path_temp);
						path_temp = (Station*)calloc(count[Graph->Problem_Station] + 2,sizeof(Station)); //Establish a temporary path
						Station Way_Point = Graph->Problem_Station;
						for(int i = count[Graph->Problem_Station]; i >= 0; i--){
							path_temp[i] = Way_Point;
							Way_Point = previous[Way_Point];
						}
						path_temp[count[Graph->Problem_Station]+1] = -1;
						//printf("path_temp:%d, path:%d\n", Bikes_Required(Graph, path_temp), Bikes_Required(Graph, path));
						//test function
						if(Bikes_Required(Graph, path_temp) < Bikes_Required(Graph, path)) { //Compare the bikes needed from the PBMC
							free(path);
							path = path_temp;
							path_temp = NULL;
						}
						else if(Bikes_Required(Graph, path_temp) == Bikes_Required(Graph, path)) {
							if(Bikes_Returned(Graph, path_temp) < Bikes_Returned(Graph, path)) { //Compare the bikes taken back to the PBMC
								free(path);
								path = path_temp;
								path_temp = NULL;
							}
						}
					}
				}
			}
		}
	}
	free(Stack); //Recycle the space allocated during the function call
	free(count);
	free(dist);
	free(previous);
	free(path_temp);
	return path;
}

int Bikes_Required( MGraph Graph, Station *path ) {
	int Current_Count = 0, Total_Required = 0, i = 1;
	while(path[i] != -1) {
		if(Graph->Num_Bikes[path[i]] != Graph->Max_Capacity / 2) {
			Current_Count += Graph->Num_Bikes[path[i]] - Graph->Max_Capacity / 2;
			if(Current_Count < 0) { //Case that Current Number is below zero, adjust the Original Number to fit the case
				Total_Required -= Current_Count;
				Current_Count = 0;
			}
		}
		i++;
	}
	return Total_Required;
}

int Bikes_Returned( MGraph Graph, Station *path ) {
	int Current_Count, i = 1;
	Current_Count = Bikes_Required(Graph, path);
	while(path[i] != -1) {
		if(Graph->Num_Bikes[path[i]] != Graph->Max_Capacity / 2) {
			Current_Count += Graph->Num_Bikes[path[i]] - Graph->Max_Capacity / 2;
		}
		i++;
	}
	return Current_Count;
}
