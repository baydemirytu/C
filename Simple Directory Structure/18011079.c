#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100


typedef struct NODE{
	char name[40];
	struct NODE* first_child;
	struct NODE* next_sibling;
}NODE;



typedef struct STACK{ 
	int top;     //chdir_dotdot() ve chdir() fonksiyonunun çalýþmasý için stack veri yapýsý kullandým
	struct NODE* stack[MAX];
}STACK;



void init_stack(STACK* dir_stack){
	dir_stack->top=-1;
}


int isEmpty(STACK *dir_stack){
	
	if(dir_stack->top==-1){		
		return 1;
	}
	else{
		return 0;
	}
		
	
}


void push(STACK *dir_stack, NODE* current_dir){
	
	dir_stack->stack[++dir_stack->top]=current_dir;					
}


NODE* pop(STACK *dir_stack){	
	if(!isEmpty(dir_stack))							
		return dir_stack->stack[dir_stack->top--];	
}


NODE* create_node(char name[30]){
	
	NODE* root = (NODE*)malloc(sizeof(NODE));

	int i=0;
	
	while( (i<40) && name[i]!='\0' ){
		root->name[i]=name[i];
		i++;
	}
	root->name[i]='\0';
	
	root->first_child=NULL;
	root->next_sibling=NULL;
	
	return root;
}


NODE* mkdir(NODE* current_dir, char name[30]){
	
	NODE* tmp_node;

	
	
	if(current_dir->first_child == NULL){
		current_dir->first_child = create_node(name);
	}
	else{
		tmp_node=current_dir->first_child;
		
		while(tmp_node->next_sibling!=NULL && (strcmp(tmp_node->name,name)!=0)){
			tmp_node=tmp_node->next_sibling;
		}
		
		if(strcmp(tmp_node->name,name)==0){
			printf("\nThe '%s'directory is already exists!",name);
		}
		else{
			tmp_node->next_sibling=create_node(name);
		}

	}
		
	return current_dir;
	
}


NODE* chdir(NODE* current_dir, char name[30], STACK* dir_stack){
	NODE* tmp_node;
	
	
	if(current_dir->first_child == NULL){
		printf("\n '%s' directory'sinin altinda directory yoktur!",current_dir->name);
	}
	else{
		tmp_node=current_dir->first_child;
		while(tmp_node->next_sibling!= NULL && strcmp(tmp_node->name,name)!=0){
			tmp_node=tmp_node->next_sibling;
		}
		if(strcmp(tmp_node->name,name)==0){
			printf("'%s' isimli directory bulundu!",name);
			push(dir_stack,current_dir);
			current_dir=tmp_node;
		}
		else{
			printf("\n'%s' isimli directory bulunamadi!",name);
		}
	}

	return current_dir;
	
}

NODE* chdir_dotdot(STACK* dir_stack, NODE* current_dir){
	
	if(strcmp(current_dir->name,"root")==0){
		printf("Bir ust directory bulunmamaktadir!");
		return current_dir;
	}
	else{
		return pop(dir_stack);
	}

}


NODE* rmdir(NODE* current_dir, char name[30]){
	NODE* tmp_node;
	NODE* prev_tmp;
	
	
	
	if(current_dir->first_child==NULL){
		printf("\nAlt directory bulunmamaktadir!");
	}
	else{
		prev_tmp=current_dir;
		tmp_node=current_dir->first_child;
		
		
		while(tmp_node->next_sibling!=NULL && strcmp(tmp_node->name,name)!=0){
			prev_tmp=tmp_node;
			tmp_node=tmp_node->next_sibling;
		}
		if(strcmp(tmp_node->name,name)==0){
			
			if(tmp_node->first_child!=NULL){
				printf("\nAlt directory'si oldugu icin silinemedi!");
			}
			else{
				if(tmp_node->next_sibling!=NULL){
					prev_tmp->next_sibling=tmp_node->next_sibling;					
					printf("Silindi!");
				}
				else{
					prev_tmp->next_sibling=NULL;
					if(current_dir->first_child==tmp_node){
						current_dir->first_child=NULL;
					}
					printf("Silindi!");
				}
			}
		}
		else{
			printf("'%s' isimli directory bulunamadi!",name);

		}
	}
	return current_dir;
}

void dir(NODE* current_dir){
	
	NODE* tmp_node;
		
	if(current_dir->first_child!=NULL){
				
		tmp_node = current_dir->first_child;
		printf(">%s",tmp_node->name);
				
			if(tmp_node->next_sibling!=NULL){
				
				while(tmp_node->next_sibling!=NULL){
						
					tmp_node = tmp_node->next_sibling;
					printf("\n %s",tmp_node->name);
				}
			}		
	}

}


void count(NODE* current_dir){

	NODE* tmp_node;
	int counter=0;
		
	if(current_dir->first_child!=NULL){
		counter++;		
		tmp_node = current_dir->first_child;
					
			if(tmp_node->next_sibling!=NULL){
				
				while(tmp_node->next_sibling!=NULL){
					counter++;	
					tmp_node = tmp_node->next_sibling;

				}
			}		
	}
	printf(">%d",counter);

}



int main(){
	
	NODE* root = create_node("root");
	NODE* current_dir = root;
	STACK* dir_stack = (STACK*)malloc(sizeof(STACK));
	init_stack(dir_stack);
	
	system("title root/");
			
	char tmp[10];
	char second[30];	
	char input[30];
	
	char exit[4]="exit";
	char komut1[6]="mkdir";
	char komut2[6]="chdir";
	char komut3[6]="rmdir";
	char komut4[4]="dir";
	char komut5[6]="count";
	
	printf("Programdan cikmak istediginizde 'exit' yaziniz(tirnaksiz).");
	
	while(strcmp(input,exit)!=0){			
		printf("\n>");
		gets(input);
				
		sscanf(input,"%s %s",tmp,second); // input alma ve parselama
				
		if(strcmp(tmp,komut1)==0){
			
			mkdir(current_dir,second);//mkdir komutu
				
		}
		
		else if(strcmp(tmp,komut2)==0){
			
			if(strcmp(second,"..")==0){
				current_dir=chdir_dotdot(dir_stack,current_dir); // chdir .. komutu
			}
			else{
				current_dir = chdir(current_dir,second,dir_stack); // chdir komutu
			}				
		}
		
		else if(strcmp(tmp,komut3)==0){
			current_dir=rmdir(current_dir,second);		//rmdir komutu
		}
	
		else if(strcmp(tmp,komut4)==0){ // dir komutu
			dir(current_dir);		
		}
		
		else if(strcmp(tmp,komut5)==0){ // count komutu
			count(current_dir);		
		}
								
	}
	
	free(root);
	free(dir_stack); 
	
    return 0;
}

