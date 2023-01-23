#include <stdio.h>

int main(void){
	int n;
	char p[100];
	scanf("%d",&n);
	for(int i =0; i < n;i++){
		scanf("%s",p);
		printf("%s", p);
		printf("\n");
	}
	return 0;
}
