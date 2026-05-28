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
typedef struct Nod Nod;
typedef struct NodSecundar NodSecundar;
struct Nod
{
	Masina info;
	Nod* next;
	NodSecundar* vecini;
};

struct NodSecundar
{
	Nod* inform;
	NodSecundar* next;
};

//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareListaPrincipala(Nod** graf, Masina m)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->vecini = NULL;
	nou->info = m;

	if ((*graf) == NULL)
	{
		(*graf) = nou;
	}
	else
	{
		Nod* p = (*graf);
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = nou;
	}
}

void inserrareListaSecundara(NodSecundar** cap, Nod* vecin)
{
	NodSecundar* nou = malloc(sizeof(NodSecundar));
	nou->next = NULL;
	nou->inform = vecin;

	if ((*cap) == NULL)
	{
		(*cap) = nou;
	}
	else
	{
		NodSecundar* p = (*cap);
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
}
//3.
//functie de cautarea in lista principala dupa ID
Nod* cautaNodDupaID(Nod* graf, int id)
{


	while (graf && graf->info.id != id)
	{
		graf = graf->next;
	}
	return graf;

}

//4.
//inserare muchie
void inserareMuchie(Nod* graf, int idStart, int idStop)
{
	Nod* nodStart = cautaNodDupaID(graf, idStart);
	Nod* nodStop = cautaNodDupaID(graf, idStop);
	if (nodStart && nodStop)
	{
		inserrareListaSecundara(&nodStart->vecini, nodStop);
		inserrareListaSecundara(&nodStop->vecini, nodStart);
	}
}

void* citireNoduriMasiniDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* graf = NULL;

	while (!(feof(f)))
	{
		inserareListaPrincipala(&graf, citireMasinaDinFisier(f));
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(Nod* graf, const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	int start, stop;
	while (!feof(f))
	{
		fscanf(f, "%d %d", &start, &stop);
		inserareMuchie(graf, start, stop);
	}
	fclose(f);
	return graf;
}
void dezalocareListaSecundara(NodSecundar** cap)
{
	while (*cap)
	{
		NodSecundar* p = (*cap);
		(*cap) = (*cap)->next;
		free(p);
	}
}
void dezalocareListaPrincipala(Nod** graf)
{
	while (*graf)
	{
		Nod* p = (*graf);
		(*graf) = (*graf)->next;
		dezalocareListaSecundara(&(p->vecini));
		if (p->info.model != NULL)
		{
			free(p->info.model);
		}
		if (p->info.numeSofer != NULL)
		{
			free(p->info.numeSofer);
		}
		free(p);
	}
}

void afiseazaListaSec(Nod* graf, int id)
{
	Nod* nou = cautaNodDupaID(graf, id);
	NodSecundar* aux = nou->vecini;

	while (aux)
	{
		afisareMasina(aux->inform->info);
		aux = aux->next;
	}
}



//Parcurgere in adancime

//creare structura Stiva
//push si pop pentru stiva
typedef struct NodStiva NodStiva;

struct NodStiva
{
	int info;
	NodStiva* next;
};

void push(NodStiva** cap, int masina)
{
	NodStiva* nou = malloc(sizeof(NodStiva));
	nou->info = masina;
	nou->next = *cap;
	*cap = nou;
}

int pop(NodStiva** cap)
{
	if (*cap)
	{
		int rezultat = (*cap)->info;
		NodStiva* aux = (*cap);
		(*cap) = (*cap)->next;
		free(aux);
		return rezultat;
	}
	else {
		return -1;
	}
}

int calculeazaNrNoduriGraf(Nod* graf)
{
	int nrNoduri = 0;
	while (graf)
	{
		nrNoduri++;
		graf = graf->next;
	}
	return nrNoduri;
}

void afisareGrafInAdancime(Nod* graf, int idPlecare)
{
	/*
	1. Este luata o STIVA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in STIVA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in STIVA
	4.1 Extragem un nod din STIVA
	4.2 Procesam nodul extras din STIVA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitat, adugandu-i in STIVA
	4.4 reluam procesul
	*/

	NodStiva* stiva = NULL;
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	int* vizitate = malloc(sizeof(int) * nrNoduri);
	for (int i = 0; i < nrNoduri; i++)
	{
		vizitate[i] = 0;
	}
	vizitate[idPlecare - 1] = 1;
	push(&stiva, idPlecare);
	while (stiva)
	{
		int idCurent = pop(&stiva);
		Nod* nodCurent = cautaNodDupaID(graf, idCurent);
		afisareMasina(nodCurent->info);
		NodSecundar* p = nodCurent->vecini;
		while (p)
		{
			if (vizitate[p->inform->info.id - 1] == 0)
			{
				vizitate[p->inform->info.id - 1] = 1;
				push(&stiva, p->inform->info.id);
			}
			p = p->next;
		}
	}

}


//Parcurgere in latime

//creare structura coada
//enqueue si dequeue pentru coada

typedef struct NodCoada NodCoada;
struct NodCoada
{
	int info;
	NodCoada* next;
};

void enQueue(NodCoada** coada, int id)
{
	NodCoada* nou = malloc(sizeof(NodCoada));
	nou->info = id;
	nou->next = NULL;
	if (*coada)
	{
		NodCoada* p = *coada;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
	else
	{
		*coada = nou;
	}
}
int dequeue(NodCoada** coada)
{
	if (*coada)
	{
		int rezultat = (*coada)->info;
		NodCoada* aux = (*coada);
		(*coada) = (*coada)->next;
		free(aux);
		return rezultat;
	}
	else return -1;
}


void afisareGrafInLatime(void* graf, int idPlecare) {
	/*
	1. Este luata o COADA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in COADA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in COADA
	4.1 Extragem un nod din coada
	4.2 Procesam nodul extras din COADA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitati, adugandu-i in COADA
	4.4 reluam procesul
	*/
	NodCoada* coada = NULL;
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	int* vizitate = malloc(sizeof(int) * nrNoduri);
	for (int i = 0; i < nrNoduri; i++)
	{
		vizitate[i] = 0;
	}
	vizitate[idPlecare - 1] = 1;
	enQueue(&coada, idPlecare);
	while (coada)
	{
		int idCurent = dequeue(&coada);
		Nod* nodCurent = cautaNodDupaID(graf, idCurent);
		afisareMasina(nodCurent->info);
		NodSecundar* p = nodCurent->vecini;
		while (p)
		{
			if (vizitate[p->inform->info.id - 1] == 0)
			{
				vizitate[p->inform->info.id - 1] = 1;
				enQueue(&coada, p->inform->info.id);
			}
			p = p->next;
		}
	}

}

int main() {
	Nod* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");
	//afiseazaListaSec(graf, 4);
	afisareGrafInLatime(graf, 1);


	dezalocareListaPrincipala(&graf);
	return 0;
}
