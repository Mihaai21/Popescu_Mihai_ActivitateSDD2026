#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include<stdlib.h>
#include<stdio.h>



typedef struct Carte Carte;

struct Carte {
	int id;
	float pret;
	char* titlu;
};

typedef struct Nod Nod;
struct Nod {
	Carte info;
	Nod* stanga;
	Nod* dreapta;
};

Carte citireDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	Carte s;

	if (fgets(buffer, 100, f) == NULL) {
		s.id = -1;
		s.pret = 0;
		s.titlu = NULL;
		return s;
	}

	char* aux = strtok(buffer, sep);
	s.id = atoi(aux);

	s.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	s.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(s.titlu, aux);

	return s;
}

void afisare(Carte s) {
	if (s.id != -1) {
		printf("id: %d\n", s.id);
		printf("pret: %.2f\n", s.pret);
		printf("titlu: %s\n\n", s.titlu);
	}
}


void AdaugaiInArbore(Nod** radacina, Carte carteNou) {
	if (*radacina == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = carteNou;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*radacina = nou;
	}
	else {
		if (carteNou.id < (*radacina)->info.id) {
			AdaugaiInArbore(&(*radacina)->stanga, carteNou);
		}
		else {
			AdaugaiInArbore(&(*radacina)->dreapta, carteNou);
		}
	}
}


Nod* citireArboreDinFisier(const char* nume_fisier) {
	FILE * f;
	Nod* radacina = NULL;
		f = fopen(nume_fisier, "r");
		if (f) {
			while (!feof(f)) {
				Carte c = citireDinFisier(f);

				if (c.id != -1) {
					AdaugaiInArbore(&radacina, c);
				}
			}
			fclose(f);
		}

		return radacina;
}

void afisareInordine(Nod* radacina) {
	if (radacina != NULL) {
		afisareInordine(radacina->stanga);
		afisare(radacina->info);
		afisareInordine(radacina->dreapta);
	}
}

Carte getById(Nod* radacina, int id)
{
	if (radacina != NULL) {
		if (id == radacina->info.id) {
			Carte c;
			c.id = radacina->info.id;
			c.pret = radacina->info.pret;
			c.titlu = (char*)malloc(strlen(radacina->info.titlu) + 1);
			strcpy(c.titlu, radacina->info.titlu);
		}
		else {
			if(id<radacina->info.id)
			{
				return getById(radacina->stanga,id);

		}
			else {
				return getById(radacina->dreapta, id);
			}

		}
	
	}
	Carte c;
	c.id = -1;
	c.pret = 0;
	c.titlu = NULL;
	return c;

}
void dezalocareArbore(Nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);

		if ((*radacina)->info.titlu != NULL) {
			free((*radacina)->info.titlu);
		}

		free(*radacina);
		*radacina = NULL;
	}
}
int main() {
	Nod* radacina = NULL;

	radacina = citireArboreDinFisier("carti.txt");

	printf("studentii din arbore:\n\n");
	afisareInordine(radacina);

	Carte CarteCautat = getById(radacina, 2);

	printf("Carteul cautat:\n\n");
	if (CarteCautat.id != -1) {
		afisare(CarteCautat);
	}
	else {
		printf("Carteul nu a fost gasit\n");
	}

	if (CarteCautat.titlu != NULL) {
		free(CarteCautat.titlu);
	}

	dezalocareArbore(&radacina);

	return 0;
}
