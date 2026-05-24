#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Soldat Soldat;
typedef struct NodP NodP;
typedef struct NodS NodS;

struct Soldat {
	int id;
	char* nume;
	char* rang;
	int aniExperienta;
	float salariu;
	float punctajMisiune;
};

struct NodP {
	Soldat info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
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

void inserareInListaP(NodP** graf, Soldat soldatNou) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->info = soldatNou;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*graf != NULL) {
		NodP* aux = *graf;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareInListaS(NodS** listaS, NodP* vecin) {
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;

	if (*listaS != NULL) {
		NodS* aux = *listaS;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*listaS = nou;
	}
}

NodP* cautaNodDupaID(NodP* graf, int id) {
	NodP* aux = graf;

	while (aux != NULL) {
		if (aux->info.id == id) {
			return aux;
		}
		aux = aux->next;
	}

	return NULL;
}

int existaVecin(NodS* listaS, int idVecin) {
	while (listaS != NULL) {
		if (listaS->info->info.id == idVecin) {
			return 1;
		}
		listaS = listaS->next;
	}
	return 0;
}

void inserareMuchie(NodP* graf, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(graf, idStart);
	NodP* stop = cautaNodDupaID(graf, idStop);

	if (start != NULL && stop != NULL && idStart != idStop) {
		if (!existaVecin(start->vecini, idStop)) {
			inserareInListaS(&(start->vecini), stop);
		}

		if (!existaVecin(stop->vecini, idStart)) {
			inserareInListaS(&(stop->vecini), start);
		}
	}
}

NodP* citireNoduriSoldatiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodP* graf = NULL;

	if (file != NULL) {
		while (1) {
			Soldat s = citireSoldatDinFisier(file);

			if (s.id != -1) {
				inserareInListaP(&graf, s);
			}
			else {
				break;
			}
		}

		fclose(file);
	}

	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* file = fopen(numeFisier, "r");

	if (file != NULL) {
		int idStart = 0;
		int idStop = 0;

		while (fscanf(file, "%d %d", &idStart, &idStop) == 2) {
			inserareMuchie(graf, idStart, idStop);
		}

		fclose(file);
	}
}

void afiseazaListaDeVecini(int id, NodP* graf) {
	NodP* nod = cautaNodDupaID(graf, id);

	if (nod != NULL) {
		printf("vecinii soldatului cu id-ul %d sunt:\n\n", id);

		NodS* lista = nod->vecini;

		if (lista == NULL) {
			printf("soldatul nu are vecini\n\n");
		}

		while (lista != NULL) {
			afisareSoldat(lista->info->info);
			lista = lista->next;
		}
	}
	else {
		printf("nu exista soldat cu id-ul %d\n\n", id);
	}
}

void afisareGraf(NodP* graf) {
	while (graf != NULL) {
		printf("soldatul:\n");
		afisareSoldat(graf->info);

		printf("are vecinii:\n");

		NodS* vecini = graf->vecini;

		if (vecini == NULL) {
			printf("nu are vecini\n");
		}

		while (vecini != NULL) {
			printf("%d - %s\n", vecini->info->info.id, vecini->info->info.nume);
			vecini = vecini->next;
		}

		printf("\n----------------------\n\n");
		graf = graf->next;
	}
}

int numaraVecini(int id, NodP* graf) {
	NodP* nod = cautaNodDupaID(graf, id);

	if (nod == NULL) {
		return 0;
	}

	int nr = 0;
	NodS* lista = nod->vecini;

	while (lista != NULL) {
		nr++;
		lista = lista->next;
	}

	return nr;
}

float calculeazaSumaSalariiVeciniDupaRang(NodP* graf, int id, const char* rangCautat) {
	NodP* nod = cautaNodDupaID(graf, id);

	if (nod == NULL) {
		return 0;
	}

	float suma = 0;
	NodS* lista = nod->vecini;

	while (lista != NULL) {
		if (strcmp(lista->info->info.rang, rangCautat) == 0) {
			suma += lista->info->info.salariu;
		}

		lista = lista->next;
	}

	return suma;
}

