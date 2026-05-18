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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime; //nr de elemente din vector
	Masina* vector;
	int nrMasini; //nr masini cate masini am fara a fi ascunse

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
	//initializeaza heap-ul cu 0 elemente 
	//dar cu o lungime primita ca parametru
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;


}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozfiuSt = pozitieNod * 2 + 1;
	int pozfiuDr = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;

	if (pozfiuSt < heap.nrMasini && heap.vector[pozMax].pret < heap.vector[pozfiuSt].pret) {
		pozMax = pozfiuSt;
	}

	if (pozfiuDr < heap.nrMasini && heap.vector[pozMax].pret < heap.vector[pozfiuDr].pret) {
		pozMax = pozfiuDr;
	}

	if (pozMax != pozitieNod) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		filtreazaHeap(heap, pozMax);
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrMasini++] = citireMasinaDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrMasini - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}
void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrMasini; i++) {
		afisareMasina(heap.vector[i]);

	}
	
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	if (heap->nrMasini > 0) {
		Masina aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrMasini - 1];
		heap->vector[heap->nrMasini - 1] = aux;
		for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--)
		{
			filtreazaHeap(*heap, i);
		}
		return aux; //shallow copy
	}



}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);

	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMasini = 0;
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("masini extrase: \n");
	afisareMasina(extrageMasina(&heap));
	dezalocareHeap(&heap);

	return 0;
}
