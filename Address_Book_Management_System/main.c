/*
===============================================================================
Project Title : Address Book Management System
Name          : Omkar Ashok Sawant
Batch Id      : 25021C_309
Date          : 17/09/2025
Language      : C Programming
===============================================================================

Project Overview:
    The Address Book Management System is a menu-driven C program that allows
    users to manage personal and professional contacts. Each contact consists
    of a Name, Phone Number, and Email Address. Contacts are stored in memory
    and can be saved to a file for future use.

Features:
    1. Create Contact:
       - Add a new contact with validation to ensure unique phone and email.

    2. Search Contact:
       - Search contacts by Name, Phone Number, or Email.
       - Supports both exact and partial matches.

    3. Edit Contact:
       - Modify existing contact details safely with duplicate checks.

    4. Delete Contact:
       - Remove a selected contact from the address book.

    5. List All Contacts:
       - Display all saved contacts in a structured, tabular format.
       - Supports optional sorting for organization.

    6. Save & Load Contacts:
       - Save contacts to a file (CSV format) for persistence.
       - Automatically load contacts on program startup if the file exists.

    7. Exit:
       - Safely close the program after saving data.

File Structure:
    - main.c            : Contains the main menu and program control loop.
    - contact.c/h       : Functions for creating & validating contacts.
    - searchsortEdit.c  : Functions for searching, editing, deleting and sorting contacts.
    - file.c/h          : Handles saving and loading contacts from a file.

Extensibility:
    - Additional fields (e.g., Address, Birthday) can be included easily.
    - Can be extended to support groups, import/export, or a GUI interface.

Suitable For:
    - Students practicing C programming, data handling, and file I/O.
    - Users who require a basic, command-line contact management tool.

===============================================================================
*/

#include <stdio.h>   // standard I/O functions
#include "contact.h" // header file for contact management functions
#include <stdio_ext.h>
int main()
{
    int choice;              // stores user menu choice
    AddressBook addressBook; // address book structure

    initialize(&addressBook); // initialize the address book (empty or load from file)

    do
    {
        // display the menu
        printf("----------------------------------------------------\n");
        printf("Address Book Menu:\n");
        printf("----------------------------------------------------\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Save contacts\n");
        printf("7. Exit\n");
        printf("\nEnter your choice (1 - 7): ");
        // scanf("%d", &choice); // read user choice
        if (scanf("%d", &choice) != 1)
        {
            printf("❌ Invalid input! Please enter a number.\n\n");
            while (getchar() != '\n');     // clear input buffer
            continue; // go back to menu
        }

        // perform operation based on user choice
        switch (choice)
        {
        case 1:
            createContact(&addressBook); // add new contact
            break;
        case 2:
            searchContact(&addressBook); // search existing contacts
            break;
        case 3:
            editContact(&addressBook); // edit selected contact
            break;
        case 4:
            deleteContact(&addressBook); // delete selected contact
            break;
        case 5:
            listContacts(&addressBook); // display all contacts
            break;
        case 6:
            printf("Saving...\n\n");
            saveContactsToFile(&addressBook); // save contacts to file
            break;
        case 7:
            printf("---------------------------------------------------------\n");
            printf("Thank you for using the Address Book Management System!\n");
            printf("You had %d contacts saved.\n", addressBook.contactCount);
            printf("Goodbye!\n");
            printf("---------------------------------------------------------\n");
            return 0; // exit the program
        default:
            printf("❌ Invalid choice ! Please try again & Enter choice between 1 - 7\n\n"); // invalid input
        }
    } while (choice != '7'); // continue until user chooses Exit

    return 0;
}
