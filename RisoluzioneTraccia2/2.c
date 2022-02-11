#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INPUT_FILE_NAME "filiali.txt"
#define OUTPUT_FILE_NAME "output.txt"

struct filiale{
	char nome[32];
	int num_filiali;
	char** i_esimaFiliale;
	struct filiale* next;
	struct compressedLocationStruct* compressedLocations;
};

struct compressedLocationStruct{
	char* location;
	int numberOfAgenciesInLocation;
	struct compressedLocationStruct* next;
};

struct filiale* leggiFile(FILE*);						// read from file												
void stampaLista(struct filiale*);						// print on screen what has been read
struct filiale* eliminaAziende(struct filiale*);		// delete agencies whose have less than two locations
void comprimi(struct filiale*);							// store location data in a compressed way
int giaControllata(struct filiale*, char*, int);		// check if I am reading the same location again. for internal use only
void stampaCompressed(struct filiale*);					// print on screen the compressed way
int contaOccorrenze(struct filiale*, char*, int);		// count how many locations are in the same city for a given agency
void stampaSuFile(struct filiale*, FILE*); 				// print on file

int main(void){
	struct filiale* head = NULL;
	FILE* fp = fopen(INPUT_FILE_NAME,"r+");
	FILE* fp_out = fopen(OUTPUT_FILE_NAME,"w+");
	
	if(fp == NULL){
		fprintf(stderr,"%s","Errore apertura file 'filiali.txt'\n");
		return 1;
	}
	
	if(fp_out == NULL){
		fprintf(stderr,"%s","Errore creazione file 'output.txt', verifica i permessi nella directory e lo spazio nel disco\n");
		return 1;
	}
	
	head = leggiFile(fp);
	
	printf(" === ELEMENTI LETTI ===\n");
	stampaLista(head);
	
	head = eliminaAziende(head);
	
	printf("\n === ELEMENTI RISULTANTI DALLA RIMOZIONE ===\n");
	stampaLista(head);
	
	comprimi(head);
	printf("\n === COMPRESSED FORMAT ===\n");
	stampaCompressed(head);
	
	stampaSuFile(head, fp_out);
	
	
	fclose(fp);
	fclose(fp_out);
	return 0;
}

/*
** Given an agency ("filiale") stored in "head", and given the city name
** and the position in the array of that city, check if I checked already
** the existance of that said city.
** Returns 1 if I checked it already, 0 otherwise. 
*/
int giaControllata(struct filiale* head, char* city, int i){
	int j;
	for(j=0 ; j<i ; ++j){
		if(strcmp(head->i_esimaFiliale[j],city) == 0) return 1;
	}
	return 0;
}

void stampaLista(struct filiale* head){
	while(head!=NULL){
		printf("%s %d",head->nome, head->num_filiali);
		int i;
		for(i=0 ; i<head->num_filiali ; ++i){
			printf(" %s", head->i_esimaFiliale[i]);
		}
		printf("\n");
		head=head->next;
	}
}

struct filiale* leggiFile(FILE* fp){
	int num_filiali,i;
	struct filiale* head = NULL;
	struct filiale* tmp = NULL;
	char nomeAzienda[32];
	char nomeCity[32];
	
	while( fscanf(fp,"%s %d",nomeAzienda,&num_filiali)>0 ){
		
		tmp = malloc(sizeof(struct filiale));
		strcpy(tmp->nome,nomeAzienda);
		tmp->num_filiali=num_filiali;
		tmp->i_esimaFiliale = malloc(num_filiali*sizeof(char*));
		
		for(i=0;i<num_filiali;i++){
			fscanf(fp,"%s",nomeCity);
			tmp->i_esimaFiliale[i]=(char*)malloc(32*sizeof(char));
			strcpy(tmp->i_esimaFiliale[i],nomeCity);	
		}
		
		tmp->next=head;
		head=tmp;
	}
	return head;
}

struct filiale* eliminaAziende(struct filiale* head){
	
	if(head==NULL) return NULL;
	struct filiale* headFissa=head;
	struct filiale* prec = NULL;
	struct filiale* tmp;

	while(head!=NULL){
		if(head->num_filiali<2){
			if(prec==NULL){ // if the node to delete is the head
				tmp=head;
				headFissa = head->next;
				head=head->next;
				free(tmp);
			}else{
				tmp=head;
				prec->next=head->next;
				head=head->next;
				free(tmp);
			}
		}else{
			prec=head;
			head=head->next;
		}
	}
	return headFissa;
}

/*
** This function will delete the array containing city names and will
** store the same information in a linked list which each node contains
** the city name and how many agencies ("filiali") there are in that city
** for a given company name.
*/
void comprimi(struct filiale* head){
	int numeroFilialiInCittaTmp, i;
    struct compressedLocationStruct* clsHead;
	struct compressedLocationStruct* clsTmp;
	
	while(head != NULL){
		clsHead = NULL;
		clsTmp = NULL;
		
		for(i=0 ; i < head->num_filiali ; ++i){
			if(giaControllata(head, head->i_esimaFiliale[i], i) == 0){	
				
				numeroFilialiInCittaTmp = contaOccorrenze(head, head->i_esimaFiliale[i], head->num_filiali);
				
				clsTmp = malloc(sizeof(struct compressedLocationStruct));
				
				clsTmp->location = (char*)malloc(32*sizeof(char));
				strcpy(clsTmp->location, head->i_esimaFiliale[i]);
				clsTmp->numberOfAgenciesInLocation = numeroFilialiInCittaTmp;
				
				clsTmp->next = clsHead;
				clsHead = clsTmp;
			}
		}
		
		// free memory from the array containing city names
		for(i=0 ; i < head->num_filiali ; ++i){
			free(head->i_esimaFiliale[i]);
		}
	    free(head->i_esimaFiliale);
				
		head->compressedLocations = clsHead;
		head = head->next;
	}
}

int contaOccorrenze(struct filiale* head, char* cityName, int numberOfCities){
	int i, occorrenze=0;
	for(i=0 ; i<numberOfCities ; ++i){
		if( strcmp(head->i_esimaFiliale[i],cityName)==0){
			++occorrenze;
		}
	}
	return occorrenze;
}

void stampaCompressed(struct filiale* head){
	struct compressedLocationStruct* tmpCompressed = NULL;
	while(head!=NULL){
		printf("%s %d", head->nome , head->num_filiali);
		tmpCompressed = head->compressedLocations;
		while(tmpCompressed != NULL){
			printf(" %s %d",  tmpCompressed->location, tmpCompressed->numberOfAgenciesInLocation);
			tmpCompressed = tmpCompressed->next;
		}
		printf("\n\r");
		head = head->next;
	}
}

void stampaSuFile(struct filiale* head, FILE* fp){
	struct compressedLocationStruct* tmpCompressed = NULL;
	while(head != NULL){
		fprintf(fp, "%s %d", head->nome , head->num_filiali);
		tmpCompressed = head->compressedLocations;
		while(tmpCompressed != NULL){
			fprintf(fp, " %s %d",  tmpCompressed->location, tmpCompressed->numberOfAgenciesInLocation);
			tmpCompressed = tmpCompressed->next;
		}
		fprintf(fp, "\n\r");
		head = head->next;
	}
}
