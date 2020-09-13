#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct city{
	char luogo[32];
	float x;
	float y;
	struct city* next;
};

struct city* leggiFile(FILE* fp);
void stampaLista(struct city* head);
struct city* eliminaDuplicati(struct city* head);
float** creaMatrice(int lunghezza);
void caricaMatrice(float** matrice, int lunghezza, struct city* head);
struct city getCitta(int offset,struct city* head);
void stampaMatrice(float** m, int lunghezza);
void scriviFile(struct city* head,float** matrice, int lunghezza);

int main(void){
	FILE* fp = fopen("cities.txt","r");
	struct city* head = NULL;
	int lunghezzaLista;
	float** matrice;
	if(fp==NULL){
		printf("An error has occurred while reading 'cities.txt'\n");
		return 1;
	}
	head = leggiFile(fp);
	head = eliminaDuplicati(head);
	printf("ELEMENTI LETTI DAL FILE:\n");
	stampaLista(head);
	lunghezzaLista = listLength(head);
	matrice = creaMatrice(lunghezzaLista);
	caricaMatrice(matrice,lunghezzaLista,head);
	printf("\n\nCITTA' E RELATIVE DISTANZE\n");
	stampaMatrice(matrice,lunghezzaLista);
	scriviFile(head,matrice,lunghezzaLista);
	system("pause");
	return 0;
}

struct city* leggiFile(FILE* fp){
	char buf[32];
	float x,y;
	struct city* head = NULL;
	struct city* tmp = NULL;
	while( fscanf(fp,"%s %f %f",buf,&x,&y)>0 ){
		tmp = malloc(sizeof(struct city));
		tmp->x=x;
		tmp->y=y;
		strcpy(tmp->luogo,buf);
		tmp->next=head;
		head=tmp;
	}
	return head;
}

void stampaLista(struct city* head){
	while(head!=NULL){
		printf("\t%s %.2f %.2f\n",head->luogo,head->x,head->y);
		head=head->next;
	}
}

struct city* eliminaDuplicati(struct city* head){
	
	struct city* headFissa = head;
	struct city* prec = NULL;
	struct city* puntatoreMobile = NULL;
	struct city* daCancellare = NULL;
	
	while(head!=NULL){
		puntatoreMobile = head->next;
		prec = head;
		while(puntatoreMobile!=NULL){
			if( !strcmp(head->luogo,puntatoreMobile->luogo) ){
				daCancellare = puntatoreMobile;
				puntatoreMobile=puntatoreMobile->next;
				prec->next=puntatoreMobile;
				free(daCancellare);
			}else{
				puntatoreMobile = puntatoreMobile->next;
			}
			prec=prec->next;
		}
		head=head->next;
	}
	return headFissa;
}

int listLength(struct city* head){
	int length = 0;
	while(head!=NULL){
		length++;
		head=head->next;
	}
	return length;
}

float** creaMatrice(int lunghezza){
	float** matrice = malloc(lunghezza*sizeof(float*));
	int i;
	for(i=0;i<lunghezza;i++){
		matrice[i] = malloc(lunghezza*sizeof(float));
	}
	return matrice;
}

void caricaMatrice(float** matrice, int lunghezza, struct city* head){
	int i,j;
	struct city citta1,citta2;
	for(i=0;i<lunghezza;i++){
		for(j=0;j<lunghezza;j++){
			citta1 = getCitta(i,head);
			citta2 = getCitta(j,head);
			matrice[i][j] = sqrt( pow(citta1.x-citta2.x,2) + pow(citta1.y-citta2.y,2) );
		}
	}
}

struct city getCitta(int offset,struct city* head){
	struct city Citta;
	int i;
	for(i=0;i<offset;i++){
		head=head->next;
	}
	strcpy(Citta.luogo,head->luogo);
	Citta.x=head->x;
	Citta.y=head->y;
	return Citta;
}

void stampaMatrice(float** m, int lunghezza){
	int i,j;
	for(i=0;i<lunghezza;i++){
		for(j=0;j<lunghezza;j++){
			printf("\t%.2f ",m[i][j]);
		}
		printf("\n");
	}
}

void scriviFile(struct city* head,float** matrice, int lunghezza){
	FILE* fp_out = fopen("output.txt","w+");
	int i=0,j=0,posizioneCittaVicina;
	float distanzaMinima;
	struct city* headFissa = head;
	struct city cittaPiuVicina;
	struct city cittaCorrente;
	if(fp_out==NULL){
		printf("An error has occurred while creating the output file\n");
	}
	

	if(lunghezza==1){
		fprintf(fp_out,"%s","Vi e' presente una sola citta'. Non e' possibile trovare una citta' vicina ad essa");
		printf("\nVi e' presente una sola citta'. Non e' possibile trovare una citta' vicina ad essa\n");
		return;
	}
	while(i<lunghezza){
		
		if(i==0){
			distanzaMinima = matrice[i+1][0];
			posizioneCittaVicina=i+1;
		}else{
			distanzaMinima = matrice[i][0];
		}
		
		for(j=0;j<lunghezza;j++){
			if( matrice[i][j]<distanzaMinima && matrice[i][j]!=0){
				distanzaMinima=matrice[i][j];
				posizioneCittaVicina=j;
			}
		}
		j--;
		cittaPiuVicina=getCitta(posizioneCittaVicina,headFissa);
		cittaCorrente=getCitta(i,headFissa);
		fprintf(fp_out,"La citta piu' vicina a %s e' %s con una distanza minima di %f\n",cittaCorrente.luogo,cittaPiuVicina.luogo,distanzaMinima);
		printf("\nLa citta piu' vicina a %s e' %s con una distanza minima di %f",cittaCorrente.luogo,cittaPiuVicina.luogo,distanzaMinima);
		i++;
	}
}
