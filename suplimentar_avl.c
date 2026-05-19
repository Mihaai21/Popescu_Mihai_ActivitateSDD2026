#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

//folosim fisierul masini.txt

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct NodArbore NodArbore;
struct NodArbore {
	Masina info;
	NodArbore* right;
	NodArbore* left;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Masina m1;

	if (!fgets(buffer, 100, file)) {
		m1.id = -1;
		m1.model = NULL;
		m1.numeSofer = NULL;
		return m1;
	}

	char* aux;
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
	if (masina.id == -1) return;

	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(NodArbore* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina)
	{
		int dreapta = calculeazaInaltimeArbore(radacina->right);
		int stanga = calculeazaInaltimeArbore(radacina->left);
		return 1 + max(dreapta, stanga);
	}
	return 0;
}

void rotireStanga(NodArbore** radacina)
{
	//retinem nodul din dreapta, in poza 2
	NodArbore* aux = (*radacina)->right;
	(*radacina)->right = aux->left; //B
	aux->left = *radacina;
	*radacina = aux;

}
void rotireDreapta(NodArbore** radacina)
{
	NodArbore* aux = (*radacina)->left;
	(*radacina)->left = aux->right;
	aux->right = *radacina;
	*radacina = aux;

}
int verificareEchilibru(NodArbore* radacina)
{
	//calculez inaltimea pe stanga
	int dr = calculeazaInaltimeArbore(radacina->right);
	int st = calculeazaInaltimeArbore(radacina->left);

	return st - dr;
}
void adaugaMasinaInArboreEchilibrat(NodArbore** radacina, Masina masinaNoua) {

	if (*radacina == NULL) //nu avem radacina
	{
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->left = NULL;
		nou->right = NULL;
		nou->info = masinaNoua;
		*radacina = nou;
	}
	else //avem radacina
	{
		if (masinaNoua.id > (*radacina)->info.id)  //punem in dreapta
		{
			adaugaMasinaInArboreEchilibrat(&(*radacina)->right, masinaNoua);
		}
		else
		{
			adaugaMasinaInArboreEchilibrat(&(*radacina)->left, masinaNoua);
		}

		//incepe verificarea de echilibru
		int grad_echilibru = verificareEchilibru(*radacina);
		if (grad_echilibru == -2) //dezechilibru pe partea dreapta
		{
			if (verificareEchilibru((*radacina)->right) == -1)  //dezechilibru complet
			{
				rotireStanga(radacina);
			}
			else // semi dezechilibru deci trebuie 2 rotiri
			{
				rotireDreapta(&(*radacina)->right);
				rotireStanga(radacina);
			}
		}
		if (grad_echilibru == 2) //dezechilibru pe partea stanga
		{
			if (verificareEchilibru((*radacina)->left) == -1)
			{
				rotireStanga(&(*radacina)->left);
			}
			rotireDreapta(radacina);
		}
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {

	FILE* file = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Masina masina = citireMasinaDinFisier(file);
			if (masina.id != -1)
			{
				adaugaMasinaInArboreEchilibrat(&radacina, masina);
			}
		}
		fclose(file);
	}
	return radacina;
}

void afisareMasiniDinArbore(NodArbore* radacina) {
	//preordine 
	if (radacina)
	{
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->left);
		afisareMasiniDinArbore(radacina->right);
	}
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	if (*radacina)
	{
		dezalocareArboreDeMasini(&(*radacina)->left);
		dezalocareArboreDeMasini(&(*radacina)->right);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);

		free(*radacina);
		*radacina = NULL;

	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(NodArbore* radacina, int id) {
	if (radacina) {
		if (radacina->info.id == id) {
			Masina m;
			m.id = radacina->info.id;
			m.nrUsi = radacina->info.nrUsi;
			m.pret = radacina->info.pret;
			m.serie = radacina->info.serie;

			m.model = (char*)malloc(strlen(radacina->info.model) + 1);
			strcpy_s(m.model, strlen(radacina->info.model) + 1, radacina->info.model);

			m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(radacina->info.numeSofer) + 1, radacina->info.numeSofer);

			return m;
		}

		if (radacina->info.id < id)
			return getMasinaByID(radacina->right, id);
		else
			return getMasinaByID(radacina->left, id);
	}

	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;
	return m;
}

int determinaNumarNoduri(NodArbore* radacina) {
	if (radacina) {
		return 1 + determinaNumarNoduri(radacina->left) +
			determinaNumarNoduri(radacina->right);
	}
	return 0;
}

float calculeazaPretTotal(NodArbore* radacina) {
	if (radacina) {
		return radacina->info.pret +
			calculeazaPretTotal(radacina->left) +
			calculeazaPretTotal(radacina->right);
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	if (radacina) {
		float suma = calculeazaPretulMasinilorUnuiSofer(radacina->left, numeSofer) +
			calculeazaPretulMasinilorUnuiSofer(radacina->right, numeSofer);

		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
			suma += radacina->info.pret;

		return suma;
	}
	return 0;
}

int main() {

	NodArbore* radacina = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(radacina);

	dezalocareArboreDeMasini(&radacina);
	return 0;
}
