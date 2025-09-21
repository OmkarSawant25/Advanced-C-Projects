/*
===============================================================================
Project Title : Car Rental Management System
Language      : C Programming
===============================================================================

Overview:
    This program allows customers to rent cars, return them, and view records.
    Admins can add, delete, or view cars. Data is stored in files for persistence.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// --------------------------- Main Function ---------------------------
int main() {
    loadData();
    mainMenu();
    saveData();
    return 0;
}

// --------------------------- Function Definitions ---------------------------

void mainMenu() {
    int choice;
    do {
        printf("\n====== Car Rental System ======\n");
        printf("1. Admin Menu\n");
        printf("2. Customer Menu\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: adminMenu(); break;
            case 2: customerMenu(); break;
            case 3: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 3);
}

void adminMenu() {
    int choice;
    do {
        printf("\n====== Admin Menu ======\n");
        printf("1. Add Car\n");
        printf("2. List Cars\n");
        printf("3. Delete Car\n");
        printf("4. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addCar(); break;
            case 2: listCars(); break;
            case 3: deleteCar(); break;
            case 4: return;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

void customerMenu() {
    int choice;
    do {
        printf("\n====== Customer Menu ======\n");
        printf("1. Rent Car\n");
        printf("2. Return Car\n");
        printf("3. List Rentals\n");
        printf("4. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: rentCar(); break;
            case 2: returnCar(); break;
            case 3: listRentals(); break;
            case 4: return;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

// --------------------------- Placeholders ---------------------------
void addCar()      { printf("[Functionality: Add Car]\n"); }
void listCars()    { printf("[Functionality: List Cars]\n"); }
void deleteCar()   { printf("[Functionality: Delete Car]\n"); }
void rentCar()     { printf("[Functionality: Rent Car]\n"); }
void returnCar()   { printf("[Functionality: Return Car]\n"); }
void listRentals() { printf("[Functionality: List Rentals]\n"); }

void loadData()    { printf("[Loading data from files]\n"); }
void saveData()    { printf("[Saving data to files]\n"); }
