#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Heap {
	int lungime; //lungimea efectiva cu tot cu elemnetele ascunse
	Masina* vector;
	int nrElemVizibile;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap h;
	h.vector = (Masina*)malloc(sizeof(Masina)*lungime);
	h.lungime = lungime;
	h.nrElemVizibile = 0;
	return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuStanga = 2 * pozitieNod + 1;
	int pozFiuDreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	if (heap.vector[pozMax].pret < heap.vector[pozFiuStanga].pret && pozFiuStanga < heap.nrElemVizibile) {

		pozMax = pozFiuStanga;
	}
	if (heap.vector[pozMax].pret < heap.vector[pozFiuDreapta].pret&& pozFiuDreapta<heap.nrElemVizibile) {
		pozMax = pozFiuDreapta;
	}

	if (pozMax != pozitieNod) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMax < (heap.nrElemVizibile - 2) / 2)
			filtreazaHeap(heap, pozMax);
	}

}
 
Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap h = initializareHeap(10);
	while (!feof(f)) {
		h.vector[h.nrElemVizibile] = citireMasinaDinFisier(f);
		h.nrElemVizibile++;
	}
	fclose(f);
	
	for (int i = (h.nrElemVizibile - 2) / 2; i >= 0;i--) {
		filtreazaHeap(h, i);
	}
	return h;
}

void afisareHeap(Heap heap) {
	for (int i = 0;i < heap.nrElemVizibile;i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemVizibile;i < heap.lungime;i++)
		afisareMasina(heap.vector[i]);
}

Masina extrageMasina(Heap* heap) {
	Masina m;
	m.id = -1;

	if (heap->nrElemVizibile != 0) {
		m = (*heap).vector[0];
		heap->vector[0] = heap->vector[heap->nrElemVizibile - 1];
		heap->vector[heap->nrElemVizibile - 1] = m;
		heap->nrElemVizibile--;
		for (int i = (heap->nrElemVizibile - 2) / 2; i >= 0;i--) {
			filtreazaHeap(*heap, i);
		}
	}
	return m;
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0;i < heap->lungime;i++) {
		if (heap->vector[i].model)
			free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->lungime = 0;
	heap->nrElemVizibile = 0;
	heap->vector = NULL;
}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(h);
	printf("Extrageri:\n");
	afisareMasina(extrageMasina(&h));
	afisareMasina(extrageMasina(&h));
	afisareMasina(extrageMasina(&h));
	printf("Heap ascuns\n");
	afiseazaHeapAscuns(h);

	dezalocareHeap(&h);
	printf("Dupa dezalocare\n");
	afisareHeap(h);

	return 0;
}
