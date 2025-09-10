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
        nameflag = validateName(username);
        if (nameflag)
        {
            strcpy(addressBook->contacts[addressBook->contactCount].name, username);
        }
        else
        {
            printf("Invalid ! Please Enter a Valid username\n");
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
            for (int i = 0; i < addressBook->contactCount; i++)
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
            for (int i = 0; i < addressBook->contactCount; i++)
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
        }
    } while (choice != 4);
}

void editContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */
}

void deleteContact(AddressBook *addressBook)
{
    /* Define the logic for deletecontact */
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

// void searchName(AddressBook *addressBook)
// {
//     char searchName[50];
//     bool nameFlag = 0, found = 0;
//     printf("Enter the username to search : ");
//     getchar();
//     scanf("%[^\n]", searchName);
//     nameFlag = validateName(searchName);

//     if (nameFlag)
//     {
//         printf("----------------------------------------------------\n");
//         printf("Searched Contacts\n");
//         printf("----------------------------------------------------\n");

//         for (int i = 0; i < addressBook->contactCount; i++)
//         {
//             // if ((strcasecmp(searchName, addressBook->contacts[i].name)) == 0)
//             // {
//             //     // printf("Name\t\t\t: %s\n", addressBook->contacts[i].name);
//             //     // printf("Phone number\t\t: %s\n", addressBook->contacts[i].phone);
//             //     // printf("Email Id\t\t: %s\n", addressBook->contacts[i].email);
//             //     // printf("--------------------------------------------------\n");
//             //     printContact(addressBook->contacts[i]);
//             //     found = 1;
//             // }
//             if (strcasecmp(searchName, addressBook->contacts[i].name) == 0 || (strcasestr(addressBook->contacts[i].name, searchName)) != NULL)
//             {
//                 printContact(addressBook->contacts[i]);
//                 found = 1;
//             }
//         }
//         if (!found)
//         {
//             printf("Username not found! Please enter an Existing username\n");
//         }
//     }
//     else
//     {
//         printf("Invalid ! Please Enter a Valid username\n");
//     }
// }
// void searchNumber(AddressBook *addressBook)
// {
//     char searchNumber[50];
//     bool numFlag = 0, foundNumber = 0;
//     printf("Enter the Phone Number to search : ");
//     getchar();
//     scanf("%[^\n]", searchNumber);
//     numFlag = validateNumber(searchNumber);

//     if (numFlag)
//     {
//         for (int i = 0; i < addressBook->contactCount; i++)
//         {
//             if ((strcasecmp(searchNumber, addressBook->contacts[i].phone)) == 0)
//             {
//                 printContact(addressBook->contacts[i]);
//                 foundNumber = 1;
//             }
//         }
//         if (!foundNumber)
//         {
//             printf("Phone Number not found! Please enter an Existing Number\n");
//         }
//     }
//     else
//     {
//         printf("Invalid ! Please Enter a Valid Phone Number\n");
//     }
// }

// void searchEmail(AddressBook *addressBook)
// {
//     char searchEmail[50];
//     bool emailFlag = 0, foundEmail = 0;
//     printf("Enter the Email Id to search : ");
//     getchar();
//     scanf("%[^\n]", searchEmail);
//     emailFlag = validateEmail(searchEmail);

//     if (emailFlag)
//     {
//         for (int i = 0; i < addressBook->contactCount; i++)
//         {
//             if ((strcasecmp(searchEmail, addressBook->contacts[i].email)) == 0)
//             {
//                 // printf("Name\t\t\t: %s\n", addressBook->contacts[i].name);
//                 // printf("Phone number\t\t: %s\n", addressBook->contacts[i].phone);
//                 // printf("Email Id\t\t: %s\n", addressBook->contacts[i].email);
//                 printContact(addressBook->contacts[i]);
//                 foundEmail = 1;
//             }
//         }
//         if (!foundEmail)
//         {
//             printf("Email Id not found! Please enter an Existing Email Id\n");
//         }
//     }
//     else
//     {
//         printf("Invalid ! Please Enter a Valid Email Id\n");
//     }
// }

// void printContact(Contact contact)
// {
//     // printf("----------------------------------------------------\n");
//     printf(" Name\t\t: %s \n", contact.name);
//     printf(" Phone Number\t: %s \n", contact.phone);
//     printf(" Email Id\t: %s \n", contact.email);
//     printf("----------------------------------------------------\n");

// }

// void sortName(AddressBook *addressBook)
// {
//     for (int i = 0; i < addressBook->contactCount; i++)
//     {
//         for (int j = 0; j < addressBook->contactCount - i - 1; j++)
//         {
//             if (strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name) > 0)
//             {
//                 Contact temp = addressBook->contacts[j];
//                 addressBook->contacts[j] = addressBook->contacts[j + 1];
//                 addressBook->contacts[j + 1] = temp;
//             }
//         }
//     }
// }

// void sortNumber(AddressBook *addressBook)
// {
//     for (int i = 0; i < addressBook->contactCount; i++)
//     {
//         for (int j = 0; j < addressBook->contactCount - i - 1; j++)
//         {
//             if (strcmp(addressBook->contacts[j].phone, addressBook->contacts[j + 1].phone) > 0)
//             {
//                 Contact temp = addressBook->contacts[j];
//                 addressBook->contacts[j] = addressBook->contacts[j + 1];
//                 addressBook->contacts[j + 1] = temp;
//             }
//         }
//     }
// }

// void sortEmail(AddressBook *addressBook)
// {
//     for (int i = 0; i < addressBook->contactCount; i++)
//     {
//         for (int j = 0; j < addressBook->contactCount - i - 1; j++)
//         {
//             if (strcmp(addressBook->contacts[j].email, addressBook->contacts[j + 1].email) > 0)
//             {
//                 Contact temp = addressBook->contacts[j];
//                 addressBook->contacts[j] = addressBook->contacts[j + 1];
//                 addressBook->contacts[j + 1] = temp;
//             }
//         }
//     }
// }

// void display(AddressBook *addressBook)
// {
//     // printf("\n----- Address Book Information -----\n\n");
//     int size = addressBook->contactCount;
//     if(!size) 
//     {
//         printf("------------------------------------------------\n");
//         printf("   There are NO CONTACTS in the Address Book.   \n");
//         printf("------------------------------------------------");
//         return;
//     }
//     printf("----------------------------------------------------\n");
//     printf("Contacts Found\n");
//     printf("----------------------------------------------------\n");
//     for (int i = 0; i < size; i++)
//     {
//         // printf("Information of Person %d\n", i + 1);
//         printf("Name\t\t: %s\n", addressBook->contacts[i].name);
//         printf("Phone Number\t: %s\n", addressBook->contacts[i].phone);
//         printf("Email\t\t: %s\n", addressBook->contacts[i].email);
//         printf("----------------------------------------------------\n");

//     }
// }