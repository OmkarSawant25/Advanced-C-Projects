#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "contact.h"
#include "file.h"

int arrayIndex[100];
int matchCount;

void searchName(AddressBook *addressBook)
{
    char searchName[50];
    matchCount = 0;
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
                arrayIndex[matchCount++] = i;
                printf("Serial no : %d \n", matchCount);
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
    matchCount = 0;
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
                arrayIndex[matchCount++] = i;
                printf("Serial no : %d \n", matchCount);
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
    matchCount = 0;
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
                arrayIndex[matchCount++] = i;
                printf("Serial no : %d \n", matchCount);
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
    if (!size)
    {
        printf("------------------------------------------------\n");
        printf("   There are NO CONTACTS in the Address Book.   \n");
        printf("------------------------------------------------\n");
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

void editName(AddressBook *addressBook)
{
    if (matchCount <= 0)
    {
        printf("No previous search results found! Please search first.\n");
        return;
    }

    int index;
    char newName[50];

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount)
    {
        printf("Invalid serial number!\n");
        return;
    }
    getchar();
    printf("Enter the new name : ");
    scanf("%[^\n]", newName);
    if (validateName(newName))
    {
        strcpy(addressBook->contacts[arrayIndex[index - 1]].name, newName);
        printf("Name changed successfully ! Address Book Updated\n\n");
    }
    else
    {
        printf("Invalid input ! Please Enter a Valid username\n");
    }
}

void editNumber(AddressBook *addressBook)
{
    if (matchCount <= 0)
    {
        printf("No previous search results found! Please search first.\n");
        return;
    }

    int index;
    char newPhone[20];

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount)
    {
        printf("Invalid serial number!\n");
        return;
    }
    getchar();
    printf("Enter the new Phone Number : ");
    scanf("%[^\n]", newPhone);
    if (validateNumber(newPhone))
    {
        strcpy(addressBook->contacts[arrayIndex[index - 1]].phone, newPhone);
        printf("Phone Number changed successfully ! Address Book Updated\n\n");
    }
    else
    {
        printf("Invalid input ! Please Enter a Valid Number\n");
    }
}

void editEmail(AddressBook *addressBook)
{
    if (matchCount <= 0)
    {
        printf("No previous search results found! Please search first.\n");
        return;
    }

    int index;
    char newEmail[20];

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount)
    {
        printf("Invalid serial number!\n");
        return;
    }
    getchar();
    printf("Enter the new Email Id : ");
    scanf("%[^\n]", newEmail);
    if (validateEmail(newEmail))
    {
        strcpy(addressBook->contacts[arrayIndex[index - 1]].email, newEmail);
        printf("Email Id changed successfully ! Address Book Updated\n\n");
    }
    else
    {
        printf("Invalid input ! Please Enter a Valid Email Id\n");
    }
}

void delete(AddressBook *addressBook)
{
    int index;
    if (matchCount <= 0)
    {
        printf("No previous search results found! Please search first.\n");
        return;
    }

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount)
    {
        printf("Invalid serial number!\n");
        return;
    }

    for(int i = arrayIndex[index - 1]; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;
    printf("Contact deleted successfully ! Address Book Updated\n\n");

}