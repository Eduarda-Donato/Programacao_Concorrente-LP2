#include <stdio.h>
#include <stdlib.h>

int main(void){
	char str[31]; /*1 do \n*/
	fgets(str, 30, stdin);
	printf("PROG2-PRE: %s", str);
	return 0;
}
