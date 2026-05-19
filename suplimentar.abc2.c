#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Pilot Pilot;

struct Pilot {
	int id;
	float greutate;
	char* echipa;
	int premii;
};

typedef struct Nod Nod;
struct Nod {
	Pilot info;
	Nod* stanga;
	Nod* dreapta;
};

Pilot citirePilotDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	Pilot p;

	if (fgets(buffer, 100, f) == NULL) {
		p.id = -1;
		p.echipa = NULL;
		p.greutate = 0;
		p.premii = 0;
		return p;
	}
	char* nou = strtok(buffer, sep);
	p.id = atoi(nou);
	p.greutate = atof(strtok(NULL, sep));

	nou = strtok(NULL, sep);
	p.echipa = (char*)malloc(strlen(nou) + 1);
	strcpy(p.echipa, nou);
	p.premii = atoi(strtok(NULL, sep));
	return p;
}

void adaugaPilotInArbore(Nod** radacina, Pilot PilotNou) {
	if( (*radacina)==NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = PilotNou;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*radacina = nou;
	}

	if (PilotNou.id < (*radacina)->info.id) {
		adaugaPilotInArbore(&(*radacina)->stanga, PilotNou);
	}
	else {


		if (PilotNou.id > (*radacina)->info.id) {
			adaugaPilotInArbore(&(*radacina)->dreapta, PilotNou);
		}
	}
	
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* f;
	
	f = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	if (f) {
		while (!feof(f)) {
			Pilot p = citirePilotDinFisier(f);
			if (p.id != -1) {
				adaugaPilotInArbore(&radacina, p);
			}

		}
		fclose(f);
	}
	return radacina;
}



void afisarePilot(Pilot p) {
	printf("id: %d\n", p.id);
	printf("greutate: %.2f\n", p.greutate);
	printf("echipa: %s\n", p.echipa);
	printf("premii: %d\n\n", p.premii);
}
void afisareInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdine(radacina->stanga);
		afisarePilot(radacina->info);
		afisareInOrdine(radacina->dreapta);
	}
}




int main() {
	Nod* radacina = NULL;
	radacina = citireArboreDinFisier("pilot.txt");
	afisareInOrdine(radacina);
	return 0;
}
