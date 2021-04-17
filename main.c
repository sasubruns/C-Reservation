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