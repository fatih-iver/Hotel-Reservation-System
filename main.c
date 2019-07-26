#include <stdio.h>
#include <stdlib.h>

#define RESERVE 1
#define CANCEL 2
#define DISPLAY 3
#define EXIT 4

#define RESERVED 1

const int NUMBER_OF_ROOMS = 10;

struct room {
    int room_number;
    int is_reserved;
    char name[20];
    char surname[20];
    float price;
};

void initialize_apart(){

    FILE *db_rooms = fopen("rooms.apt", "wb");

    int room_number;

    for(room_number = 1; room_number <= NUMBER_OF_ROOMS; room_number++){
        struct room empty_room = {room_number, 0, "empty", "room", -1};
        fwrite(&empty_room, sizeof(struct room), 1, db_rooms);
    }

    fclose(db_rooms);
}

void display_choices() {
    printf("****************************\n");
    printf("1 - Reserve a room\n");
    printf("2 - Cancel a reservation\n");
    printf("3 - Display all empty rooms\n");
    printf("4 - Exit\n");
    printf("****************************\n");
}

void do_reservation() {
    FILE *db_rooms = fopen("rooms.apt", "rb+");

    int room_number;
    struct room current_room;

    while(1) {

        printf("Please enter a room number to make reservation: ");
        scanf("%d",&room_number);

        if( room_number < 1 || room_number > NUMBER_OF_ROOMS) {
            printf("Please enter a room number between 1 - %d\n", NUMBER_OF_ROOMS);
            continue;
        }

        int offset = (room_number - 1) * sizeof(struct room);
        fseek(db_rooms, offset, SEEK_SET);
        fread(&current_room, sizeof(struct room), 1, db_rooms);

        if(current_room.is_reserved != RESERVED) {
            fseek(db_rooms, offset, SEEK_SET);

            current_room.is_reserved = RESERVED;
            printf("Name: ");
            scanf("%s", &current_room.name);
            printf("Surname: ");
            scanf("%s", &current_room.surname);
            printf("Price: ");
            scanf("%f", &current_room.price);

            fwrite(&current_room, sizeof(struct room), 1, db_rooms);
            printf("Room is reserved successfully!\n");
            break;
        } else {
            printf("Room is already reserved!\n");
        }
    }

    fclose(db_rooms);
}

void cancel_reservation() {

    FILE *db_rooms = fopen("rooms.apt", "rb+");

    int room_number;
    struct room current_room;

    while(1) {

        printf("Please enter the room number to cancel reservation: ");
        scanf("%d",&room_number);

        if( room_number < 1 || room_number > NUMBER_OF_ROOMS) {
            printf("Please enter a room number between 1 - %d\n", NUMBER_OF_ROOMS);
            continue;
        }

        int offset = (room_number - 1) * sizeof(struct room);
        fseek(db_rooms, offset, SEEK_SET);
        fread(&current_room, sizeof(struct room), 1, db_rooms);

        if(current_room.is_reserved == RESERVED) {
            fseek(db_rooms, offset, SEEK_SET);
            struct room empty_room = {room_number, 0, "", "", -1};
            fwrite(&empty_room, sizeof(struct room), 1, db_rooms);
            printf("Reservation is canceled successfully!\n");
            break;
        } else {
            printf("Room is not reserved!\n");
        }
    }

    fclose(db_rooms);
}

void display_empty_rooms() {

    FILE *db_rooms = fopen("rooms.apt", "rb");

    struct room current_room;

    while(fread(&current_room, sizeof(struct room), 1, db_rooms)) {
        if(current_room.is_reserved != RESERVED) {
            printf ("%d ", current_room.room_number);
        }
    }

    printf("\n");

    fclose(db_rooms);
}

int main()
{
    initialize_apart();

    int choice;

    do {

        display_choices();

        printf("Please enter your choice: ");
        scanf("%d",&choice);

        switch(choice) {
            case RESERVE:
                do_reservation();
                break;
            case CANCEL:
                cancel_reservation();
                break;
            case DISPLAY:
                display_empty_rooms();
                break;
        }

   }while( choice !=  EXIT);
    return 0;
}
