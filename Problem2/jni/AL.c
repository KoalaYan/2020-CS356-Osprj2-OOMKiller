#include<stdio.h>
#include<stdlib.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<string.h>


int main(){
	int i;
	printf("start add.\n");
	for(i=0;i < 20;i++){
		printf("uid = %d",i);
		syscall(59, i, 1000);
	}
	for(i=0;i < 10;i++){
		printf("uid = %d",i);
		syscall(59, i, 2000);
	}
	printf("end add.\n");
	return 0;
}
