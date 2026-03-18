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
typedef struct ListaDubla ListaDubla;
typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};
//creare structura pentru un nod dintr-o lista simplu inlantuita

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}
void afisareInversaListaMasini(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

//adaugarea in lista
void adaugaMasinaInLista(ListaDubla* ld, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = ld->ultim;
	if (ld->ultim) {
		ld->ultim->next = nou; 
		

	}
	else {
		ld->prim = nou;
	}
	ld->ultim = nou;
}
//adaugare la inceput
void adaugaLaInceputInLista(ListaDubla* ld, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = ld->prim;
	nou->prev = NULL;
	if (ld->prim) {
		ld->prim->prev = nou;

	}
	else {
		ld->ultim = nou;
	}
	ld->prim = nou;
}

ListaDubla citireListaMasiniDinFisier(const char* numeFisier) {


	FILE* f = fopen(numeFisier, "r");
	if (f) {
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;
		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}
		fclose(f);
		return ld;
	}

}

void dezalocareListaMasini(ListaDubla *ld) {
	while (ld->prim&&ld->prim->next) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		free(ld->prim);
	}
	ld->prim = ld->ultim = NULL;
}


	float calculeazaPretMediu(ListaDubla lista) {
		Nod* p = lista.prim;
		float suma = 0;
		int contor = 0;
		while (p) {
			suma += p->info.pret;
			contor++;
			p = p->next;
		}
		if (contor == 0)
			return 0;

		return suma / contor;
	}


	
	void stergeMasiniDupaId(ListaDubla* ld, int id) {
		Nod* p = ld->prim;

		while (p && p->info.id != id) {
			p = p->next;
		}

		if (!p) return; // id-ul nu a fost gasit

		// dezleaga nodul din lista
		if (p->prev)
			p->prev->next = p->next;
		else
			ld->prim = p->next; // era primul nod

		if (p->next)
			p->next->prev = p->prev;
		else
			ld->ultim = p->prev; // era ultimul nod

		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}

int main() {
	ListaDubla lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	float pretMediu = calculeazaPretMediu(lista);
	printf("Pretul mediu: %.2f\n", pretMediu);
	stergeMasiniDupaId(&lista, 2); // sterge masina cu id-ul 2
	printf("\nDupa stergere id=2:\n");
	afisareListaMasini(lista);
	return 0;
}
