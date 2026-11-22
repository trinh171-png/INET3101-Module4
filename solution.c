
#include <stdio.h>
#include <string.h>

#define FLIGHTS 4
#define SEATS 128

struct Seat {
    int seatNumber;
    int isAssigned;
    char lastName[50];
    char firstName[50];
};
void saveReservations(struct Seat seats[FLIGHTS][SEATS]) {
    FILE *file = fopen("reservations.dat", "wb");

    if (file == NULL) {
        printf("Error saving reservations!\n");
        return;
    }

    if (fwrite(seats, sizeof(struct Seat),
               FLIGHTS * SEATS, file) != FLIGHTS * SEATS) {
        printf("Error writing reservations!\n");
    } else {
        printf("Reservations saved successfully!\n");
    }

    fclose(file);
}

void loadReservations(struct Seat seats[FLIGHTS][SEATS]) {
    FILE *file = fopen("reservations.dat", "rb");

    if (file == NULL) {
        printf("No saved reservations found.\n");
        return;
    }

    struct Seat saved[FLIGHTS][SEATS];

    if (fread(saved, sizeof(struct Seat),
              FLIGHTS * SEATS, file) == FLIGHTS * SEATS) {
        memcpy(seats, saved, sizeof(saved));
        printf("Reservations loaded successfully!\n");
    } else {
        printf("Error loading reservations!\n");
    }

    fclose(file);
}

