#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct Telefon initializare(int id, int RAM, const char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy_s(t.producator, strlen(producator) + 1, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}


	void afisare(struct Telefon t) {
		printf("Id: %d\n", t.id);
		printf("RAM: %d\n", t.RAM);
		printf("Producator: %s\n", t.producator);
		printf("Pret: %.2f\n", t.pret);
		printf("Serie: %c\n", t.serie);
		printf("\n");
	}


void afisareVector(struct Telefon* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Telefon* vectorNou = NULL;
	vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon)*nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = vector[i];
		//deep copy
		vectorNou[i].producator = (char*)malloc(strlen(vector[i].producator )+ 1);
		strcpy_s(vectorNou[i].producator, strlen(vector[i].producator)  + 1, vector[i].producator);
		 
	}
	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++)
	{
		if ((*vector)[i].producator != NULL) {
			free((*vector)[i].producator);
		}


	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
	}


void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pretMinim, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(* dimensiune)++;

}
	}
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	*vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * (*dimensiune));
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s((*vectorNou)[k].producator, strlen(vector[i].producator) + 1, vector[i].producator);
			k++;
		}
	}

}

struct Telefon getPrimulTelefonByProducator(struct Telefon* vector, int nrElemente, const char* producator) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon t;
	t.producator = NULL;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].producator, producator) == 0) {
			t = vector[i];
			//shallow atribuie adresa
			t.producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s(t.producator, strlen(vector[i].producator) + 1, vector[i].producator);
			return t;
		}
}

	return t;
}

int main() {
	
	struct Telefon* telefoane = NULL; //pointer
	int nrTelefoane = 3;
	telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 200, "samsung", 203330, 's');
	telefoane[1] = initializare(1, 6600, "seg", 200, 's');
	telefoane[2] = initializare(1, 230, "samsung", 200, 's');
	afisareVector(telefoane, nrTelefoane);

	struct Telefon* primeleTelefoane = NULL;
	int nrPrimeleTelefoane = 2;

	primeleTelefoane = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrPrimeleTelefoane);
	printf("\n\nPrimeleTelefoane:\n");
	afisareVector(primeleTelefoane, nrPrimeleTelefoane);
	dezalocare(&primeleTelefoane, &nrPrimeleTelefoane);
	afisareVector(primeleTelefoane, nrPrimeleTelefoane);
	struct Telefon* telefoaneScumpe = NULL;
	int nrTelefoaneScumpe = 0;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2000, &telefoaneScumpe, &nrTelefoaneScumpe);
	printf("telefoane scumpe: \n");
	afisareVector(telefoaneScumpe, nrTelefoaneScumpe);
	dezalocare(&telefoaneScumpe, &nrTelefoaneScumpe);
	struct Telefon telefon=getPrimulTelefonByProducator(telefoane, nrTelefoane, "samsung");
printf("tel gasit: \b");
afisare(telefon);

	return 0;
}
