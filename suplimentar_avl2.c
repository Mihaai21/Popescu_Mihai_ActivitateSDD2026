#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Telefon Telefon;

struct Telefon {
	int id;
	float pret;
	char* producator;
	int taste; 
};

typedef struct Nod Nod;

struct Nod {
	Telefon info;
	Nod* stanga;
	Nod* dreapta;
	int inaltime;
};

Telefon citireDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	Telefon t;

	if (fgets(buffer, 100, f) == NULL) {
		t.id = -1;
		t.producator = NULL;
		return t;
	}

	char* aux = strtok(buffer, sep);
	t.id = atoi(aux);

	t.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	t.producator = (char*)malloc(strlen(aux) + 1);
	strcpy(t.producator, aux);

	t.taste = atoi(strtok(NULL, sep));

	return t;
}
void afisare(Telefon t) {
	printf("id: %d\n", t.id);
	printf("pret: %.2f\n", t.pret);
	printf("producator: %s\n", t.producator);
	printf("taste: %d\n\n", t.taste);
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

int factorEchilibru(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	return inaltime(radacina->stanga) - inaltime(radacina->dreapta);
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

Nod* adaugaInAVL(Nod* radacina, Telefon telefonNou) {
	if (radacina == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = telefonNou;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		nou->inaltime = 1;
		return nou;
	}

	if (telefonNou.id < radacina->info.id) {
		radacina->stanga = adaugaInAVL(radacina->stanga, telefonNou);
	}
	else {
		if (telefonNou.id > radacina->info.id) {
			radacina->dreapta = adaugaInAVL(radacina->dreapta, telefonNou);
		}
		else {
			return radacina;
		}
	}

	radacina->inaltime = 1 + maxim(inaltime(radacina->stanga), inaltime(radacina->dreapta));

	int fe = factorEchilibru(radacina);

	if (fe > 1 && telefonNou.id < radacina->stanga->info.id) {
		return rotireDreapta(radacina);
	}

	if (fe < -1 && telefonNou.id > radacina->dreapta->info.id) {
		return rotireStanga(radacina);
	}

	if (fe > 1 && telefonNou.id > radacina->stanga->info.id) {
		radacina->stanga = rotireStanga(radacina->stanga);
		return rotireDreapta(radacina);
	}

	if (fe < -1 && telefonNou.id < radacina->dreapta->info.id) {
		radacina->dreapta = rotireDreapta(radacina->dreapta);
		return rotireStanga(radacina);
	}

	return radacina;
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* radacina = NULL;

	if (f) {
		while (!feof(f)) {
			Telefon t = citireDinFisier(f);
			if (t.id != -1) {
				radacina = adaugaInAVL(radacina, t);
			}
		}
		fclose(f);
	}

	return radacina;
}

void afisareInOrdine(Nod* radacina) {
	if (radacina!=NULL) {
		afisareInOrdine(radacina->stanga);
		afisare(radacina->info);
		afisareInOrdine(radacina->dreapta);
	}
}

int main() {
	Nod* radacina = NULL;

	radacina = citireArboreDinFisier("telefon.txt");
	printf("tel in arbore\n \n");
	afisareInOrdine(radacina);
	return 0;
}