int main(void) {

    int flightNumbers[FLIGHTS] = {101, 102, 201, 202};
    struct Seat seats[FLIGHTS][SEATS];

    char choice, flightChoice, seatChoice;
    int flightNumber, flightIndex;
    int seatNumber, emptySeats;
    char lastName[50], firstName[50];

    // Initialize all seats
    for (int i = 0; i < FLIGHTS; i++) {
        for (int j = 0; j < SEATS; j++) {
            seats[i][j].seatNumber = j + 1;
            seats[i][j].isAssigned = 0;
            seats[i][j].lastName[0] = '\0';
            seats[i][j].firstName[0] = '\0';
        }
    } 
    loadReservations(seats);


    // Main Menu
    do {
        printf("\n===== COLOSSUS AIRLINES =====\n");
        printf("a. Outbound Flight\n");
        printf("b. Inbound Flight\n");
        printf("c. Quit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        if (choice == 'a' || choice == 'b') {

            // Second Menu
            do {
                printf("\n===== FLIGHT MENU =====\n");
                printf("a. Flight Number\n");
                printf("b. Back to Main\n");
                printf("Enter your choice: ");
                scanf(" %c", &flightChoice);

                if (flightChoice == 'a') {

                    if (choice == 'a') {
                        printf("Outbound flights: 101, 102\n");
                    } else {
                        printf("Inbound flights: 201, 202\n");
                    }

                    printf("Enter flight number: ");
                    if (scanf("%d", &flightNumber) != 1) {
                        printf("Invalid input!\n");
                        return 0;
                    }

                    flightIndex = -1;

                    // Find selected flight
                    for (int i = 0; i < FLIGHTS; i++) {
                        if (flightNumbers[i] == flightNumber) {
                            if ((choice == 'a' && i < 2) ||
                                (choice == 'b' && i >= 2)) {
                                flightIndex = i;
                            }
                        }
                    }

                    if (flightIndex == -1) {
                        printf("Invalid flight number!\n");
                    } else {

                        // Third Menu
                        do {
                            printf("\n===== SEAT MANAGEMENT =====\n");
                            printf("Flight: %d\n", flightNumber);
                            printf("a. Count empty seats\n");
                            printf("b. List empty seats\n");
                            printf("c. Alphabetical passenger list\n");
                            printf("d. Assign a seat\n");
                            printf("e. Delete a seat assignment\n");
                            printf("f. Return to Main Menu\n");
                            printf("Enter your choice: ");
                            scanf(" %c", &seatChoice);

                            switch (seatChoice) {

                                case 'a':
                                    emptySeats = 0;

                                    for (int i = 0; i < SEATS; i++) {
                                        if (seats[flightIndex][i].isAssigned == 0) {
                                            emptySeats++;
                                        }
                                    }

                                    printf("Empty seats: %d\n", emptySeats);
                                    break;

                                case 'b':
                                    printf("\nEmpty seats:\n");

                                    for (int i = 0; i < SEATS; i++) {
                                        if (seats[flightIndex][i].isAssigned == 0) {
                                            printf("%d ", seats[flightIndex][i].seatNumber);
                                        }
                                    }

                                    printf("\n");
                                    break;

                                case 'c': {
                                    int order[SEATS];
                                    int count = 0;

                                    for (int i = 0; i < SEATS; i++) {
                                        if (seats[flightIndex][i].isAssigned == 1) {
                                            order[count] = i;
                                            count++;
                                        }
                                    }

                                    // Sort by last name
                                    for (int i = 0; i < count - 1; i++) {
                                        for (int j = i + 1; j < count; j++) {
                                            if (strcmp(
                                                seats[flightIndex][order[i]].lastName,
                                                seats[flightIndex][order[j]].lastName
                                            ) > 0) {
                                                int temp = order[i];
                                                order[i] = order[j];
                                                order[j] = temp;
                                            }
                                        }
                                    }

                                    if (count == 0) {
                                        printf("No passengers assigned.\n");
                                    } else {
                                        printf("\nSeat  Last Name  First Name\n");

                                        for (int i = 0; i < count; i++) {
                                            int index = order[i];

                                            printf("%d  %s  %s\n",
                                                seats[flightIndex][index].seatNumber,
                                                seats[flightIndex][index].lastName,
                                                seats[flightIndex][index].firstName);
                                        }
                                    }
                                    break;
                                }

                                case 'd':
                                    printf("Enter seat number (1-128, 0 to cancel): ");
                                    if (scanf("%d", &seatNumber) != 1) {
                                        printf("Invalid input!\n");
                                        return 0;
                                    }

                                    if (seatNumber == 0) {
                                        printf("Assignment canceled.\n");
                                    } else if (seatNumber < 1 || seatNumber > SEATS) {
                                        printf("Invalid seat number!\n");
                                    } else if (seats[flightIndex][seatNumber - 1].isAssigned == 1) {
                                        printf("Seat already assigned!\n");
                                    } else {
                                        printf("Enter last name (0 to cancel): ");
                                        scanf("%49s", lastName);

                                        if (strcmp(lastName, "0") == 0) {
                                            printf("Assignment canceled.\n");
                                            break;
                                        }

                                        printf("Enter first name (0 to cancel): ");
                                        scanf("%49s", firstName);

                                        if (strcmp(firstName, "0") == 0) {
                                            printf("Assignment canceled.\n");
                                            break;
                                        }

                                        strcpy(seats[flightIndex][seatNumber - 1].lastName, lastName);
                                        strcpy(seats[flightIndex][seatNumber - 1].firstName, firstName);

                                        seats[flightIndex][seatNumber - 1].isAssigned = 1;

                                        printf("Seat %d assigned successfully!\n", seatNumber);
                                        saveReservations(seats);
                                    }
                                    break;

                                case 'e':
                                    printf("Enter seat number to delete (0 to cancel): ");
                                    if (scanf("%d", &seatNumber) != 1) {
                                        printf("Invalid input!\n");
                                        return 0;
                                    }

                                    if (seatNumber == 0) {
                                        printf("Deletion canceled.\n");
                                    } else if (seatNumber < 1 || seatNumber > SEATS) {
                                        printf("Invalid seat number!\n");
                                    } else if (seats[flightIndex][seatNumber - 1].isAssigned == 0) {
                                        printf("Seat is already empty!\n");
                                    } else {
                                        seats[flightIndex][seatNumber - 1].isAssigned = 0;
                                        seats[flightIndex][seatNumber - 1].lastName[0] = '\0';
                                        seats[flightIndex][seatNumber - 1].firstName[0] = '\0';

                                        printf("Seat assignment deleted!\n");
                                        saveReservations(seats);
                                    }
                                    break;

                                case 'f':
                                    printf("Returning to Main Menu...\n");
                                    break;

                                default:
                                    printf("Invalid choice!\n");
                            }

                        } while (seatChoice != 'f');

                        // Leave Second Menu too
                        flightChoice = 'b';
                    }

                } else if (flightChoice == 'b') {
                    printf("Returning to Main Menu...\n");
                } else {
                    printf("Invalid choice!\n");
                }

            } while (flightChoice != 'b');

        } else if (choice == 'c') {
            printf("Goodbye!\n");
        } else {
            printf("Invalid choice!\n");
        }

    } while (choice != 'c');

    return 0;
}
