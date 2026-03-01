#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Biblioteca {
	int id;
	int nrCarti;
	char* nume;
	float suprafata;
	char sector; // aici retinem cifra sectorului
};

// Functie de initializare
struct Biblioteca initializare(int id, int nrCarti, char* nume, float suprafata, char sector) {
	struct Biblioteca s;
	s.id = id;  // initializam id-ul
	s.nrCarti = nrCarti;
	s.nume = (char*)malloc(strlen(nume) + 1); // alocare memorie pentru nume
	strcpy(s.nume, nume); // copiere deep copy
	s.suprafata = suprafata;
	s.sector = sector; // sector ca cifra ('0'..'9')
	return s;
}

// Functie de copiere struct (deep copy)
struct Biblioteca copiazaBiblioteca(struct Biblioteca sursa) {
	return initializare(sursa.id, sursa.nrCarti, sursa.nume, sursa.suprafata, sursa.sector);
}

void afisare(struct Biblioteca s) {
	printf("id: %d, Nr de carti: %d, nume: %s, suprafata: %.2f, sectorul: %c\n",
		s.id, s.nrCarti, s.nume, s.suprafata, s.sector);
}

void afisareVector(struct Biblioteca* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Biblioteca* copiazaPrimeleNElemente(struct Biblioteca* vector, int nrElemente, int nrElementeCopiate) {

	if (nrElementeCopiate > nrElemente) {
		nrElementeCopiate = nrElemente;
	}

	struct Biblioteca* vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrElementeCopiate);

	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = copiazaBiblioteca(vector[i]);
	}

	return vectorNou;
}

// Dezalocare vector si elemente
void dezalocare(struct Biblioteca** vector, int* nrElemente) {

	for (int i = 0; i < (*nrElemente); i++) {
		free((*vector)[i].nume);
	}

	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaBibliotecileCuSuprafataMare(
	struct Biblioteca* vector,
	int nrElemente,
	float suprafataMinima,
	struct Biblioteca** vectorNou,
	int* dimensiune)
{
	*dimensiune = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata > suprafataMinima) {
			(*dimensiune)++;
		}
	}

	*vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * (*dimensiune));

	int k = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata > suprafataMinima) {
			(*vectorNou)[k] = copiazaBiblioteca(vector[i]);
			k++;
		}
	}
}

int main() {
	struct Biblioteca b;
	b = initializare(1, 3, "nationala", 500, '3'); // sector = cifra 3
	afisare(b);

	int nrBiblioteci = 6;
	// int → 4 bytes
	// char* → 8 bytes(pointer pe 64-bit)
	// float → 4 bytes
	// char → 1 byte
	// ocupa 21 si avand 5 biblioteci trb 105 bytes

	struct Biblioteca* vectorB = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrBiblioteci);

	vectorB[0] = b; // shallow copy
	vectorB[1] = initializare(6, 121, "ASE", 200, '1');
	vectorB[2] = initializare(8, 131, "DA", 100, '2');
	vectorB[3] = initializare(9, 1221, "EU", 300, '3');
	vectorB[4] = initializare(6, 171, "ASIA", 120, '4');
	vectorB[5] = initializare(3, 181, "USA", 320, '5');

	printf("elementele din vector:\n");
	afisareVector(vectorB, nrBiblioteci);

	int nrCopiate = 3;
	struct Biblioteca* copie = copiazaPrimeleNElemente(vectorB, nrBiblioteci, nrCopiate);

	printf("\nElementele copiate:\n");
	afisareVector(copie, nrCopiate);

	dezalocare(&copie, &nrCopiate);

	struct Biblioteca* biblioteciSuprafataMare = NULL;
	int numarBiblioteciSuprafataMare = 0;

	copiazaBibliotecileCuSuprafataMare(
		vectorB,
		nrBiblioteci,
		100,
		&biblioteciSuprafataMare,
		&numarBiblioteciSuprafataMare);

	printf("\nBiblioteci cu suprafata > 100:\n");
	afisareVector(biblioteciSuprafataMare, numarBiblioteciSuprafataMare);

	dezalocare(&biblioteciSuprafataMare, &numarBiblioteciSuprafataMare);
	dezalocare(&vectorB, &nrBiblioteci);

	return 0;
}
