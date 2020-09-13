/* traccia 4 */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#define maxLength 32

struct studente{
	char nome[maxLength];
	char cognome[maxLength];
	int matricola;
	char esame[maxLength];
	int voto;
	struct studente* next;
};

struct studente* caricaLista(FILE* fp);
void stampaLista(struct studente* head);
struct studente* eliminaRic(struct studente* head);
void scriviFile(struct studente* head);
int checkSeGiaVisitato(struct studente* lista,int matricola);

int main(void){
	FILE* fp = fopen("input.txt","r");
	struct studente* head = NULL;
	if(fp==NULL){
		perror("Errore, impossibile aprire il file\n");
		return 1;
	}
	
	head = caricaLista(fp);
	stampaLista(head);
	head=eliminaRic(head);
	printf("\n\n---\n\n");
	stampaLista(head);
	scriviFile(head);
	fclose(fp);
	return 0;
}

int checkSeGiaVisitato(struct studente* lista,int matricola){
	if(lista==NULL) return 0;
	if(lista->matricola == matricola) return 1;
	return checkSeGiaVisitato(lista->next,matricola);
}

void scriviFile(struct studente* head){
	FILE* fp_out = fopen("output.txt","w+");
	if(fp_out==NULL){
		perror("Impossibile scrivere su file in output: ");
		return;
	}
	if(head==NULL){
		fprintf(fp_out,"%s","No data inserted.");
	}
	
	struct studente* ListaVisitati = NULL;
	struct studente* ListaVisitatiTMP = NULL;
	struct studente* headApp = NULL;
	
	while(head!=NULL){
		if(checkSeGiaVisitato(ListaVisitati,head->matricola)==1){
			head=head->next;
			continue;
		}
		ListaVisitatiTMP = malloc(sizeof(struct studente));
		ListaVisitatiTMP->matricola=head->matricola;
		ListaVisitatiTMP->next=ListaVisitati;
		ListaVisitati=ListaVisitatiTMP;
		fprintf(fp_out,"%s %s %d %s %d\n",head->nome,head->cognome,head->matricola,head->esame,head->voto);
		
		for(headApp=head->next;headApp!=NULL;headApp=headApp->next){
			if(headApp->matricola==head->matricola){
				fprintf(fp_out,"%s %s %d %s %d\n",headApp->nome,headApp->cognome,headApp->matricola,headApp->esame,headApp->voto);
			}
		}
		head=head->next;
	}
	fclose(fp_out);
}

struct studente* eliminaRic(struct studente* head){
	if(head==NULL) return NULL;
	struct studente* succ = eliminaRic(head->next);
	
	if(head->voto<18){
		free(head);
		return succ;
	}else{
		head->next=succ;
	}
	return head;	
}

void stampaLista(struct studente* head){
	while(head!=NULL){
		printf("%d\n",head->matricola);
		head=head->next;
	}
	return;
}

struct studente* caricaLista(FILE* fp){
	if(fp==NULL) exit(1);
	
	struct studente* head = NULL;
	struct studente* tmp = NULL;
	
	char nome_buf[maxLength];
	char cognome_buf[maxLength];
	int matricola_buf;
	char esame_buf[maxLength];
	int voto_buf;
	
	while( fscanf(fp,"%s %s %d %s %d",nome_buf,cognome_buf,&matricola_buf,esame_buf,&voto_buf)>0 ){
		tmp = malloc(sizeof(struct studente));
		tmp->matricola=matricola_buf;
		tmp->voto=voto_buf;
		strcpy(tmp->nome,nome_buf);
		strcpy(tmp->cognome,cognome_buf);
		strcpy(tmp->esame,esame_buf);
		tmp->next=head;
		head = tmp;
	}
	return head;
}
