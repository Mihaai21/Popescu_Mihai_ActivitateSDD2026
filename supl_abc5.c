#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FierDeCalcat FierDeCalcat;
typedef struct Nod Nod;

struct FierDeCalcat {
	int id;
	char* marca;
	char* model;
	char* tipTalpa;
	int putereW;
	float capacitateRezervor;
	float pret;
	int areAbur;
};

struct Nod {
	FierDeCalcat info;
	Nod* stanga;
	Nod* dreapta;
};

char* copiazaString(const char* text) {
	if (text == NULL) {
		return NULL;
	}
	char* copie = (char*)malloc(strlen(text) + 1);
	strcpy(copie, text);
	return copie;
}

FierDeCalcat initializareFierInvalid() {
	FierDeCalcat f;
	f.id = -1;
	f.marca = NULL;
	f.model = NULL;
	f.tipTalpa = NULL;
	f.putereW = 0;
	f.capacitateRezervor = 0;
	f.pret = 0;
	f.areAbur = 0;
	return f;
}

void dezalocareFier(FierDeCalcat* f) {
	if (f->marca != NULL) {
		free(f->marca);
	}
	if (f->model != NULL) {
		free(f->model);
	}
	if (f->tipTalpa != NULL) {
		free(f->tipTalpa);
	}

	f->id = -1;
	f->marca = NULL;
	f->model = NULL;
	f->tipTalpa = NULL;
	f->putereW = 0;
	f->capacitateRezervor = 0;
	f->pret = 0;
	f->areAbur = 0;
}

FierDeCalcat copiazaFier(FierDeCalcat f) {
	FierDeCalcat copie;
	copie.id = f.id;
	copie.marca = copiazaString(f.marca);
	copie.model = copiazaString(f.model);
	copie.tipTalpa = copiazaString(f.tipTalpa);
	copie.putereW = f.putereW;
	copie.capacitateRezervor = f.capacitateRezervor;
	copie.pret = f.pret;
	copie.areAbur = f.areAbur;
	return copie;
}

FierDeCalcat citireFierDinFisier(FILE* file) {
	char buffer[200];
	char sep[3] = ",\n";
	FierDeCalcat f = initializareFierInvalid();

	if (fgets(buffer, 200, file) == NULL) {
		return f;
	}

	char* aux = strtok(buffer, sep);
	if (aux == NULL) {
		return f;
	}
	f.id = atoi(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareFier(&f);
		return initializareFierInvalid();
	}
	f.marca = copiazaString(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareFier(&f);
		return initializareFierInvalid();
	}
	f.model = copiazaString(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareFier(&f);
		return initializareFierInvalid();
	}
	f.tipTalpa = copiazaString(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareFier(&f);
		return initializareFierInvalid();
	}
	f.putereW = atoi(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareFier(&f);
		return initializareFierInvalid();
	}
	f.capacitateRezervor = atof(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareFier(&f);
		return initializareFierInvalid();
	}
	f.pret = atof(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareFier(&f);
		return initializareFierInvalid();
	}
	f.areAbur = atoi(aux);

	return f;
}

void afisareFier(FierDeCalcat f) {
	if (f.id != -1) {
		printf("id: %d\n", f.id);
		printf("marca: %s\n", f.marca);
		printf("model: %s\n", f.model);
		printf("tip talpa: %s\n", f.tipTalpa);
		printf("putere: %d W\n", f.putereW);
		printf("capacitate rezervor: %.2f L\n", f.capacitateRezervor);
		printf("pret: %.2f\n", f.pret);
		printf("are abur: %s\n\n", f.areAbur ? "da" : "nu");
	}
}

Nod* creareNod(FierDeCalcat fier) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = fier;
	nou->stanga = NULL;
	nou->dreapta = NULL;
	return nou;
}

