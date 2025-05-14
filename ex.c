#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int zona;
    int ore;
    char numarMasina[50];
} Plata;

void afisareMeniu() {
    printf("\033[1;36m\n===== Sistem de plata parcare online =====\033[0m\n");
    printf("\033[1;33m1.\033[0m \033[1;37mPlateste parcare\033[0m\n");
    printf("\033[1;33m2.\033[0m \033[1;37mVizualizeaza istoric\033[0m\n");
    printf("\033[1;33m3.\033[0m \033[1;37mIesire\033[0m\n");
    printf("\033[1;36mAlege o optiune: \033[0m");
}

void afisareZona() {
    printf("\n\033[1;35m--- Selecteaza Zona ---\033[0m\n");
    printf("\033[1;33m1.\033[0m Zona Galbena (\033[1;32m1 RON/ora\033[0m)\n");
    printf("\033[1;31m2.\033[0m Zona Rosie (\033[1;32m1.5 RON/ora\033[0m)\n");
    printf("\033[1;32m3.\033[0m Zona Verde (\033[1;32m2 RON/ora\033[0m)\n");
    printf("\033[1;36mAlege o zona: \033[0m");
}

int validareNumarMasina(const char* numar) {
    int len = strlen(numar);
    if (len < 6 || len > 8) return 0;

    int i = 0;

    // 1-2 litere
    if (!isalpha(numar[i])) return 0;
    i++;
    if (isalpha(numar[i])) i++;

     // 2-3 cifre
    int cifre = 0;
    while (isdigit(numar[i]) && cifre < 3) {
        i++; cifre++;
    }
    if (cifre < 2) return 0;

    // 2-3 litere
    int litere = 0;
    while (isalpha(numar[i]) && litere < 3) {
        i++; litere++;
    }
    if (litere < 2 || numar[i] != '\0') return 0;

    return 1;
}

void citesteNumarMasina(char* buffer) {
    int valid;
    do {
        printf("\033[1;36mIntroduceti numarul masinii (ex: B123XYZ): \033[0m");
        scanf("%s", buffer);
        for (int i = 0; buffer[i]; i++)
            buffer[i] = toupper(buffer[i]);

        valid = validareNumarMasina(buffer);
        if (!valid) {
            printf("\033[1;31mNumar invalid! Formatul trebuie sa fie de forma B123XYZ.\033[0m\n");
        }
    } while (!valid);
}

void salvarePlata(Plata p) {
    FILE *fisier = fopen("istoric.txt", "a");
    if (fisier == NULL) {
        printf("\033[1;31mEroare la deschiderea fisierului!\033[0m\n");
        return;
    }

    const char* numeZona;
    const char* culoareZona;
    double tarif;

    switch (p.zona) {
        case 1:
            numeZona = "Zona Galbena";
            culoareZona = "\033[1;33m";
            tarif = 1.0;
            break;
        case 2:
            numeZona = "Zona Rosie";
            culoareZona = "\033[1;31m";
            tarif = 1.5;
            break;
        case 3:
            numeZona = "Zona Verde";
            culoareZona = "\033[1;32m";
            tarif = 2.0;
            break;
        default:
            printf("\033[1;31mOptiune zona invalida!\033[0m\n");
            fclose(fisier);
            return;
    }

    double total = tarif * p.ore;

    fprintf(fisier, "Plata: %s - %d ore - %.2f RON - Masina: %s\n", numeZona, p.ore, total, p.numarMasina);
    fclose(fisier);

    printf("\n\033[1;32mPlata efectuata cu succes:\033[0m ");
    printf("%s%s\033[0m - ", culoareZona, numeZona);
    printf("%d ore - \033[1;36m%.2f RON\033[0m - ", p.ore, total);
    printf("Masina: \033[1;34m%s\033[0m\n", p.numarMasina);
}

void vizualizareIstoric() {
    FILE *fisier = fopen("istoric.txt", "r");
    if (fisier == NULL) {
        printf("\033[1;33mNu exista istoric de plati.\033[0m\n");
        return;
    }

    char linie[256];
    printf("\n\033[1;35m--- Istoric Plati ---\033[0m\n");

    while (fgets(linie, sizeof(linie), fisier)) {
        char zona[50], masina[50];
        int ore;
        float suma;
        if (sscanf(linie, "Plata: %[^\n-] - %d ore - %f RON - Masina: %s", zona, &ore, &suma, masina) == 4) {
            const char* culoareZona = "\033[0m";
        if (strstr(zona, "Galbena")) culoareZona = "\033[1;33m";
            else if (strstr(zona, "Rosie")) culoareZona = "\033[1;31m";
            else if (strstr(zona, "Verde")) culoareZona = "\033[1;32m";
            printf("Plata: %s%s\033[0m - %d ore - ", culoareZona, zona, ore);
            printf("\033[1;36m%.2f RON\033[0m - Masina: \033[1;34m%s\033[0m\n", suma, masina);
        } else {

            printf("\033[1;30m%s\033[0m", linie);
        }
    }

    fclose(fisier);
}


int main() {
    int optiune;

    do {
        afisareMeniu();
        if (scanf("%d", &optiune) != 1) {
            printf("\033[1;31mInput invalid! Introduceti o cifra.\033[0m\n");
            while (getchar() != '\n');
            continue;
        }

        system("cls || clear");

        switch (optiune) {
            case 1: {
                Plata p;
                printf("\n\033[1;37mAti ales sa platiti parcarea.\033[0m\n");

                // Zona
                do {
                    afisareZona();
                    if (scanf("%d", &p.zona) != 1 || p.zona < 1 || p.zona > 3) {
                        printf("\033[1;31mZona invalida. Incercati din nou.\033[0m\n");
                        while (getchar() != '\n');
                        p.zona = 0;
                    }
                } while (p.zona < 1 || p.zona > 3);

                // Ore
                do {
                    printf("\033[1;36mIntroduceti numarul de ore: \033[0m");
                    if (scanf("%d", &p.ore) != 1 || p.ore <= 0) {
                        printf("\033[1;31mNumar de ore invalid! Introduceti o valoare pozitiva.\033[0m\n");
                        while (getchar() != '\n');
                        p.ore = -1;
                    }
                } while (p.ore <= 0);

                citesteNumarMasina(p.numarMasina);
                salvarePlata(p);
                break;
            }
            case 2:
                vizualizareIstoric();
                break;
            case 3:
                printf("\033[1;33mIesire din aplicatie...\033[0m\n");
                break;
            default:
                printf("\033[1;31mOptiune invalida. Incercati din nou.\033[0m\n");
        }
    } while (optiune != 3);

    return 0;
}
