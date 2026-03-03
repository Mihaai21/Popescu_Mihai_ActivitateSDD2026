#include<stdio.h>
#include<string.h>
#include<malloc.h>

struct Magazin {
	char* nume;
	int nrProduse;
};

struct nod {
	Magazin info;
	nod* next;
};

Magazin createMagazin(const char* nume, int nrProduse) {
	Magazin m;
	m.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(m.nume, nume);
	m.nrProduse = nrProduse;
	return m;
}

nod* createNod(Magazin m, nod* next) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = m;
	nou->next = next;
	return nou;
}

nod* inserareSfarsit(nod* cap, Magazin m) {
	nod* nou = createNod(m, NULL);
	if (cap) {
		nod* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		cap = nou;
	}
	return cap;
}

void afisareMagazin(Magazin m) {
	printf("Magazinul %s are %d produse.\n", m.nume, m.nrProduse);
}

void afisareListaMagazine(nod* cap) {
	while (cap) {
		afisareMagazin(cap->info);
		cap = cap->next;
	}
}

nod* inserareInceput(nod* cap, Magazin m) {
	nod* nou = createNod(m, cap);
	return nou;
}

float medieProduse(nod* cap) {
	if (cap) {
		int nr = 0, s = 0;
		while (cap) {
			nr++;
			s += cap->info.nrProduse;
			cap = cap->next;
		}
		return s / nr;
	}
	else {
		return 0;
	}
}

nod* stergereLista(nod* cap) {
	while (cap) {
		nod* aux = cap;
		cap = cap->next;
		free(aux->info.nume);
		free(aux);
	}
	return cap;
}

void stergereListaRecursiv(nod** cap) {
	if (*cap) {
		stergereListaRecursiv(&(*cap)->next);
		free((*cap)->info.nume);
		free(*cap);
		*cap = NULL;
	}
}

nod* inserareInainteDeUnNodCuValoareaData(nod* cap, const char* nume, Magazin m) {
	nod* nou = createNod(m, NULL);
	if (cap) {
		nod* p = cap;
		nod* adresaMax = NULL;

		while (p) {
			if (p->next && strcmp(p->next->info.nume, nume) == 0) {
				if (!adresaMax) {
					adresaMax = p;
				}
				if (p->next->info.nrProduse > adresaMax->next->info.nrProduse) {
					adresaMax = p;
				}
			}
			p = p->next;
		}

		if (adresaMax == NULL) {
			if (strcmp(cap->info.nume, nume) == 0) {
				cap = inserareInceput(cap, m);
			}
			else {
				cap = inserareSfarsit(cap, m);
			}
		}
		else {
			if (strcmp(cap->info.nume, nume) == 0) {
				if (cap->info.nrProduse > adresaMax->info.nrProduse) {
					cap = inserareInceput(cap, m);
					return cap;
				}
			}
			nou->next = adresaMax->next;
			adresaMax->next = nou;
		}
	}
	else {
		cap = nou;
	}
	return cap;
}

void main() {

	nod* cap = NULL;

	cap = inserareSfarsit(cap, createMagazin("Mega", 300));
	cap = inserareSfarsit(cap, createMagazin("Profi", 150));
	cap = inserareSfarsit(cap, createMagazin("Lidl", 500));
	cap = inserareSfarsit(cap, createMagazin("Kaufland", 800));

	afisareListaMagazine(cap);

	cap = inserareInceput(cap, createMagazin("Carrefour", 600));

	printf("\n");
	afisareListaMagazine(cap);

	printf("\nMedia produselor: %5.2f\n", medieProduse(cap));

	printf("\n");

	cap = inserareInainteDeUnNodCuValoareaData(cap, "Lidl", createMagazin("NOU", 200));

	afisareListaMagazine(cap);

	// cap = stergereLista(cap);
	// stergereListaRecursiv(&cap);
}
