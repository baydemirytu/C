#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct PERSON{
	
	char id[6];
	char name[30];
	char surname[30];
	char birthDate[5];
	char address[25];
	struct PERSON* next;
	
}PERSON;

typedef struct{
	
	char id[6];
	int index;
	int isDeleted;
	
}HASH_NODE;


PERSON* createPerson(char* , char*, char*, char* , char*);
void printPerson(PERSON* person);
HASH_NODE createNode(char*);//Tablodaki elemanlarý oluþturur.
int calculateKey(char*);
int hash(int , int, int);
int hash1(int, int);
int hash2(int, int);
int findHashIndex(HASH_NODE* hashTable,int key, int m, char* id);
void printCache(PERSON* );



int main(){
	
	int n;//Cachein eleman sayýsý
	int m;// hash table ýn uzunlugu
	char id[11];
	char name[30];
	char surname[30];
	char birthDate[8];
	char address[80];
	int i,j;
	int key;//id nin horner metoduyla bulunan keyi
	int hashIndex;//key in hash tabledaki yeri 
	int cacheCounter=0;//Cachedeki eleman sayýsýný tutar.
	
	PERSON* cacheHead;
	PERSON* tmp;
	PERSON* prev;
		
	printf("Cache kapasitesini (N) girin:");
	scanf("%d",&n);
	
	printf("Hash tablosunun uzunlugunu girin:");
	scanf("%d",&m);
	
	HASH_NODE* hashTable = (HASH_NODE*) malloc(sizeof(HASH_NODE)*m);
	
	for(i=0;i<m;i++){
		strcpy(hashTable[i].id,"-1");
		hashTable[i].isDeleted=0;
	}
	
	printf("%d uzunluklu Hash Tablosu olusturuldu.\n",m);
	
	FILE * f = fopen("test.txt","r");
	
	printf("\nSorgulama basliyor...\n");	
	printf("\n-------------------------------------------\n");
	
	while(!feof(f)){
		
		fscanf(f, "%s ", &id);
		key=calculateKey(id);
		hashIndex = findHashIndex(hashTable, key,m, id);
		printf("\n%s id'li kisi sorgulaniyor...\n",id);
		
		if(strcmp(hashTable[hashIndex].id,"-1")==0 || hashTable[hashIndex].isDeleted==1){
			//Bu if blogunda cache de bilgisi olmayan veya silinmiþ personu cache e ekliyoruz
			fscanf(f, "%s %s %s %s ", &name, &surname, &birthDate, &address);
			hashTable[hashIndex] = createNode(id);
			
			if(cacheCounter==0){
				//cachein ilk elemanýný ekleme
				cacheHead = createPerson(id,name,surname,birthDate,address);
				cacheCounter++;
				hashTable[hashIndex].index=0;	
				printf("\n%s id'li kisi hashTable[%d] ve cache'e eklendi.\n",id,hashIndex);
				printCache(cacheHead);
			}
			else if ( cacheCounter < n){
				// eleman silmeden en baþa person eklenmesi
				tmp = createPerson(id,name,surname,birthDate,address);
				tmp->next = cacheHead;
				cacheHead = tmp;
				
				cacheCounter++;
				
				tmp=cacheHead;
				j=0;
				while(tmp!=NULL){
					
					//Cacheddeki personlarýn hashte adresleri artýyor
					hashTable[findHashIndex(hashTable,calculateKey(tmp->id),m,tmp->id)].index=j++;
					tmp=tmp->next;
					
				}
				
				printf("\n%s id'li kisi hashTable[%d] ve cache'e eklendi.\n",id,hashIndex);
				printCache(cacheHead);
				
			}
			else{
				//sondaki elemaný silerek en baþa eleman ekleme,indexleri güncelleme
				tmp = createPerson(id,name,surname,birthDate,address);
				tmp->next = cacheHead;
				cacheHead = tmp;
				
				for(j=0;j<n;j++){
					prev = tmp;
					tmp=tmp->next;
				}
				
				prev->next = NULL;//Sondaki eleman ulaþýlmaz hale geliyor-siliniyor.
				hashTable[findHashIndex(hashTable,calculateKey(tmp->id),m,tmp->id)].isDeleted=1;
				
				printf("\n%s id'li kisi hashTable[%d] ve cache'e eklendi.",id,hashIndex);
				printf("\n%s id'li kisi cacheden silindi. ",tmp->id); 
				printf("hashTable[%d].isDeleted=1 yapildi.\n", findHashIndex(hashTable,calculateKey(tmp->id),m,tmp->id));
				printCache(cacheHead);
				
				free(tmp);
				//cachein son halindeki indexler hashe aktarýlýyor
				tmp=cacheHead;
				for(j=0;j<n;j++){
					hashTable[findHashIndex(hashTable,calculateKey(tmp->id),m,tmp->id)].index=j;
					tmp=tmp->next;
				}
				
				
			}
			
			
		}
		else if(strcmp(id,hashTable[hashIndex].id)==0){
			//Bu else if blogunda cache olan bir elemanýn sorgulanmasý iþi halledilir.
			//hashTable[hashIndex]->index indeki cache personunu bulup yazdýrma
			//bu personu cachein baþýna getirme
			//diðer personlarýn indexini hash table da güncelleme
			
			fscanf(f, "%s %s %s %s ", &name, &surname, &birthDate, &address);//dosyada sonraki satýra geçmek için okuma yapýlýr.
			
			if ( cacheCounter == 1 ){
				//Tek eleman olduðu için bir þey yapmaya gerek yoktur.
				printf("\n%s id'li kisi cacheteki tek eleman oldugu icin islem yapilmadi.\n",id);
			} else {
				
				printf("\n%s id'li kisi cachede bulundugu icin eleman ekleme cikarma yapilmadi.",id);
				printf("\n%s id'li kisi basa tasiniyor...\n",id);
				
				tmp=cacheHead;
				prev=tmp;
				while(strcmp(tmp->id,id)!=0){
					
					prev = tmp;
					tmp=tmp->next;
					
				}
				
				
				if(tmp->next==NULL){
					//Son eleman baþa gelir demek
					
					prev->next=NULL;
					tmp->next=cacheHead;
					cacheHead=tmp;
					
				}else{
					//Sonda olmayan bir eleman basa gelir
					prev->next=tmp->next;
					tmp->next=cacheHead;
					cacheHead=tmp;
					
				}
				
				j=0;
				tmp=cacheHead;
				while(tmp!=NULL){
					
					hashTable[findHashIndex(hashTable,calculateKey(tmp->id),m,tmp->id)].index=j++;
					tmp=tmp->next;
				}
				
				
			}
			
			printCache(cacheHead);
			
		}
			
		
	}
	
	
	printf("\n\n----Sorgulama dosyasi tamamen okunmustur.---");
	
	printCache(cacheHead);
	
	fclose(f);
	free(hashTable);
	free(cacheHead);
	
	printf("\n---Program sonlanmistir---");
	
    return 0;
}

