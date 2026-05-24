#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Soldat Soldat;
typedef struct Heap Heap;

struct Soldat {
	int id;
	char* nume;
	char* rang;
	int aniExperienta;
	float salariu;
	float punctajMisiune;
};

struct Heap {
	int lungime;
	Soldat* vector;
	int nrSoldati;
};

char* copiazaString(const char* text) {
	if (text == NULL) {
		return NULL;
	}
	char* copie = (char*)malloc(strlen(text) + 1);
	strcpy(copie, text);
	return copie;
}

Soldat initializareSoldatInvalid() {
	Soldat s;
	s.id = -1;
	s.nume = NULL;
	s.rang = NULL;
	s.aniExperienta = 0;
	s.salariu = 0;
	s.punctajMisiune = 0;
	return s;
}

void dezalocareSoldat(Soldat* s) {
	if (s->nume != NULL) {
		free(s->nume);
	}
	if (s->rang != NULL) {
		free(s->rang);
	}
	s->id = -1;
	s->nume = NULL;
	s->rang = NULL;
	s->aniExperienta = 0;
	s->salariu = 0;
	s->punctajMisiune = 0;
}

Soldat copiazaSoldat(Soldat s) {
	Soldat copie;
	copie.id = s.id;
	copie.nume = copiazaString(s.nume);
	copie.rang = copiazaString(s.rang);
	copie.aniExperienta = s.aniExperienta;
	copie.salariu = s.salariu;
	copie.punctajMisiune = s.punctajMisiune;
	return copie;
}

Soldat citireSoldatDinFisier(FILE* file) {
	char buffer[200];
	char sep[3] = ",\n";
	Soldat s = initializareSoldatInvalid();

	if (fgets(buffer, 200, file) == NULL) {
		return s;
	}

	char* aux = strtok(buffer, sep);
	if (aux == NULL) {
		return s;
	}
	s.id = atoi(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return s;
	}
	s.nume = copiazaString(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return s;
	}
	s.rang = copiazaString(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return s;
	}
	s.aniExperienta = atoi(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return s;
	}
	s.salariu = atof(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return s;
	}
	s.punctajMisiune = atof(aux);

	return s;
}

void afisareSoldat(Soldat s) {
	if (s.id != -1) {
		printf("id: %d\n", s.id);
		printf("nume: %s\n", s.nume);
		printf("rang: %s\n", s.rang);
		printf("ani experienta: %d\n", s.aniExperienta);
		printf("salariu: %.2f\n", s.salariu);
		printf("punctaj misiune: %.2f\n\n", s.punctajMisiune);
	}
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrSoldati; i++) {
		afisareSoldat(heap.vector[i]);
	}
}

void afisareVectorSoldati(Soldat* vector, int nrSoldati) {
	for (int i = 0; i < nrSoldati; i++) {
		afisareSoldat(vector[i]);
	}
}

int prioritateRang(const char* rang) {
	if (strcmp(rang, "soldat") == 0) {
		return 1;
	}
	if (strcmp(rang, "caporal") == 0) {
		return 2;
	}
	if (strcmp(rang, "sergent") == 0) {
		return 3;
	}
	if (strcmp(rang, "locotenent") == 0) {
		return 4;
	}
	if (strcmp(rang, "maior") == 0) {
		return 5;
	}
	return 0;
}

int arePrioritate(Soldat s1, Soldat s2) {
	if (s1.punctajMisiune > s2.punctajMisiune) {
		return 1;
	}
	if (s1.punctajMisiune < s2.punctajMisiune) {
		return 0;
	}
	if (prioritateRang(s1.rang) > prioritateRang(s2.rang)) {
		return 1;
	}
	if (prioritateRang(s1.rang) < prioritateRang(s2.rang)) {
		return 0;
	}
	if (s1.aniExperienta > s2.aniExperienta) {
		return 1;
	}
	return 0;
}

void interschimbaSoldati(Soldat* s1, Soldat* s2) {
	Soldat aux = *s1;
	*s1 = *s2;
	*s2 = aux;
}

void filtrareHeap(Heap* heap, int pozitie) {
	int pozitieMax = pozitie;
	int pozitieStanga = 2 * pozitie + 1;
	int pozitieDreapta = 2 * pozitie + 2;

	if (pozitieStanga < heap->nrSoldati &&
		arePrioritate(heap->vector[pozitieStanga], heap->vector[pozitieMax])) {
		pozitieMax = pozitieStanga;
	}

	if (pozitieDreapta < heap->nrSoldati &&
		arePrioritate(heap->vector[pozitieDreapta], heap->vector[pozitieMax])) {
		pozitieMax = pozitieDreapta;
	}

	if (pozitieMax != pozitie) {
		interschimbaSoldati(&heap->vector[pozitie], &heap->vector[pozitieMax]);
		filtrareHeap(heap, pozitieMax);
	}
}

void construiesteHeap(Heap* heap) {
	for (int i = heap->nrSoldati / 2 - 1; i >= 0; i--) {
		filtrareHeap(heap, i);
	}
}

void adaugaSoldatInVectorHeap(Heap* heap, Soldat s) {
	if (heap->nrSoldati == heap->lungime) {
		int lungimeNoua;

		if (heap->lungime == 0) {
			lungimeNoua = 5;
		}
		else {
			lungimeNoua = heap->lungime * 2;
		}

		Soldat* vectorNou = (Soldat*)malloc(sizeof(Soldat) * lungimeNoua);

		for (int i = 0; i < heap->nrSoldati; i++) {
			vectorNou[i] = heap->vector[i];
		}

		free(heap->vector);
		heap->vector = vectorNou;
		heap->lungime = lungimeNoua;
	}

	heap->vector[heap->nrSoldati] = s;
	heap->nrSoldati++;
}

