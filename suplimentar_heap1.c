#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Laptop Laptop;

struct Laptop {
	int id;
	int ram;
	float pret;
	char* producator;
	char* model;
	unsigned char serie;
};

typedef struct Heap Heap;
struct Heap {
	int lungime;
	Laptop* vector;
	int nrLaptopuri;
};

Laptop citireLaptopDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	Laptop l;
	if (fgets(buffer, 100, f)==NULL) {
		l.id = -1;
		l.ram = 0;
		l.pret = 0;
		l.producator = NULL;
		l.model = NULL;
		l.serie = '-';
		return l;

	}


	char* aux = strtok(buffer, sep);
	l.id = atoi(aux);

	l.ram = atoi(strtok(NULL, sep));
	l.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	l.producator = malloc(strlen(aux) + 1);
	strcpy(l.producator, aux);

	aux = strtok(NULL, sep);
	l.model = malloc(strlen(aux) + 1);
	strcpy(l.model, aux);

	l.serie = *strtok(NULL, sep);

	return l;
}

void afisareLaptop(Laptop laptop) {
	if (laptop.id != -1) {
		printf("Id: %d\n", laptop.id);
		printf("RAM: %d\n", laptop.ram);
		printf("Pret: %.2f\n", laptop.pret);
		printf("Producator: %s\n", laptop.producator);
		printf("Model: %s\n", laptop.model);
		printf("Serie: %c\n\n", laptop.serie);
	}
}


Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrLaptopuri = 0;
	heap.vector = (Laptop*)malloc(sizeof(Laptop) * lungime);
	for (int i = 0; i < lungime; i++) {
		heap.vector[i].id = -1;
		heap.vector[i].ram = 0;
		heap.vector[i].pret = 0;
		heap.vector[i].producator = NULL;
		heap.vector[i].model = NULL;
		heap.vector[i].serie = '-';
	}
	return heap;
}

void filtreazaheap(Heap heap, int pozitieNod) {
	int pozfiuSt = pozitieNod * 2 + 1;
	int pozfiuDr = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;
	if (pozfiuSt < heap.nrLaptopuri && heap.vector[pozMax].pret < heap.vector[pozfiuSt].pret) {
		pozMax = pozfiuSt;
	}
	if (pozfiuDr < heap.nrLaptopuri && heap.vector[pozMax].pret < heap.vector[pozfiuDr].pret) {
		pozMax = pozfiuDr;
	}

	if (pozMax != pozitieNod) { //pahare
		Laptop aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		filtreazaheap(heap, pozMax);
		
	}
}
Heap citireHeapDeLaptopuriDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	if (f) {
		while (!feof(f)) {
			Laptop laptop = citireLaptopDinFisier(f);
			if (laptop.id != -1) {
				heap.vector[heap.nrLaptopuri++] = laptop;
			}
		}
		fclose(f);
	}
	for (int i = (heap.nrLaptopuri - 2) / 2; i >= 0; i--) {
		filtreazaheap(heap, i);
	}
	return heap;
}
void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrLaptopuri; i++) {
		afisareLaptop(heap.vector[i]);
	}
}
int main() {
	Heap heap = citireHeapDeLaptopuriDinFisier("laptopuri.txt");
	printf("heap initial:\n");
	afisareHeap(heap);
	return 0;
}
