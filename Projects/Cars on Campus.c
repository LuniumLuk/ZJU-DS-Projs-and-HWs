#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define cutoff 20//this is the number of elements deciding whether using quick sort or insertion sort

typedef struct info* PtrToInfo;
struct info{
	char name[8];//save car's name
	int time;//save car's in/out time
	int status;//show car's status, 1 means in, 0 means out
};

int n,k;//n is the number of records, k is the number of queries
int num,count=0;//num is the number of cars, count is the number of cars in school at a specific time 
int *cartime;//this will be an array saving each car's total time in school

void read(PtrToInfo record, int* checkpoint);// read in the records
void quicksort_name(PtrToInfo record, int left ,int right);//quicksort by car's name
void insertion_name(PtrToInfo record, int left, int right);//insertion sort by car's name
void quicksort_time(PtrToInfo record, int left, int right);//qucksort by car's time for each car
void insertion_time(PtrToInfo record, int left, int right); //insertion sort by car's name for each car
void sort_time(PtrToInfo record, int* car);//a function sort car's time and get the result
void task(PtrToInfo record, int* car, int* checkpoint);//a function calculate car's number in each time and get each car's total time in school
void findmost(PtrToInfo record, int* car);//a function finding the max total time and related cars

int main()
{
	PtrToInfo record;
	int i;
	int *car;//it will be an array saving each car's first record in the record
	int *checkpoint;//it will be an array saving each checkpoint
	
	scanf("%d%d",&n,&k);
	record = (PtrToInfo)malloc(n*sizeof(struct info));//dynamic memory allocation for each record
	checkpoint = (int*)malloc((k+1)*sizeof(int));//dynamic memory allocation for each checkpoint
	
	read(record,checkpoint);//input record
	quicksort_name(record,0,n-1);//sort by name
	
	car = (int*)malloc((n+1)*sizeof(int));//dynamic memory allocation for car's first record in the record
	sort_time(record,car); //sort by each car's time
	
	cartime = (int*)malloc(num*sizeof(int));//dynamic memory allocation for each car's total time in school
	task(record,car,checkpoint);//check car's number at each time and find out each car's staying time
	findmost(record,car);// compare cars' staying time, find the most ones and names of related cars
	
	system("pause");
	 
	return 0;
}

void read(PtrToInfo record, int* checkpoint)
{
	int i,h,min,s;
	char temp[3];
	for(i=0;i<n;i++){
		scanf("%s %d:%d:%d %s",record[i].name,&h,&min,&s,temp);
		record[i].time = h*3600+min*60+s;
		if(strcmp(temp,"in")==0)
			record[i].status = 1;
		else
			record[i].status = 0;
	}
	for(i=0;i<k;i++){
		scanf("%d:%d:%d",&h,&min,&s);
		checkpoint[i] = h*3600+min*60+s;//change time to second
	}
	checkpoint[k] = 24*3600;//add a checkpoint "24:00:00" to make it easy to  
}							//calculate car's total time and check every point together

void quicksort_name(PtrToInfo record, int left, int right)
{
	int i=left,j=right;
	char pivot[8];
	struct info tempinfo=record[left];//save the element used to compare
	if(left+cutoff>=right)
		insertion_name(record,left,right);//if the number of elements is less than 21, insertion sort is better
	else{
		strcpy(pivot,record[left].name);
		while(i<j){
			while(i<j&&strcmp(pivot,record[j].name)<=0)
				j--;
			record[i] = record[j];
			while(i<j&&strcmp(pivot,record[i].name)>=0)
				i++;
			record[j] = record[i];
		}
		record[i] = tempinfo;
		quicksort_name(record,left,i-1);//sort the left part
		quicksort_name(record,i+1,right);//sort the right part
	}
}

void insertion_name(PtrToInfo record, int left, int right)
{
	int i,j;
	struct info temp;
	for(i=left;i!=right&&strcmp(record[i].name,record[i+1].name)<0;i++);//find the first record needed sorting
	if(i!=right){
		for(;i<=right;i++){
			for(j=i-1;j>=left;j--){
				if(strcmp(record[j].name,record[j+1].name)>0){
					temp = record[j];
					record[j] = record[j+1];
					record[j+1] = temp;
				}
			}
		}
	}
}

