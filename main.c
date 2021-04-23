#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Create reservation struct instance from parameters

struct reservation* create_reservation(int month, int day, int hour, char* description) {

    struct reservation* r = malloc(sizeof(struct reservation));
    r->month = month;
    r->day = day;
    r->hour = hour;
    r->description = malloc(sizeof(char) * 80);
    strcpy(r->description, description);

    return r;

}

// Delete reservation with parameters

struct calendar* del_reservation(int month, int day, int hour, struct calendar* cal) {

    for (int i = 0; i < cal->size; i++) {

        struct reservation r = *(cal->ptr + i);
        
        if (r.month == month && r.day == day && r.hour == hour) {

            return remove_reservation(i, cal);

        }

    }

    printf("Varausta ei löytynyt. Yritä uudestaan.");

    return cal;

}

// Remove reservation with index from calendar

struct calendar* remove_reservation(int index, struct calendar* cal) {

    struct calendar* newcal = malloc(sizeof(struct calendar));
    newcal->size = 0;
    newcal->ptr = malloc((cal->size - 1) * sizeof(struct reservation));
    for (int i = 0, j = 0; i < cal->size; i++) {
        if (i != index) {
            newcal->ptr[j++] = cal->ptr[i];
            newcal->size = newcal->size + 1;
        } else {
            free(cal->ptr[i].description);
        }
    }
    free(cal->ptr);
    free(cal);

    printf("Varaus poistettu onnistuneesti.\n");

    return newcal;
}

// Add an instance of a reservation struct into the calendar (array of structs) if the slot is free.
// Returns NULL if reservation wasn't added, otherwise returns updated calendar.

struct calendar* add_reservation(struct reservation* r, struct calendar* cal, int verbose) {

    // Check whether reservation already exists; If it does, return NULL

    for (int i = 0; i < cal->size; i++) {
        
        struct reservation r2 = *(cal->ptr + i);

        if (r->month == r2.month && r->day == r2.day && r->hour == r2.hour) {

            if (verbose == 1) {
                printf("Haluttuna ajankohtana on jo varaus. Varaa toinen aika.");
            }

            return cal;
        }

    }

    size_t newsize = cal->size + 1;

    // Add room in calendar array for one new reservation.

    struct reservation* newptr = realloc(cal->ptr, sizeof(struct reservation) * newsize);

    if (newptr == NULL) {
        return NULL;
    }

    // Store reservation in newly created empty space and update calendar pointer and size.

    *(newptr + cal->size) = *r;
    free(r);

    cal->size = newsize;
    cal->ptr = newptr;

    if (verbose == 1) {
        printf("Aika varattu onnistuneesti.\n");
    }

    return cal;

}

// Save reservations in a file

void save_reservations(char* filename, struct calendar* cal) {

    FILE* f = fopen(filename, "wb");

    if (f == NULL) {
        return;
    }

    for (int i = 0; i < cal->size; i++) {
        struct reservation* r = cal->ptr + i;
        fwrite(&r->month, sizeof(int), 1, f);
        fwrite(&r->day, sizeof(int), 1, f);
        fwrite(&r->hour, sizeof(int), 1, f);
        fputs(r->description, f);
        fputc('\n', f);
    }
    fclose(f);
}

// Load reservations from a file

struct calendar* load_reservations(char* filename) {

    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        return NULL;
    }

    struct calendar* newcal = malloc(sizeof(struct calendar));
    newcal->ptr = malloc(0);
    newcal->size = 0;

    while (1) {
        void* first = malloc(sizeof(int));
        void* day = malloc(sizeof(int));
        void* hour = malloc(sizeof(int));
        char* desc = malloc(sizeof(char) * 51);
        int firstread = fread(first, sizeof(int), 1, f);
        if (firstread == 0) {
            free(first);
            free(day);
            free(hour);
            free(desc);
            break;
        }
        int* firstint = (int *)first;
        fread(day, sizeof(int), 1, f);
        int* dayint = (int *)day;
        fread(hour, sizeof(int), 1, f);
        int* hourint = (int *)hour;
        fgets(desc, 80, f);
        desc[strcspn(desc, "\n")] = 0;

        struct reservation* r = create_reservation(*firstint, *dayint, *hourint, desc);

        newcal = add_reservation(r, newcal, 0);

        free(first);
        free(day);
        free(hour);
        free(desc);
    }

    fclose(f);
    printf("Tiedosto avattu onnistuneesti\n");

    return newcal;

}

