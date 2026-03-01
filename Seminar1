#include<stdio.h>
#include<stdlib.h>

struct Produs {
	int id;
	int stoc;
	char* denumire;
	float pret;
	char categorie;

};

struct Produs initializare(int id, int stoc, char* denumire, float pret, char categorie) {
	struct Produs s;
	s.id = 1;
	s.stoc = stoc;
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(s.denumire, denumire);

	s.pret = pret;
	s.categorie = categorie;
	return s;
}

void afisare(struct Produs s) {
	printf("\nID: %d\n", s.id);
	printf("stoc:%d \n", s.stoc);
	printf("denumire: %s\n", s.denumire);
	printf("pret: %5.2f\n", s.pret);
	printf("categ: %c\n", s.categorie);

}

void modifica_Denumire(struct Produs* s, char* nouDenumire) {
	free((*s).denumire);
	s->denumire = (char*)malloc(sizeof(char) * strlen(nouDenumire) + 1);
	strcpy(s->denumire, nouDenumire);
}

void dezalocare(struct Produs* s) {
	if (s->denumire != NULL) {
		free(s->denumire);
		s->denumire = NULL;
	}

}


//functie val totala
float CalculTotal(struct Produs p) {
	return p.stoc * p.pret;
}

int main() {
	struct Produs s;
	s = initializare(1, 20, "telefon", 2000, 'A');
	afisare(s);
	modifica_Denumire(&s, "Mouse");
	afisare(s);
	dezalocare(&s);
	printf("val tot: %5.2f ron", CalculTotal(s));
	return 0;
}

