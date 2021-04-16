struct reservation {
    int month;
    int day;
    int hour;
    char* description;
};

struct calendar {
    struct reservation* ptr;
    size_t size;
};

struct reservation* create_reservation(int month, int day, int hour, char* description);

struct calendar* del_reservation(int month, int day, int hour, struct calendar* cal);

struct calendar* remove_reservation(int i, struct calendar* cal);

struct calendar* add_reservation(struct reservation* r, struct calendar* cal);

void save_reservations(char* filename, struct calendar* cal);

struct calendar* load_reservations(char* filename);

void print_reservations();