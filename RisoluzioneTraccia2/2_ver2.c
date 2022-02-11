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
};

struct filiale* leggiFile(FILE*);						// read from file												
void stampaLista(struct filiale*);						// print on screen what has been read
struct filiale* eliminaAziende(struct filiale*);		// delete agencies whose have less than two locations
void comprimi(struct filiale*);							// store location data in a compressed way
int contaOccorrenze(struct filiale* , char*, int, int);	// count how many locations are in the same city for a given agency
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
	stampaLista(head);
	stampaSuFile(head, fp_out);
	
	fclose(fp);
	fclose(fp_out);
	return 0;
}

void stampaLista(struct filiale* head){
	while(head!=NULL){
		printf("%s %d",head->nome, head->num_filiali);
		int i;
		for(i=0 ; i<head->num_filiali ; ++i){
			if(head->i_esimaFiliale[i] != NULL)
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

void comprimi(struct filiale* head){
	int numeroFilialiInCittaTmp, i;
	char numeroFilialiInCittaBuffer[8]; 
	
	while(head != NULL){
		for(i=0 ; i < head->num_filiali ; ++i){
			if(head->i_esimaFiliale[i] != NULL){
				numeroFilialiInCittaTmp = contaOccorrenze(head, head->i_esimaFiliale[i], head->num_filiali, i);
				itoa(numeroFilialiInCittaTmp, numeroFilialiInCittaBuffer, 10);
				strcat(head->i_esimaFiliale[i], " ");
				strcat(head->i_esimaFiliale[i], numeroFilialiInCittaBuffer);
			}
		}
		head = head->next;
	}
}

int contaOccorrenze(struct filiale* head, char* cityName, int numberOfCities, int startPosition){
	int i; 
	int occorrenze = 1; // There's already the occurrance in position "startPosition" which does not have to be free-d
	for(i=startPosition+1 ; i<numberOfCities ; ++i){
		if(head->i_esimaFiliale[i]!=NULL && strcmp(head->i_esimaFiliale[i],cityName)==0){
			++occorrenze;
			free(head->i_esimaFiliale[i]);
			head->i_esimaFiliale[i] = NULL;
		}
	}
	return occorrenze;
}

void stampaSuFile(struct filiale* head, FILE* fp){
	while(head!=NULL){
		fprintf(fp, "%s %d",head->nome, head->num_filiali);
		int i;
		for(i=0 ; i<head->num_filiali ; ++i){
			if(head->i_esimaFiliale[i] != NULL)
				fprintf(fp, " %s", head->i_esimaFiliale[i]);
		}
		fprintf(fp, "\n");
		head=head->next;
	}
}
