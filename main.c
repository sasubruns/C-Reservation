#include "main.h"
#include <stdio.h>
#include <stdlib.h>

// Create reservation struct instance from parameters

struct reservation* create_reservation(int month, int day, int hour, char* description) {

    struct reservation r;
    r.month = month;
    r.day = day;
    r.hour = hour;
    r.description = description;

    return &r;

}

// Delete reservation with parameters

struct calendar* del_reservation(int month, int day, int hour, struct calendar* cal) {

    for (int i = 0; i < cal->size; i++) {

        struct reservation r = *(cal->ptr + i);
        
        if (r.month == month && r.day == day && r.hour == hour) {

            return remove_reservation(i, cal);
            break;

        }

    }

    return cal;

}

// Remove reservation with index i from calendar

struct calendar* remove_reservation(int i, struct calendar* cal) {

    memmove(cal->ptr + i, cal->ptr + i + 1, cal->size - i - 1);
    cal->ptr = realloc(cal->ptr, cal->size - 1);
    cal->size = cal->size -1;
    return cal;

}

// Add an instance of a reservation struct into the calendar (array of structs) if the slot is free.
// Returns NULL if reservation wasn't added, otherwise returns updated calendar.

struct calendar* add_reservation(struct reservation* r, struct calendar* cal) {

    // Check whether reservation already exists; If it does, return NULL

    for (int i = 0; i < cal->size; i++) {
        
        struct reservation r2 = *(cal->ptr + i);

        if (r->month == r2.month && r->day == r2.day && r->hour == r2.hour) {
            return NULL;
        }

    }

    size_t newsize = cal->size + 1;

    // Add room in calendar array for one new reservation.
    // Since this happens frequently, performance could be improved by doubling the
    // size of the array whenever new space is needed, and checking when adding new reservations
    // whether the array is filled. (TODO?)

    struct reservation* newptr = realloc(cal->ptr, sizeof(struct reservation) * newsize);

    if (newptr == NULL) {
        return NULL;
    }

    // Store reservation in newly created empty space and update calendar pointer and size.

    *(newptr + cal->size) = *r;

    cal->size = newsize;
    cal->ptr = newptr;

    return cal;

}

// Save reservations in a file

void save_reservations(char* filename, struct calendar* cal) {

    FILE* f = fopen(filename, "wb");

    if (f == NULL) {
        return NULL;
    }

    for (int i = 0; i < cal->size; i++) {
        struct reservation* r = cal->ptr + i;
        fwrite(&r->month, sizeof(int), 1, f);
        fwrite(&r->day, sizeof(int), 1, f);
        fwrite(&r->hour, sizeof(int), 1, f);
        fputs(r->description, f);
        int eol = 200;
        fwrite(eol, sizeof(int), 1, f);
    }
    fclose(f);
}

// Load reservations from a file

struct calendar* load_reservations(char* filename) {

    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        return NULL;
    }

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

            printf("%s %d.%d klo %d", r.description, r.day, r.month, r.hour);

        }

    }

}

int main() {

    struct calendar* cal;

    printf("Tervetuloa ajanvaraukseen.\n");

    while (1) {
        printf("Syötä komento tai syötä H saadaksesi apua.\n");

        char command;
        char* desc;
        int n1;
        int n2;
        int n3;

        int read = scanf("%c %s %d %d %d", command, desc, n1, n2, n3);
        if (read == 0) {
            printf("Komennon syöttäminen epäonnistui. Yritä uudelleen.\n");
            break;
        }

        switch (command)
        {
        case 'A':
            break;

        case 'D':
            break;

        case 'L':
            break;

        case 'W':
            break;

        case 'O':
            break;

        case 'H':
            printf("Kirjoita komentoa vastaava kirjain ja sen jälkeen komentoon liittyvät parametrit seuraavasti:");
            printf("A kuvaus kuukausi päivä tunti - Lisää merkintä kalenteriin");
            printf("D kuukausi päivä tunti - Poista merkintä kalenterista");
            printf("L - Tulosta kalenteri");
            printf("W tiedostonimi - Tallenna kalenteri");
            printf("O tiedostonimi - Lataa kalenteri");
            printf("Q - Poistu ohjelmasta");
            printf("H - Tulosta tietoa kommennoista");
            break;

        case 'Q':
            printf("Suljetaan...");
            free(cal);
            break;
        
        default:
            printf("Komento ei ole validi. Yritä uudelleen.");
            break;
        }
    }

}