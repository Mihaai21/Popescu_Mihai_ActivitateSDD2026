#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Produs Produs;

struct Produs {
	int id;
	float pret;
	char* denumire;
};

typedef struct Heap Heap;

struct Heap {
	int lungime;
	Produs* vector;
	int nrProduse;
};

Produs citireDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Produs p;

	if (fgets(buffer, 100, file) == NULL) {
		p.id = -1;
		p.pret = 0;
		p.denumire = NULL;
		return p;
	}

	char* aux = strtok(buffer, sep);
	p.id = atoi(aux);

	p.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	p.denumire = (char*)malloc(strlen(aux) + 1);
	strcpy(p.denumire, aux);

	return p;
}

void afisare(Produs produs) {
	if (produs.id != -1) {
		printf("Id: %d\n", produs.id);
		printf("Pret: %.2f\n", produs.pret);
		printf("Denumire: %s\n\n", produs.denumire);
	}
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrProduse = 0;
	heap.vector = (Produs*)malloc(sizeof(Produs) * lungime);

	for (int i = 0; i < lungime; i++) {
		heap.vector[i].id = -1;
		heap.vector[i].pret = 0;
		heap.vector[i].denumire = NULL;
	}

	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozfiuSt = pozitieNod * 2 + 1;
	int pozfiuDr = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;

	if (pozfiuSt < heap.nrProduse && heap.vector[pozMax].pret < heap.vector[pozfiuSt].pret) {
		pozMax = pozfiuSt;
	}

	if (pozfiuDr < heap.nrProduse && heap.vector[pozMax].pret < heap.vector[pozfiuDr].pret) {
		pozMax = pozfiuDr;
	}

	if (pozMax != pozitieNod) {
		Produs aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		filtreazaHeap(heap, pozMax);
	}
}

Heap citireHeapDeDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	if (file) {
		while (!feof(file)) {
			Produs produs = citireDinFisier(file);
			if (produs.id != -1) {
				heap.vector[heap.nrProduse++] = produs;
			}
		}
		fclose(file);
	}

	for (int i = (heap.nrProduse - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrProduse; i++) {
		afisare(heap.vector[i]);
	}
}

Produs extrage(Heap* heap) {
	if (heap->nrProduse > 0) {
		Produs aux = heap->vector[0];

		heap->vector[0] = heap->vector[heap->nrProduse - 1];
		heap->vector[heap->nrProduse - 1] = aux;

		heap->nrProduse--;

		for (int i = (heap->nrProduse - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}

		return aux;
	}

	Produs p;
	p.id = -1;
	p.pret = 0;
	p.denumire = NULL;
	return p;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		if (heap->vector[i].denumire != NULL) {
			free(heap->vector[i].denumire);
		}
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrProduse = 0;
}

int main() {
	Heap heap = citireHeapDeDinFisier("produse.txt");

	printf("heap initial:\n");
	afisareHeap(heap);

	printf("produs extras:\n");
	Produs produsExtras = extrage(&heap);
	afisare(produsExtras);

	printf("heap dupa extragere:\n");
	afisareHeap(heap);

	dezalocareHeap(&heap);

	return 0;
}