void printCache(PERSON* cacheHead){
	
	printf("\nGuncel cache:");
	PERSON* tmp=cacheHead;
	
	while(tmp!=NULL){
		printPerson(tmp);
		tmp=tmp->next;
	}
	
	printf("\n-------------------------------------------\n");
	return;
}


int findHashIndex(HASH_NODE* hashTable, int key, int m, char* id){
	//id'si verilen kiþinin hashTable'da nereye yerleþmesi gerektiðini bulur
	int i=0;
	int hashIndex = hash(key,i,m);
	// o indiste bulunan id kendi idsine eþit deðilse, o indiste herhangi baþka bir id bulunuyorsa ve oradaki eleman silinmemiþse bir sonraki yere bakar.
	while( strcmp(id,hashTable[hashIndex].id)!=0 && strcmp("-1",hashTable[hashIndex].id)!=0  &&  hashTable[hashIndex].isDeleted==0){
		i++;
		hashIndex = hash(key,i,m);
	}

	return hashIndex;	
}


int hash(int key, int i, int m){
	
	return (hash1(key,m) + i*hash2(key,m)) % m;
	
}


int hash1(int key, int m){
	
	return key % m;
	
}


int hash2(int key, int m){
	
	return 1 + (key % (m-1));

}



int calculateKey(char* id){
	
	int idLength = 5;
	int i;
	int key=0;
	int prime=31;
	int c;
	
	for(i=idLength-1;i>=0;i--){
		c=id[i] - '0';
		key = (prime * key) + c;
		 
	}
	
	return key;
}


HASH_NODE createNode(char* id){
	
	HASH_NODE node ;
	strcpy(node.id,id);
	node.index=0;
	node.isDeleted=0;
	
	return node;
	
}



PERSON* createPerson(char* id, char* name,char* surname, char* birthDate, char* address){
	
	PERSON* person=(PERSON*)malloc(sizeof(PERSON));
	strcpy(person->id,id);
	strcpy(person->name,name);
	strcpy(person->surname,surname);
	strcpy(person->birthDate,birthDate);
	strcpy(person->address,address);
	person->next=NULL;
	
	return person;
}


void printPerson(PERSON* person){
	printf("\n%s - ",person->id);
	printf("%s %s - ",person->name,person->surname);
	printf("%s - ",person->birthDate);
	printf("%s",person->address);
	return;
}