Nod* adaugaFierInArbore(Nod* radacina, FierDeCalcat fier) {
	if (radacina == NULL) {
		return creareNod(fier);
	}

	if (fier.id < radacina->info.id) {
		radacina->stanga = adaugaFierInArbore(radacina->stanga, fier);
	}
	else if (fier.id > radacina->info.id) {
		radacina->dreapta = adaugaFierInArbore(radacina->dreapta, fier);
	}
	else {
		dezalocareFier(&fier);
	}

	return radacina;
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;

	if (file != NULL) {
		while (1) {
			FierDeCalcat f = citireFierDinFisier(file);
			if (f.id != -1) {
				radacina = adaugaFierInArbore(radacina, f);
			}
			else {
				break;
			}
		}
		fclose(file);
	}

	return radacina;
}

void afisareInordine(Nod* radacina) {
	if (radacina != NULL) {
		afisareInordine(radacina->stanga);
		afisareFier(radacina->info);
		afisareInordine(radacina->dreapta);
	}
}

void afisarePreordine(Nod* radacina) {
	if (radacina != NULL) {
		afisareFier(radacina->info);
		afisarePreordine(radacina->stanga);
		afisarePreordine(radacina->dreapta);
	}
}

void afisarePostordine(Nod* radacina) {
	if (radacina != NULL) {
		afisarePostordine(radacina->stanga);
		afisarePostordine(radacina->dreapta);
		afisareFier(radacina->info);
	}
}

FierDeCalcat getFierById(Nod* radacina, int id) {
	if (radacina == NULL) {
		return initializareFierInvalid();
	}

	if (id == radacina->info.id) {
		return copiazaFier(radacina->info);
	}

	if (id < radacina->info.id) {
		return getFierById(radacina->stanga, id);
	}

	return getFierById(radacina->dreapta, id);
}

float calculeazaPretTotalFiareCuAbur(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	float suma = 0;

	if (radacina->info.areAbur == 1) {
		suma += radacina->info.pret;
	}

	suma += calculeazaPretTotalFiareCuAbur(radacina->stanga);
	suma += calculeazaPretTotalFiareCuAbur(radacina->dreapta);

	return suma;
}

int numaraFiareCuPuterePeste(Nod* radacina, int putereMinima) {
	if (radacina == NULL) {
		return 0;
	}

	int nr = 0;

	if (radacina->info.putereW > putereMinima) {
		nr++;
	}

	nr += numaraFiareCuPuterePeste(radacina->stanga, putereMinima);
	nr += numaraFiareCuPuterePeste(radacina->dreapta, putereMinima);

	return nr;
}

int numaraFiareDupaTipTalpa(Nod* radacina, const char* tipTalpa) {
	if (radacina == NULL) {
		return 0;
	}

	int nr = 0;

	if (strcmp(radacina->info.tipTalpa, tipTalpa) == 0) {
		nr++;
	}

	nr += numaraFiareDupaTipTalpa(radacina->stanga, tipTalpa);
	nr += numaraFiareDupaTipTalpa(radacina->dreapta, tipTalpa);

	return nr;
}

void copiazaFiareDupaTipTalpaRecursiv(Nod* radacina, const char* tipTalpa, FierDeCalcat* vector, int* pozitie) {
	if (radacina != NULL) {
		copiazaFiareDupaTipTalpaRecursiv(radacina->stanga, tipTalpa, vector, pozitie);

		if (strcmp(radacina->info.tipTalpa, tipTalpa) == 0) {
			vector[*pozitie] = copiazaFier(radacina->info);
			(*pozitie)++;
		}

		copiazaFiareDupaTipTalpaRecursiv(radacina->dreapta, tipTalpa, vector, pozitie);
	}
}

void copiazaFiareDupaTipTalpa(Nod* radacina, const char* tipTalpa, FierDeCalcat** vector, int* nrFiare) {
	*nrFiare = numaraFiareDupaTipTalpa(radacina, tipTalpa);

	if (*nrFiare > 0) {
		*vector = (FierDeCalcat*)malloc(sizeof(FierDeCalcat) * (*nrFiare));
		int pozitie = 0;
		copiazaFiareDupaTipTalpaRecursiv(radacina, tipTalpa, *vector, &pozitie);
	}
	else {
		*vector = NULL;
	}
}

