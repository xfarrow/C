#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct filiale{
	char nome[32];
	int num_filiali;
	char** i_esimaFiliale;
	struct filiale* next;
};


struct filiale* leggiFile(FILE*);
struct filiale* eliminaAziende(struct filiale* head);
void stampaLista(struct filiale* head);

int main(void){
	struct filiale* head = NULL;
	FILE* fp = fopen("filiali.txt","r+");
	FILE* fp_out = fopen("output.txt","w+");
	
	if(fp==NULL){
		fprintf(stderr,"%s","Errore apertura file 'filiali.txt'\n");
		return 1;
	}
	
	if(fp_out==NULL){
		fprintf(stderr,"%s","Errore creaxione file 'output.txt', verifica i permessi nella directory e lo spazio nel disco\n");
		return 1;
	}
	
	head = leggiFile(fp);
	head=eliminaAziende(head);
	stampaLista(head);
	stampaSuFile(head,fp_out);
	fclose(fp);
	fclose(fp_out);
	return 0;
}

int giaControllata(struct filiale* head, char* city, int i, int dim){
	int j;
	for(j=0;j<dim;j++){
		if(  strcmp(head->i_esimaFiliale[j],city) && j<i) return 1;
	}
	return 0;
}
// non funziona
void stampaSuFile(struct filiale* head, FILE* fp){
	if(head==NULL){
		fprintf(fp,"%s","No input file found. Aborted.\n");
		return;
	}
	int num_filiali,i,j,filiali_per_city;
	char cityAttuale[32];
	
	while(head!=NULL){
		fprintf(fp,"%s ",head->nome);
		num_filiali=head->num_filiali;
		for(i=0;i<num_filiali;i++){
			strcpy(cityAttuale,head->i_esimaFiliale[i]);
			if(giaControllata(head,cityAttuale,i,num_filiali)) continue;	
			filiali_per_city=1;
			for(j=i+1;j<num_filiali;j++){
				if(  strcmp(cityAttuale,head->i_esimaFiliale[j])==0 ){
					++filiali_per_city;
				}
			}
			fprintf(fp,"%d %s",filiali_per_city,cityAttuale);	
		}
		fprintf(fp,"%c",'\n');
		head=head->next;
	}
}

void stampaLista(struct filiale* head){
	while(head!=NULL){
		printf("%d",head->num_filiali);
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
			//se il nodo da eliminare è la testa
			if(prec==NULL){
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
