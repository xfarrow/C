#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#define MAX_LENGTH 32

struct persona{
	char nome[MAX_LENGTH];
	char cognome[MAX_LENGTH];
	int altezza;
	int peso;
	int age;
	struct persona* next;
};

struct persona* leggiFile(FILE* fp);
void stampaLista(struct persona* head);
struct persona* elimina_less_than(int key, struct persona* head);
struct persona* duplica(struct persona* head);
struct persona* elimina_less_than_ric(struct persona* prec,struct persona* head,int key);
int contaElementi(struct persona* head);
struct persona* trovaPari(struct persona* head);
struct persona* elimina(int x, struct persona* head);
struct persona* eliminaXesimo(int x, struct persona* head);
struct persona* eliminaIterativo(int x, struct persona* head);

int main(void){
	struct persona* head;
	FILE* fp = fopen("persone.txt","r");
	if(fp==NULL){
        printf("%s",strerror(errno));
		return 1;
	}
	head=leggiFile(fp);
	head = duplica(head);
	stampaLista(head);
    printf("Elementi all' interno della lista: %d\n",contaElementi(head));
  	head = eliminaXesimo(2,head);
  	printf("Elementi all' interno della lista: %d\n",contaElementi(head));
	stampaLista(head);
    return 0;
}

//elimina x-esimo elemento dalla lista
struct persona* eliminaXesimo(int x, struct persona* head){
	if(x==1) return head->next;
	struct persona* suc = eliminaXesimo(x-1,head->next);
	head->next=suc;
	return head;
}

struct persona* eliminaIterativo(int x, struct persona* head){
	int i;
	struct persona* tmp;
	for(i=1;i<=x;i++){
		tmp=head;
		head=head->next;
		free(tmp);
	}
	return head;
}

//ricevuta una Lista e un intero X, elimini i primi X elementi
struct persona* elimina(int x, struct persona* head){
	struct persona* suc = NULL;
	if(x==1){
		suc=head->next;
		free(head);
		return suc;
	}
	suc = elimina(x-1,head->next);
	free(head);
	return suc;
}


//puntatore al minimo elemento pari nella lista
struct persona* minPari(struct persona* head){
	struct persona* minimoPrecedente;
    if(head==NULL) return NULL;
    minimoPrecedente = minPari(head->next);
    if(head->age%2==0){
    	if(minimoPrecedente==NULL) return head;
    	else if(head->age < minimoPrecedente->age) return head;
	}
	return minimoPrecedente;
}

//trova il primo elemento pari
struct persona* trovaPari(struct persona* head){
    if(head->age%2==0 || head==NULL) return head;
    return trovaPari(head->next);
}

//conta gli elementi nella lista
int contaElementi(struct persona* head){
    if(head==NULL) return 0;
    return 1+contaElementi(head->next);
}

struct persona* elimina_less_than_ric(struct persona* prec,struct persona* head,int key){
    struct persona* da_eliminare=NULL;
    
    if(head==NULL) return NULL;
    
    struct persona* suc = elimina_less_than_ric(head,head->next,key);
    
    if(head->age<key){
        da_eliminare=head;
        if(prec!=NULL)
            prec->next=suc;
        else{
            free(da_eliminare);
            return suc;
        }
        head=suc;
        free(da_eliminare);
    }
    return head;
}

struct persona* duplica(struct persona* head){
	if(head==NULL) return NULL;
    
    struct persona* suc = duplica(head->next);
    
    if(head->age%2==0){
        struct persona* duplicato = malloc(sizeof(struct persona));
        duplicato->next=suc;
        head->next=duplicato;
        duplicato->age=head->age;
        duplicato->altezza=head->altezza;
        duplicato->peso=head->peso;
        strcpy(duplicato->nome,head->nome);
        strcpy(duplicato->cognome,head->cognome);
    }
    return head;
    
}

struct persona* elimina_less_than(int key, struct persona* head){
	struct persona* prec = NULL;
	struct persona* headFissa = head;
	struct persona* da_eliminare = NULL;
	while(head!=NULL){
		if(head->peso<key){
			//Se bisogna cancellare la testa:
			if(prec==NULL){
				da_eliminare=head;
				headFissa = head->next;
				head=head->next;
				free(da_eliminare);
			}else{
				da_eliminare=head;
				prec->next=head->next;
				head=head->next;
				free(da_eliminare);
			}
		}else{
			prec=head;
			head=head->next;
		}
	}
	return headFissa;
}

void stampaLista(struct persona* head){
	printf("	---- LISTA ----\n");
	while(head!=NULL){
		printf("Nome: %s\n",head->nome);
		printf("Cognome: %s\n",head->cognome);
		printf("Peso: %d\n",head->peso);
		printf("Altezza: %d\n",head->altezza);
		printf("Eta': %d\n",head->age);
		printf("\n\n");
		head=head->next;
	}
}

struct persona* leggiFile(FILE* fp){
	char nome[MAX_LENGTH];
	char cognome[MAX_LENGTH];
	int altezza;
	int peso;
	int age;
	int x;
	
	struct persona* head = NULL;
	struct persona* tmp = NULL;
	
	while((x=fscanf(fp,"%s %s %d %d %d",nome,cognome,&peso,&altezza,&age))>0){
		tmp = malloc(sizeof(struct persona));
		tmp->peso=peso;
		tmp->altezza=altezza;
		tmp->age=age;
		tmp->next=head;
		
		strcpy(tmp->nome,nome);
		strcpy(tmp->cognome,cognome);
		head=tmp;
	}
	return head;
}

