#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "contact.h"
#include "file.h"

void listContacts(AddressBook *addressBook)
{
    int choice;
    printf("----------------------------------------------------\n");
    printf("List & Sort contact\n");
    printf("----------------------------------------------------\n");

    do
    {
        printf("1. Sort by name\n");
        printf("2. Sort by number\n");
        printf("3. Sort by email\n");
        printf("4. Exit to main\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            sortName(addressBook);
            display(addressBook);
            break;
        case 2:
            sortNumber(addressBook);
            display(addressBook);
            break;
        case 3:
            sortEmail(addressBook);
            display(addressBook);
            break;
        case 4:
            break;
        default:
            choice = 4;
            break;
        }
    } while (choice != 4);
}

void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;

    // Load contacts from file during initialization (After files)
    // loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS);              // Exit the program
}

void createContact(AddressBook *addressBook)
{
    /* Define the logic to create a Contacts */
    int nameflag = 0;
    int mobilenumberflag = 0;
    int emailflag = 0;
    printf("----------------------------------------------------\n");
    printf("Create contact\n");
    printf("----------------------------------------------------\n");

    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("Address Book is Full! Can't Enter more contacts\n");
        return;
    }
    do
    {
        char username[50];
        printf("Enter the username : ");
        getchar();
        scanf("%[^\n]", username);
        nameflag = validateName(username);          // Validating the username
        if (nameflag)
        {
            strcpy(addressBook->contacts[addressBook->contactCount].name, username);
        }
        else
        {
            printf("Invalid input ! Please Enter a Valid username\n");
        }
    } while (!nameflag);

    do
    {
        char userPhone[20];
        bool duplicate = false;
        printf("Enter the Phone Number : ");
        scanf("%s", userPhone);
        mobilenumberflag = validateNumber(userPhone) && (strlen(userPhone) == 10);
        if (mobilenumberflag)
        {
            for (int i = 0; i < addressBook->contactCount; i++)         // Finding the Duplicates
            {
                if (strcmp(addressBook->contacts[i].phone, userPhone) == 0)
                {
                    printf("Phone Number Already Exists !\n");
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate)
            {
                strcpy(addressBook->contacts[addressBook->contactCount].phone, userPhone);
                mobilenumberflag = 1;
            }
            else
            {
                mobilenumberflag = 0;
            }
        }
        else
        {
            printf("Invalid ! Please Enter a Valid Phone Number \n");
        }
    } while (!mobilenumberflag);

    do
    {
        char userEmail[50];
        bool duplicate = false;
        printf("Enter the Email Id : ");
        scanf("%s", userEmail);
        emailflag = validateEmail(userEmail);
        if (emailflag)
        {
            for (int i = 0; i < addressBook->contactCount; i++)                 // Finding the Duplicates
            {
                if (strcmp(addressBook->contacts[i].email, userEmail) == 0)
                {
                    printf("Email Already Exists !\n");
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate)
            {
                strcpy(addressBook->contacts[addressBook->contactCount].email, userEmail);
                emailflag = 1;
            }
            else
            {
                emailflag = 0;
            }
        }
        else
        {
            printf("Invalid ! Please Enter a Valid Email Id\n");
        }
    } while (!emailflag);

    addressBook->contactCount++;
    printf("\nContact Created successfully ! Address Book Updated\n\n");

}

void searchContact(AddressBook *addressBook)
{
    /* Define the logic for search */
    int choice;
    
    printf("----------------------------------------------------\n");
    printf("Search contact\n");
    printf("----------------------------------------------------\n");

    do
    {
        printf("1. Search by name\n");
        printf("2. Search by number\n");
        printf("3. Search by email\n");
        printf("4. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            searchName(addressBook);
            break;
        case 2:
            searchNumber(addressBook);
            break;
        case 3:
            searchEmail(addressBook);
            break;
        case 4:
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            choice = 4;
        }
    } while (choice != 4);
}

void editContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */
    searchContact(addressBook);

    printf("\n----------------------------------------------------\n");
    printf("Edit contact\n");
    printf("----------------------------------------------------\n");

    int choice;
 
    do
    {
        printf("1. Edit name\n");
        printf("2. Edit number\n");
        printf("3. Edit email\n");
        printf("4. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
        case 1:
            editName(addressBook);
            // printf("Enter the new name : ");
            // scanf("%s", newName);
            // if (validateName(newName))
            // {
            //     printf("Enter the serial no : ");
            //     scanf("%d", &index);
            //     strcpy(addressBook->contacts[index].name, newName);
            //     printf("Name changed successfully ! Address Book Updated\n\n");
            // }
            // else
            // {
            //     printf("Invalid input ! Please Enter a Valid username\n");
            // }
            break;
        case 2:
            editNumber(addressBook);
            break;
        case 3:
            editEmail(addressBook);
            break;
        case 4:
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            choice = 4;
        }
    } while (choice != 4);

}

void deleteContact(AddressBook *addressBook)
{
    /* Define the logic for deletecontact */
    searchContact(addressBook);

    printf("\n----------------------------------------------------\n");
    printf("Delete contact\n");
    printf("----------------------------------------------------\n");

    delete(addressBook);

}

int validateName(char *a)
{
    while (*a != '\0')
    {
        if (my_isalpha(*a) == 0 && *a != ' ')
        {
            return 0;
        }
        a++;
    }
    return 1;
}

int validateNumber(char *a)
{
    while (*a != '\0')
    {
        if (my_num(*a) == 0)
        {
            return 0;
        }
        a++;
    }
    return 1;
}

int my_isalpha(char a)
{
    if ((a >= 65 && a <= 90) || (a >= 97 && a <= 122))  
    {
        return 1;
    }
    return 0;
}

int my_num(char a)
{
    if (a >= 48 && a <= 57)
    {
        return 1;
    }
    return 0;
}

int validateEmail(char *a)
{
    int at = 0;
    if (a[0] == '@')
        return 0;
    if (!(dotcom(a) && dist(a)))
        return 0;
    // if (!dist(a))
    //     return 0;
    while (*a != '\0')
    {
        if (*a == ' ')
            return 0;
        else if (*a == '@')
            at++;
        else if (!((*a >= 'a' && *a <= 'z') || *a == '.' || *a >= '0' && *a <= '9'))
            return 0;

        a++;
    }
    if (at == 1)
        return 1;
    else
        return 0;
}

int dotcom(char *a)
{
    int j = strlen(a) - 1;
    if (a[j] == 'm' && a[j - 1] == 'o' && a[j - 2] == 'c' && a[j - 3] == '.')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int dist(char *a)
{
    int atpos = 0;
    int i = 0;
    int j = strlen(a) - 1;
    for (int i = 0; i < j; i++)
    {
        if (a[i] == '@')
        {
            atpos = i;
            break;
        }
    }
    if (atpos < j - 4)
        return 1;
    return 0;
}