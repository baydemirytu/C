#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100


typedef struct{
	int top;
	int size;
	char codes[MAX];
}STACK;

void initialize(STACK *s);
int isEmpty(STACK *);
int isFull(STACK *);
int pop(STACK *, char*);
int push(STACK *, char );
int decode_the_string(char[] ,STACK *s);


int main(){
	char kod1[MAX],kod2[MAX];
	STACK *s = (STACK*)malloc(sizeof(STACK));
	
	printf("Enter the Kod1:");
	scanf("%s",&kod1);
	printf("Kod1:%s\n\n",kod1);
	
	printf("Enter the Kod2:");
	scanf("%s",&kod2);
	printf("Kod2:%s",kod2);
	
	decode_the_string(kod1,s);
	printf("\nDecoded Kod1:%s",kod1);


	
	decode_the_string(kod2,s);
	printf("\nDecoded Kod2:%s",kod2);



	if(strcmp(kod1,kod2)==0){
		printf("\nBu iki casus iletisime gecebilir!");
	}
	else{
		printf("\nBu iki casus iletisime gecemez!");
	}
	
	free(s);
    return 0;
}


void initialize(STACK *s){
	s->top = -1;
	s->size = 0;
}

int isEmpty(STACK *s){
	
	if(s->top==-1){
		
		printf("Stack is empty!\n");
		return 1;
	}
	else{
		return 0;
	}
}

int isFull(STACK *s){
	
	if(s->top==MAX-1){
		
		printf("Stackoverflow!\n");
		return 1;
	}
	else{
		return 0;
	}
}

int pop(STACK *s, char* x){
	
	if(!isEmpty(s)){					
		
		*x = s->codes[s->top--];
		return 1;		
	}
	else{
		return 0;
	}
}

int push(STACK *s, char member){
	
	if(isFull(s)){
		return 0;
	}
	else{
		
		s->codes[++s->top]=member;	
		s->size++;			
		return 1;
	}
}

int decode_the_string(char kod_to_decrypt[],STACK *s){
	char tmp = kod_to_decrypt[0], junk;
	int i=0,counter,value;
	
	initialize(s);

	
	while(kod_to_decrypt[i]!='\0'){
		value=atoi(&tmp);
		if(kod_to_decrypt[i]<='9' && kod_to_decrypt[i]>='0'){
									
			if( s->size >= value){
				counter=atoi(&tmp);
				
				while(counter!=0){
					pop(s,&junk);
					counter--;
				}
			} else{
				printf("\nHatali mesaj!Error!");
				return 0;
			}
			
		} else {
			push(s,kod_to_decrypt[i]);
		}
		i++;
		tmp=kod_to_decrypt[i];		
	}
	
	counter=s->top;
	
	for(i = (s->top); i > -1; i--){		
		pop(s, &kod_to_decrypt[i]);		
	}
	
	kod_to_decrypt[counter+1]='\0';
	return 1;
}






