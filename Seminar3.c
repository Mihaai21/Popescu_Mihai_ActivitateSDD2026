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

void afisareMasina(Masina masina) {
	printf("id masina: %d", masina.id);
	printf("nr usi: %d", masina.nrUsi);
	printf("pret: %d", masina.pret);
	printf("model: %d", masina.model);
	printf("nume: %d", masina.numeSofer);
	printf("serie: %d", masina.serie);

}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
		}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < *nrMasini; i++)
	{
		temp[i] = (*masini)[i];
		temp[*nrMasini] = masinaNoua;
		free(*masini);
		(*masini) = temp;
		(*nrMasini)++; 
	}



}

Masina citireMasinaFisier(FILE* file) {
	char buff[50];

	Masina m;
	fgets(buff, 50, file);
	char var[3] = ",\n";
	
	m.id = atoi(strtok(buff, var));
	m.nrUsi=atoi(strtok(NULL, var));
	m.pret=atof(strtok(NULL, var));
	char* aux = strtok(NULL, var);
	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.model, aux);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(m.numeSofer, aux);
	m.serie = (strtok(NULL, var))[0];
	return m;


}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {	
	FILE* file;
	file = fopen(numeFisier, "r");
	Masina* masini = NULL;
	while (!feof(file)) {
		adaugaMasinaInVector(&masini, *nrMasiniCitite, citireMasinaFisier(file));
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++)
	{
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);

	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}
int main() {


	int nrMasini = 0;
	Masina* masini = NULL;
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
		return 0;



	return 0;
}