Heap citireHeapSoldatiDinFisier(const char* numeFisier) {
	Heap heap;
	heap.lungime = 0;
	heap.vector = NULL;
	heap.nrSoldati = 0;

	FILE* file = fopen(numeFisier, "r");

	if (file != NULL) {
		Soldat s;

		while (1) {
			s = citireSoldatDinFisier(file);

			if (s.id != -1) {
				adaugaSoldatInVectorHeap(&heap, s);
			}
			else {
				break;
			}
		}

		fclose(file);
		construiesteHeap(&heap);
	}

	return heap;
}

Soldat extrageSoldatCuPrioritateMaxima(Heap* heap) {
	Soldat rezultat = initializareSoldatInvalid();

	if (heap->nrSoldati > 0) {
		rezultat = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrSoldati - 1];
		heap->nrSoldati--;
		filtrareHeap(heap, 0);
	}

	return rezultat;
}

float calculeazaSumaSalariiDupaRang(Heap heap, const char* rangCautat, int aniMinimi) {
	float suma = 0;

	for (int i = 0; i < heap.nrSoldati; i++) {
		if (strcmp(heap.vector[i].rang, rangCautat) == 0 &&
			heap.vector[i].aniExperienta >= aniMinimi) {
			suma += heap.vector[i].salariu;
		}
	}

	return suma;
}

void copiazaSoldatiDupaRangSiPunctaj(
	Heap heap,
	const char* rangCautat,
	float punctajMinim,
	Soldat** vectorNou,
	int* nrSoldatiCopiati
) {
	*nrSoldatiCopiati = 0;

	for (int i = 0; i < heap.nrSoldati; i++) {
		if (strcmp(heap.vector[i].rang, rangCautat) == 0 &&
			heap.vector[i].punctajMisiune >= punctajMinim) {
			(*nrSoldatiCopiati)++;
		}
	}

	if (*nrSoldatiCopiati > 0) {
		*vectorNou = (Soldat*)malloc(sizeof(Soldat) * (*nrSoldatiCopiati));

		int k = 0;

		for (int i = 0; i < heap.nrSoldati; i++) {
			if (strcmp(heap.vector[i].rang, rangCautat) == 0 &&
				heap.vector[i].punctajMisiune >= punctajMinim) {
				(*vectorNou)[k] = copiazaSoldat(heap.vector[i]);
				k++;
			}
		}
	}
	else {
		*vectorNou = NULL;
	}
}

Heap copiazaSoldatiDinRangInHeap(Heap heap, const char* rangCautat) {
	Heap heapNou;
	heapNou.lungime = 0;
	heapNou.vector = NULL;
	heapNou.nrSoldati = 0;

	for (int i = 0; i < heap.nrSoldati; i++) {
		if (strcmp(heap.vector[i].rang, rangCautat) == 0) {
			adaugaSoldatInVectorHeap(&heapNou, copiazaSoldat(heap.vector[i]));
		}
	}

	construiesteHeap(&heapNou);

	return heapNou;
}

void selecteazaTopKSoldatiDupaRang(Heap heap, const char* rangCautat, int k) {
	Heap heapNou = copiazaSoldatiDinRangInHeap(heap, rangCautat);

	if (heapNou.nrSoldati == 0) {
		printf("nu exista soldati cu rangul cautat\n");
	}
	else {
		for (int i = 0; i < k && heapNou.nrSoldati > 0; i++) {
			Soldat extras = extrageSoldatCuPrioritateMaxima(&heapNou);
			afisareSoldat(extras);
			dezalocareSoldat(&extras);
		}
	}

	for (int i = 0; i < heapNou.nrSoldati; i++) {
		dezalocareSoldat(&heapNou.vector[i]);
	}

	free(heapNou.vector);
	heapNou.vector = NULL;
	heapNou.lungime = 0;
	heapNou.nrSoldati = 0;
}

void dezalocareVectorSoldati(Soldat** vector, int* nrSoldati) {
	for (int i = 0; i < *nrSoldati; i++) {
		dezalocareSoldat(&(*vector)[i]);
	}

	free(*vector);
	*vector = NULL;
	*nrSoldati = 0;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrSoldati; i++) {
		dezalocareSoldat(&heap->vector[i]);
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrSoldati = 0;
}

int main() {
	Heap heap = citireHeapSoldatiDinFisier("soldati.txt");

	printf("heap-ul initial:\n\n");
	afisareHeap(heap);

	printf("soldatul extras cu prioritate maxima:\n\n");
	Soldat extras = extrageSoldatCuPrioritateMaxima(&heap);
	afisareSoldat(extras);
	dezalocareSoldat(&extras);

	printf("heap-ul dupa extragere:\n\n");
	afisareHeap(heap);

	float suma = calculeazaSumaSalariiDupaRang(heap, "sergent", 5);
	printf("suma salariilor pentru sergenti cu minim 5 ani experienta: %.2f\n\n", suma);

	Soldat* vectorNou = NULL;
	int nrSoldatiCopiati = 0;

	copiazaSoldatiDupaRangSiPunctaj(heap, "locotenent", 80, &vectorNou, &nrSoldatiCopiati);

	printf("soldati copiati dupa rang si punctaj:\n\n");
	afisareVectorSoldati(vectorNou, nrSoldatiCopiati);

	dezalocareVectorSoldati(&vectorNou, &nrSoldatiCopiati);

	printf("top 2 sergenti:\n\n");
	selecteazaTopKSoldatiDupaRang(heap, "sergent", 2);

	dezalocareHeap(&heap);

	return 0;
}