Soldat getSoldatCuCeiMaiMultiVecini(NodP* graf) {
	if (graf == NULL) {
		return initializareSoldatInvalid();
	}

	NodP* aux = graf;
	NodP* nodMaxim = graf;
	int maximVecini = numaraVecini(graf->info.id, graf);

	while (aux != NULL) {
		int nrVecini = numaraVecini(aux->info.id, graf);

		if (nrVecini > maximVecini) {
			maximVecini = nrVecini;
			nodMaxim = aux;
		}

		aux = aux->next;
	}

	return copiazaSoldat(nodMaxim->info);
}

int numaraVeciniCuPunctajPeste(NodP* graf, int id, float punctajMinim) {
	NodP* nod = cautaNodDupaID(graf, id);

	if (nod == NULL) {
		return 0;
	}

	int nr = 0;
	NodS* lista = nod->vecini;

	while (lista != NULL) {
		if (lista->info->info.punctajMisiune > punctajMinim) {
			nr++;
		}

		lista = lista->next;
	}

	return nr;
}

void copiazaVeciniCuPunctajPeste(NodP* graf, int id, float punctajMinim, Soldat** vector, int* nrSoldati) {
	*nrSoldati = numaraVeciniCuPunctajPeste(graf, id, punctajMinim);

	if (*nrSoldati > 0) {
		*vector = (Soldat*)malloc(sizeof(Soldat) * (*nrSoldati));

		NodP* nod = cautaNodDupaID(graf, id);
		NodS* lista = nod->vecini;
		int k = 0;

		while (lista != NULL) {
			if (lista->info->info.punctajMisiune > punctajMinim) {
				(*vector)[k] = copiazaSoldat(lista->info->info);
				k++;
			}

			lista = lista->next;
		}
	}
	else {
		*vector = NULL;
	}
}

int numaraMuchii(NodP* graf) {
	int nr = 0;

	while (graf != NULL) {
		NodS* lista = graf->vecini;

		while (lista != NULL) {
			nr++;
			lista = lista->next;
		}

		graf = graf->next;
	}

	return nr / 2;
}

int numaraNoduri(NodP* graf) {
	int nr = 0;

	while (graf != NULL) {
		nr++;
		graf = graf->next;
	}

	return nr;
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

void dezalocareGraf(NodP** graf) {
	while (*graf != NULL) {
		NodP* nodCurent = *graf;
		*graf = (*graf)->next;

		NodS* vecin = nodCurent->vecini;

		while (vecin != NULL) {
			NodS* temp = vecin;
			vecin = vecin->next;
			free(temp);
		}

		dezalocareSoldat(&nodCurent->info);
		free(nodCurent);
	}
}

int main() {
	NodP* graf = citireNoduriSoldatiDinFisier("soldati.txt");
	citireMuchiiDinFisier("muchii.txt", graf);

	printf("graful complet:\n\n");
	afisareGraf(graf);

	printf("lista de vecini pentru soldatul cu id-ul 1:\n\n");
	afiseazaListaDeVecini(1, graf);

	int nrVecini = numaraVecini(1, graf);
	printf("soldatul cu id-ul 1 are %d vecini\n\n", nrVecini);

	float suma = calculeazaSumaSalariiVeciniDupaRang(graf, 1, "sergent");
	printf("suma salariilor vecinilor cu rangul sergent pentru soldatul 1 este: %.2f\n\n", suma);

	Soldat maximVecini = getSoldatCuCeiMaiMultiVecini(graf);
	printf("soldatul cu cei mai multi vecini este:\n\n");
	afisareSoldat(maximVecini);
	dezalocareSoldat(&maximVecini);

	Soldat* vector = NULL;
	int nrSoldati = 0;

	copiazaVeciniCuPunctajPeste(graf, 1, 80, &vector, &nrSoldati);

	printf("vecinii soldatului 1 cu punctaj peste 80 sunt:\n\n");
	afisareVectorSoldati(vector, nrSoldati);

	dezalocareVectorSoldati(&vector, &nrSoldati);

	int nrNoduri = numaraNoduri(graf);
	int nrMuchii = numaraMuchii(graf);

	printf("numar noduri in graf: %d\n", nrNoduri);
	printf("numar muchii in graf: %d\n\n", nrMuchii);

	dezalocareGraf(&graf);

	return 0;
}
