#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Soldat Soldat;
typedef struct Nod Nod;

struct Soldat {
	int id;
	char* nume;
	char* rang;
	int aniExperienta;
	float salariu;
	float punctajMisiune;
};

struct Nod {
	Soldat info;
	Nod* stanga;
	Nod* dreapta;
	int inaltime;
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
		return initializareSoldatInvalid();
	}
	s.nume = copiazaString(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return initializareSoldatInvalid();
	}
	s.rang = copiazaString(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return initializareSoldatInvalid();
	}
	s.aniExperienta = atoi(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return initializareSoldatInvalid();
	}
	s.salariu = atof(aux);

	aux = strtok(NULL, sep);
	if (aux == NULL) {
		dezalocareSoldat(&s);
		return initializareSoldatInvalid();
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

int maxim(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int inaltime(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	return radacina->inaltime;
}

int gradEchilibru(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	return inaltime(radacina->stanga) - inaltime(radacina->dreapta);
}

Nod* creareNod(Soldat soldat) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = soldat;
	nou->stanga = NULL;
	nou->dreapta = NULL;
	nou->inaltime = 1;
	return nou;
}

Nod* rotireDreapta(Nod* radacina) {
	Nod* nouaRadacina = radacina->stanga;
	Nod* aux = nouaRadacina->dreapta;

	nouaRadacina->dreapta = radacina;
	radacina->stanga = aux;

	radacina->inaltime = 1 + maxim(inaltime(radacina->stanga), inaltime(radacina->dreapta));
	nouaRadacina->inaltime = 1 + maxim(inaltime(nouaRadacina->stanga), inaltime(nouaRadacina->dreapta));

	return nouaRadacina;
}

Nod* rotireStanga(Nod* radacina) {
	Nod* nouaRadacina = radacina->dreapta;
	Nod* aux = nouaRadacina->stanga;

	nouaRadacina->stanga = radacina;
	radacina->dreapta = aux;

	radacina->inaltime = 1 + maxim(inaltime(radacina->stanga), inaltime(radacina->dreapta));
	nouaRadacina->inaltime = 1 + maxim(inaltime(nouaRadacina->stanga), inaltime(nouaRadacina->dreapta));

	return nouaRadacina;
}

Nod* adaugaSoldatInAVL(Nod* radacina, Soldat soldat) {
	if (radacina == NULL) {
		return creareNod(soldat);
	}

	if (soldat.id < radacina->info.id) {
		radacina->stanga = adaugaSoldatInAVL(radacina->stanga, soldat);
	}
	else if (soldat.id > radacina->info.id) {
		radacina->dreapta = adaugaSoldatInAVL(radacina->dreapta, soldat);
	}
	else {
		dezalocareSoldat(&soldat);
		return radacina;
	}

	radacina->inaltime = 1 + maxim(inaltime(radacina->stanga), inaltime(radacina->dreapta));

	int echilibru = gradEchilibru(radacina);

	if (echilibru > 1 && soldat.id < radacina->stanga->info.id) {
		return rotireDreapta(radacina);
	}

	if (echilibru < -1 && soldat.id > radacina->dreapta->info.id) {
		return rotireStanga(radacina);
	}

	if (echilibru > 1 && soldat.id > radacina->stanga->info.id) {
		radacina->stanga = rotireStanga(radacina->stanga);
		return rotireDreapta(radacina);
	}

	if (echilibru < -1 && soldat.id < radacina->dreapta->info.id) {
		radacina->dreapta = rotireDreapta(radacina->dreapta);
		return rotireStanga(radacina);
	}

	return radacina;
}

Nod* citireAVLDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;

	if (file != NULL) {
		while (1) {
			Soldat s = citireSoldatDinFisier(file);
			if (s.id != -1) {
				radacina = adaugaSoldatInAVL(radacina, s);
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
		afisareSoldat(radacina->info);
		afisareInordine(radacina->dreapta);
	}
}

void afisarePreordine(Nod* radacina) {
	if (radacina != NULL) {
		afisareSoldat(radacina->info);
		afisarePreordine(radacina->stanga);
		afisarePreordine(radacina->dreapta);
	}
}

void afisarePostordine(Nod* radacina) {
	if (radacina != NULL) {
		afisarePostordine(radacina->stanga);
		afisarePostordine(radacina->dreapta);
		afisareSoldat(radacina->info);
	}
}

Soldat getSoldatById(Nod* radacina, int id) {
	if (radacina == NULL) {
		return initializareSoldatInvalid();
	}

	if (id == radacina->info.id) {
		return copiazaSoldat(radacina->info);
	}

	if (id < radacina->info.id) {
		return getSoldatById(radacina->stanga, id);
	}

	return getSoldatById(radacina->dreapta, id);
}

float calculeazaSumaSalariiDupaRang(Nod* radacina, const char* rangCautat, int aniMinimi) {
	if (radacina == NULL) {
		return 0;
	}

	float suma = 0;

	if (strcmp(radacina->info.rang, rangCautat) == 0 &&
		radacina->info.aniExperienta >= aniMinimi) {
		suma += radacina->info.salariu;
	}

	suma += calculeazaSumaSalariiDupaRang(radacina->stanga, rangCautat, aniMinimi);
	suma += calculeazaSumaSalariiDupaRang(radacina->dreapta, rangCautat, aniMinimi);

	return suma;
}

int numaraSoldatiCuPunctajPeste(Nod* radacina, float prag) {
	if (radacina == NULL) {
		return 0;
	}

	int nr = 0;

	if (radacina->info.punctajMisiune > prag) {
		nr++;
	}

	nr += numaraSoldatiCuPunctajPeste(radacina->stanga, prag);
	nr += numaraSoldatiCuPunctajPeste(radacina->dreapta, prag);

	return nr;
}

int numaraSoldatiDupaRang(Nod* radacina, const char* rangCautat) {
	if (radacina == NULL) {
		return 0;
	}

	int nr = 0;

	if (strcmp(radacina->info.rang, rangCautat) == 0) {
		nr++;
	}

	nr += numaraSoldatiDupaRang(radacina->stanga, rangCautat);
	nr += numaraSoldatiDupaRang(radacina->dreapta, rangCautat);

	return nr;
}

void copiazaSoldatiDupaRangRecursiv(Nod* radacina, const char* rangCautat, Soldat* vector, int* pozitie) {
	if (radacina != NULL) {
		copiazaSoldatiDupaRangRecursiv(radacina->stanga, rangCautat, vector, pozitie);

		if (strcmp(radacina->info.rang, rangCautat) == 0) {
			vector[*pozitie] = copiazaSoldat(radacina->info);
			(*pozitie)++;
		}

		copiazaSoldatiDupaRangRecursiv(radacina->dreapta, rangCautat, vector, pozitie);
	}
}

void copiazaSoldatiDupaRang(Nod* radacina, const char* rangCautat, Soldat** vector, int* nrSoldati) {
	*nrSoldati = numaraSoldatiDupaRang(radacina, rangCautat);

	if (*nrSoldati > 0) {
		*vector = (Soldat*)malloc(sizeof(Soldat) * (*nrSoldati));
		int pozitie = 0;
		copiazaSoldatiDupaRangRecursiv(radacina, rangCautat, *vector, &pozitie);
	}
	else {
		*vector = NULL;
	}
}

Soldat* getSoldatCuPunctajMaximRef(Nod* radacina) {
	if (radacina == NULL) {
		return NULL;
	}

	Soldat* maximCurent = &radacina->info;
	Soldat* maximStanga = getSoldatCuPunctajMaximRef(radacina->stanga);
	Soldat* maximDreapta = getSoldatCuPunctajMaximRef(radacina->dreapta);

	if (maximStanga != NULL && maximStanga->punctajMisiune > maximCurent->punctajMisiune) {
		maximCurent = maximStanga;
	}

	if (maximDreapta != NULL && maximDreapta->punctajMisiune > maximCurent->punctajMisiune) {
		maximCurent = maximDreapta;
	}

	return maximCurent;
}

Soldat getSoldatCuPunctajMaxim(Nod* radacina) {
	Soldat* rezultat = getSoldatCuPunctajMaximRef(radacina);

	if (rezultat != NULL) {
		return copiazaSoldat(*rezultat);
	}

	return initializareSoldatInvalid();
}

void afisareVectorSoldati(Soldat* vector, int nrSoldati) {
	for (int i = 0; i < nrSoldati; i++) {
		afisareSoldat(vector[i]);
	}
}

void dezalocareVectorSoldati(Soldat** vector, int* nrSoldati) {
	for (int i = 0; i < *nrSoldati; i++) {
		dezalocareSoldat(&(*vector)[i]);
	}

	free(*vector);
	*vector = NULL;
	*nrSoldati = 0;
}

void dezalocareArbore(Nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);
		dezalocareSoldat(&(*radacina)->info);
		free(*radacina);
		*radacina = NULL;
	}
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}

	return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

