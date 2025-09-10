#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "contact.h"
#include "file.h"

void searchName(AddressBook *addressBook)
{
    char searchName[50];
    bool nameFlag = 0, found = 0;
    printf("Enter the username to search : ");
    getchar();
    scanf("%[^\n]", searchName);
    nameFlag = validateName(searchName);

    if (nameFlag)
    {
        printf("----------------------------------------------------\n");
        printf("Searched Contacts\n");
        printf("----------------------------------------------------\n");

        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcasecmp(searchName, addressBook->contacts[i].name) == 0 || (strcasestr(addressBook->contacts[i].name, searchName)) != NULL)
            {
                printContact(addressBook->contacts[i]);
                found = 1;
            }
        }
        if (!found)
        {
            printf("User Name Not Found!\n");
            printf("----------------------------------------------------\n");
        }
    }
    else
    {
        printf("Invalid ! Please Enter a Valid username\n");
    }
}
void searchNumber(AddressBook *addressBook)
{
    char searchNumber[50];
    bool numFlag = 0, foundNumber = 0;
    printf("Enter the Phone Number to search : ");
    getchar();
    scanf("%[^\n]", searchNumber);
    numFlag = validateNumber(searchNumber);

    if (numFlag)
    {
        printf("----------------------------------------------------\n");
        printf("Searched Contacts\n");
        printf("----------------------------------------------------\n");
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if ((strcasecmp(searchNumber, addressBook->contacts[i].phone)) == 0)
            {
                printContact(addressBook->contacts[i]);
                foundNumber = 1;
            }
        }
        if (!foundNumber)
        {
            printf("Phone Number Not Found!\n");
            printf("----------------------------------------------------\n");
        }
    }
    else
    {
        printf("Invalid ! Please Enter a Valid Phone Number\n");
    }
}

void searchEmail(AddressBook *addressBook)
{
    char searchEmail[50];
    bool emailFlag = 0, foundEmail = 0;
    printf("Enter the Email Id to search : ");
    getchar();
    scanf("%[^\n]", searchEmail);
    emailFlag = validateEmail(searchEmail);

    if (emailFlag)
    {
        printf("----------------------------------------------------\n");
        printf("Searched Contacts\n");
        printf("----------------------------------------------------\n");
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if ((strcasecmp(searchEmail, addressBook->contacts[i].email)) == 0)
            {
                printContact(addressBook->contacts[i]);
                foundEmail = 1;
            }
        }
        if (!foundEmail)
        {
            printf("Email Id Not Found!\n");
            printf("----------------------------------------------------\n"); 
        }
    }
    else
    {
        printf("Invalid ! Please Enter a Valid Email Id\n");
    }
}

void printContact(Contact contact)
{
    printf(" Name\t\t: %s \n", contact.name);
    printf(" Phone Number\t: %s \n", contact.phone);
    printf(" Email Id\t: %s \n", contact.email);
    printf("----------------------------------------------------\n");

}

void sortName(AddressBook *addressBook)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            if (strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name) > 0)
            {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
}

void sortNumber(AddressBook *addressBook)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            if (strcmp(addressBook->contacts[j].phone, addressBook->contacts[j + 1].phone) > 0)
            {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
}

void sortEmail(AddressBook *addressBook)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            if (strcmp(addressBook->contacts[j].email, addressBook->contacts[j + 1].email) > 0)
            {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
}

void display(AddressBook *addressBook)
{
    int size = addressBook->contactCount;
    if(!size) 
    {
        printf("------------------------------------------------\n");
        printf("   There are NO CONTACTS in the Address Book.   \n");
        printf("------------------------------------------------");
        return;
    }
    printf("----------------------------------------------------\n");
    printf("Contacts Found\n");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < size; i++)
    {
        printf("Contact %d\n", i + 1);
        printf("Name\t\t: %s\n", addressBook->contacts[i].name);
        printf("Phone Number\t: %s\n", addressBook->contacts[i].phone);
        printf("Email\t\t: %s\n", addressBook->contacts[i].email);
        printf("----------------------------------------------------\n");

    }
}