// Print all reservations in format "DSC DAY.MONTH. klo HOUR".
// Inefficient.

void print_reservations(struct calendar* cal) {

    for (int m = 1; m < 13; m++) {

        for (int d = 1; d < 32; d++) {

            for (int h = 1; h < 25; h++) {

                print_reservation(m, d, h, cal);

            }

        }

    }

}

// Print a reservation with the given parameters if it exists in the calendar.

void print_reservation(int month, int day, int hour, struct calendar* cal) {

    for (int i = 0; i < cal->size; i++) {

        struct reservation r = *(cal->ptr + i);
        if (r.month == month && r.day == day && r.hour == hour) {

            printf("%s %d.%d klo %d\n", r.description, r.day, r.month, r.hour);

        }

    }

}

int main() {

    struct calendar* cal = malloc(sizeof(struct calendar));
    cal->ptr = malloc(0);
    cal->size = 0;

    printf("Tervetuloa ajanvaraukseen.\n");

    while (1) {
        printf("\n");
        printf("Syötä komento tai syötä H saadaksesi apua.\n");

        char buf[200];

        fgets(buf, 200, stdin);

        char* command = malloc(sizeof(char));
        char* s1 = malloc(80 * sizeof(char));
        char* s2 = malloc(80 * sizeof(char));
        char* s3 = malloc(80 * sizeof(char));
        char* s4 = malloc(80 * sizeof(char));

        int read = sscanf(buf, "%c %s %s %s %s\n", command, s1, s2, s3, s4);

        /* printf("DEBUGGING\n");
        printf("command: %c\n", *command);
        printf("s1: %s\n", s1);
        printf("s2: %s\n", s2);
        printf("s3: %s\n", s3);
        printf("s4: %s\n", s4);
        printf("read: %d\n", read);
        printf("END DEBUGGING\n"); */

        if (read == 0) {
            printf("Komennon syöttäminen epäonnistui. Yritä uudelleen.\n");
            break;
        }

        switch (*command)
        {
        case 'A':
            if (read == 5) {
                int month = atoi(s2);
                int day = atoi(s3);
                int hour = atoi(s4);
                if (month > 0 && month < 13 && day > 0 && day < 32 && hour > 0 && hour < 24) {
                    struct reservation* r = create_reservation(atoi(s2), atoi(s3), atoi(s4), s1);
                    cal = add_reservation(r, cal, 1);
                } else {
                    printf("Osa tai kaikki aika-arvot eivät ole valideja.\n");
                }
            } else {
                printf("Komento vaatii tasan 4 parametria.\n");
            }
            break;

        case 'D':
            if (read == 4) {
                cal = del_reservation(atoi(s1), atoi(s2), atoi(s3), cal);
            } else {
                printf("Komento vaatii tasan kolme parametria.\n");
            }
            break;

        case 'L':
            print_reservations(cal);
            break;

        case 'W':
            if (read == 2) {
                save_reservations(s1, cal);
            } else {
                printf("Komento vaatii tasan yhden parametrin.\n");
            }
            break;

        case 'O':
            if (read == 2) {
                free(cal->ptr);
                free(cal);
                cal = load_reservations(s1);
            } else {
                printf("Komento vaatii tasan yhden parametrin.\n");
            }
            break;

        case 'H':
            printf("Kirjoita komentoa vastaava kirjain ja sen jälkeen komentoon liittyvät parametrit seuraavasti:\n");
            printf("A kuvaus kuukausi päivä tunti - Lisää merkintä kalenteriin\n");
            printf("D kuukausi päivä tunti - Poista merkintä kalenterista\n");
            printf("L - Tulosta kalenteri\n");
            printf("W tiedostonimi - Tallenna kalenteri\n");
            printf("O tiedostonimi - Lataa kalenteri\n");
            printf("Q - Poistu ohjelmasta\n");
            printf("H - Tulosta tietoa kommennoista\n");
            break;

        case 'Q':
            printf("Suljetaan...\n");
            for (int i = 0; i < cal->size; i++) {
                free(cal->ptr[i].description);
            }
            free(cal->ptr);
            free(cal);
            free(command);
            free(s1);
            free(s2);
            free(s3);
            free(s4);
            return 0;
        
        default:
            printf("Komento ei ole validi. Yritä uudelleen.");
            break;
        }

        free(command);
        free(s1);
        free(s2);
        free(s3);
        free(s4);
    }

}