int main() {
	Nod* radacina = citireAVLDinFisier("soldati.txt");

	printf("afisare inordine:\n\n");
	afisareInordine(radacina);

	printf("soldat cautat dupa id:\n\n");
	Soldat cautat = getSoldatById(radacina, 6);
	afisareSoldat(cautat);
	dezalocareSoldat(&cautat);

	float suma = calculeazaSumaSalariiDupaRang(radacina, "sergent", 5);
	printf("suma salariilor pentru sergenti cu minim 5 ani experienta: %.2f\n\n", suma);

	int nrPunctaj = numaraSoldatiCuPunctajPeste(radacina, 85);
	printf("numar soldati cu punctaj peste 85: %d\n\n", nrPunctaj);

	Soldat* vector = NULL;
	int nrSoldati = 0;

	copiazaSoldatiDupaRang(radacina, "locotenent", &vector, &nrSoldati);

	printf("soldati copiati cu rangul locotenent:\n\n");
	afisareVectorSoldati(vector, nrSoldati);

	dezalocareVectorSoldati(&vector, &nrSoldati);

	printf("soldatul cu punctaj maxim:\n\n");
	Soldat maximPunctaj = getSoldatCuPunctajMaxim(radacina);
	afisareSoldat(maximPunctaj);
	dezalocareSoldat(&maximPunctaj);

	int nrNoduri = determinaNumarNoduri(radacina);
	printf("numar total de soldati in arbore: %d\n\n", nrNoduri);

	dezalocareArbore(&radacina);

	return 0;
}