void quicksort_time(PtrToInfo record, int left, int right)
{
	int i=left,j=right,pivot;
	struct info tempinfo=record[left];//save the element used to compare
	if(left+cutoff>=right)
		insertion_time(record,left,right);//if the number of elements is less than 21, insertion sort is better
	else{
		pivot = record[left].time;
		while(i<j){
			while(i<j&&pivot<=record[j].time)
				j--;
			record[i] = record[j];
			while(i<j&&pivot>=record[i].time)
				i++;
			record[j] = record[i];
		}
		record[i] = tempinfo;
		quicksort_time(record,left,i-1);//sort the left part
		quicksort_time(record,i+1,right);//sort the right part
	}
}

void insertion_time(PtrToInfo record, int left, int right)
{
	int i,j;
	struct info temp;
	for(i=left;i!=right&&record[i].time<record[i+1].time;i++);//find the first record needed sorting
	if(i!=right){
		for(;i<=right;i++){
			for(j=i-1;j>=left;j--){
				if(record[j].time>record[j+1].time){
					temp = record[j];
					record[j] = record[j+1];
					record[j+1] = temp;
				}
			}
		}
	}
}

void sort_time(PtrToInfo record, int* car)
{
	int i;
	car[0] = 0;
	num = 1;
	for(i=0;i<n-1;i++){
		if(strcmp(record[i].name,record[i+1].name)!=0){//find the boundary of different cars
			car[num] = i+1;//save each car's first record in the record
			quicksort_time(record,car[num-1],car[num]-1);//sort each car's time
			num++;//save the number of cars
		}
	}
	car[num] = n;//add a car's virtual first place 'car[num]' to make it easy to  
			     //calculate car's total time and check every point together
	quicksort_time(record,car[num-1],car[num]-1);//sort the last car's time
}

void task(PtrToInfo record, int* car, int* checkpoint)
{
	int *ri,ci,chi;
	ri = (int*)malloc((num+1)*sizeof(int));//dynamic memory allocation for each ri 
										   //ri shows each car's record we'll check next
	for(ci=0;ci<=num;ci++){
		ri[ci] = car[ci];
		cartime[ci] = 0;
	}//initial ri and car's total time in school
	for(chi=0;chi<k+1;chi++){//show each checkpoint
		for(ci=0;ci<num;ci++){//show each car
			if(ri[ci]==car[ci+1]) continue;//this means this car has been checked over, so check next
			while(record[ri[ci]].time<=checkpoint[chi]){//time<checkpoint needs to be checked
				if(ri[ci]!=car[ci+1]-1&&record[ri[ci]].status==1&&record[ri[ci]+1].status==0){//only 'in' and 'out' together with the order is correct
					cartime[ci] -= record[ri[ci]].time;//tatal time = out time - in time
					count++;//car is in the school, so tatal car's number add 1
				}
				else if(ri[ci]!=car[ci]&&record[ri[ci]].status==0&&record[ri[ci]-1].status==1){
					cartime[ci] += record[ri[ci]].time;
					count--;//car is out of the school, so tatal car's number subtract 1
				}
				ri[ci]++;//find the next status needed to be checked
				if(ri[ci]==car[ci+1]){
					break;//one car is checked over
				}
			}
		}
		if(chi!=k)               //if chi==k, this is the situation with the checkpoint "24:00:00" we set
			printf("%d\n",count);//so we get each car's total time in the school at final
	}
}

void findmost(PtrToInfo record, int* car)
{
	int i,j=0,max=0;
	char (*p)[8],temp[8];
	p = (char(*)[8])malloc(num*sizeof(char*));//dynamic memory allocation for p to save the car's name whose total time is max
	for(i=0;i<num;i++){
		if(cartime[i]>max){
			max = cartime[i];
			j = 1;//save the number of car's names whose total time is max
			strcpy(p[0],record[car[i]].name);
		}
		else if(cartime[i]==max&&max!=0){
			strcpy(p[j++],record[car[i]].name);//save car's name whose total time is max
		}
	}
	if(max!=0){//max==0 means no car has a correct record
		for(i=0;i<j;i++)
			printf("%s ",p[i]);//print cars' names
		printf("%02d:%02d:%02d",max/3600,max%3600/60,max%60);//print the tatal max time
	}
}