FierDeCalcat* getFierCuPretMaximRef(Nod* radacina) {
	if (radacina == NULL) {
		return NULL;
	}

	FierDeCalcat* maximCurent = &radacina->info;
	FierDeCalcat* maximStanga = getFierCuPretMaximRef(radacina->stanga);
	FierDeCalcat* maximDreapta = getFierCuPretMaximRef(radacina->dreapta);

	if (maximStanga != NULL && maximStanga->pret > maximCurent->pret) {
		maximCurent = maximStanga;
	}

	if (maximDreapta != NULL && maximDreapta->pret > maximCurent->pret) {
		maximCurent = maximDreapta;
	}

	return maximCurent;
}

FierDeCalcat getFierCuPretMaxim(Nod* radacina) {
	FierDeCalcat* rezultat = getFierCuPretMaximRef(radacina);

	if (rezultat != NULL) {
		return copiazaFier(*rezultat);
	}

	return initializareFierInvalid();
}

void calculeazaSumaSiNumarDupaMarca(Nod* radacina, const char* marca, float* suma, int* nr) {
	if (radacina != NULL) {
		calculeazaSumaSiNumarDupaMarca(radacina->stanga, marca, suma, nr);

		if (strcmp(radacina->info.marca, marca) == 0) {
			*suma += radacina->info.pret;
			(*nr)++;
		}

		calculeazaSumaSiNumarDupaMarca(radacina->dreapta, marca, suma, nr);
	}
}

float calculeazaPretMediuPentruMarca(Nod* radacina, const char* marca) {
	float suma = 0;
	int nr = 0;

	calculeazaSumaSiNumarDupaMarca(radacina, marca, &suma, &nr);

	if (nr > 0) {
		return suma / nr;
	}

	return 0;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

int maxim(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int determinaInaltimeArbore(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	return 1 + maxim(determinaInaltimeArbore(radacina->stanga), determinaInaltimeArbore(radacina->dreapta));
}

void afisareVectorFiare(FierDeCalcat* vector, int nrFiare) {
	for (int i = 0; i < nrFiare; i++) {
		afisareFier(vector[i]);
	}
}

void dezalocareVectorFiare(FierDeCalcat** vector, int* nrFiare) {
	for (int i = 0; i < *nrFiare; i++) {
		dezalocareFier(&(*vector)[i]);
	}

	free(*vector);
	*vector = NULL;
	*nrFiare = 0;
}

void dezalocareArbore(Nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);
		dezalocareFier(&(*radacina)->info);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* radacina = citireArboreDinFisier("fiare.txt");

	printf("afisare inordine:\n\n");
	afisareInordine(radacina);

	printf("fier cautat dupa id:\n\n");
	FierDeCalcat cautat = getFierById(radacina, 5);
	afisareFier(cautat);
	dezalocareFier(&cautat);

	float sumaAbur = calculeazaPretTotalFiareCuAbur(radacina);
	printf("pretul total al fiarelor cu abur: %.2f\n\n", sumaAbur);

	int nrPutere = numaraFiareCuPuterePeste(radacina, 2400);
	printf("numar fiare cu putere peste 2400 W: %d\n\n", nrPutere);

	FierDeCalcat* vector = NULL;
	int nrFiare = 0;

	copiazaFiareDupaTipTalpa(radacina, "ceramica", &vector, &nrFiare);

	printf("fiare copiate cu talpa ceramica:\n\n");
	afisareVectorFiare(vector, nrFiare);

	dezalocareVectorFiare(&vector, &nrFiare);

	printf("fierul cu pret maxim:\n\n");
	FierDeCalcat maximPret = getFierCuPretMaxim(radacina);
	afisareFier(maximPret);
	dezalocareFier(&maximPret);

	float pretMediu = calculeazaPretMediuPentruMarca(radacina, "Tefal");
	printf("pretul mediu pentru marca Tefal: %.2f\n\n", pretMediu);

	int nrNoduri = determinaNumarNoduri(radacina);
	printf("numar total de fiare in arbore: %d\n\n", nrNoduri);

	int inaltime = determinaInaltimeArbore(radacina);
	printf("inaltime arbore: %d\n\n", inaltime);

	dezalocareArbore(&radacina);

	return 0;
}
