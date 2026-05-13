//la test heap, abc, avl, grafuri fara parcurgeri
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

typedef struct NodP NodP;
typedef struct NodS NodS;	

struct NodP {
	Masina info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};

//2.
//functii de inserare in liste
//si in principala si in secundara



void inserareInListaP(NodP** graf, Masina masinaNoua) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vecini = NULL;
	if ((*graf)) {
		NodP* aux = (*graf);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else (*graf) = nou;
}

void inserareInListaS(NodS** listaS, NodP* vecin) {
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;
	if ((*listaS)) {
		NodS* aux = (*listaS);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else (*listaS) = nou;
}

//3.
//functie de cautarea in lista principala dupa ID
NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	NodP* aux = listaPrincipala;
	while (aux) {
		if (aux->info.id == id) {
			return aux;
		}
		aux = aux->next;
	}
	return NULL;
}

//4.
//inserare muchie
void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaID(listaPrincipala, idStop);
	if (start && stop) {
		inserareInListaS(&(start->vecini), stop);
		inserareInListaS(&(stop->vecini), start);
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	NodP* graf = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		inserareInListaP(&graf, m);
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		int idStart = 0;
		int idStop = 0;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(graf, idStart, idStop);
	}
	fclose(f);
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
}

void afiseazaListaDeVecini(int id, NodP* graf) {
	NodP* temp = cautaNodDupaID(graf, id);
	NodS* lista = temp->vecini;
	while (lista) {
		afisareMasina(lista->info->info);
		lista = lista->next;
	}
}

int main() {
	NodP* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);
	afiseazaListaDeVecini(8, graf);

	return 0;
}
