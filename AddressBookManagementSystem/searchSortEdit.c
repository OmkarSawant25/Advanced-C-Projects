#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "contact.h"
#include "file.h"

int arrayIndex[MAX_CONTACTS]; // Stores indexes of matched contacts during a search
int matchCount;               // Number of contacts matched in the last search

void searchName(AddressBook *addressBook)
{
    char searchName[50];
    matchCount = 0; // Reset match count before searching
    bool nameFlag = 0, found = 0;

    printf("Enter the username to search : ");
    // getchar();                   // Clear input buffer
    scanf(" %49[^\n]", searchName); // Read full input including spaces from the user

    nameFlag = validateName(searchName); // Validate entered name

    if (nameFlag)
    {
        searchStart();
        for (int i = 0; i < addressBook->contactCount; i++) // Loop through all contacts
        {
            // Check if name matches fully or partially
            if (strcasecmp(searchName, addressBook->contacts[i].name) == 0 || (strcasestr(addressBook->contacts[i].name, searchName)) != NULL)
            {
                arrayIndex[matchCount++] = i; // Save matched contact index in the array
                // printf("Serial no : %d \n", matchCount);
                printContact(addressBook->contacts[i], matchCount); // Display matched contact
                found = 1;                                          // Mark as found
            }
        }

        if (!found) // No matches found
        {
            printf("\t\t\tUser Name Not Found!\n");
        }
        printf("+------------------------------------------------------------------------------------------------+\n\n");
    }
    else
    {
        printf("Invalid ! Please Enter a Valid username\n\n");
    }
}

void searchNumber(AddressBook *addressBook)
{
    char searchNumber[50];
    matchCount = 0; // Reset match count
    bool numFlag = 0, foundNumber = 0;

    printf("Enter the Phone Number to search : ");
    // getchar();
    scanf(" %[^\n]", searchNumber);

    numFlag = validateNumber(searchNumber); // Validate number format

    if (numFlag)
    {
        searchStart();
        for (int i = 0; i < addressBook->contactCount; i++) // Check each contact
        {
            // Exact match OR partial match
            if (strcasecmp(searchNumber, addressBook->contacts[i].phone) == 0 || strstr(addressBook->contacts[i].phone, searchNumber) != NULL)
            // if ((strcasecmp(searchNumber, addressBook->contacts[i].phone)) == 0) // Exact match
            {
                arrayIndex[matchCount++] = i;                       // Save index
                printContact(addressBook->contacts[i], matchCount); // Display matched contact
                foundNumber = 1;
            }
        }

        if (!foundNumber) // No match found
        {
            printf("\t\t\tPhone Number Not Found!\n\n");
        }
        printf("+------------------------------------------------------------------------------------------------+\n\n");
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

    emailFlag = validateEmail(searchEmail); // Validate email format

    if (emailFlag)
    {
        searchStart();
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if ((strcasecmp(searchEmail, addressBook->contacts[i].email)) == 0) // Exact email match
            {
                arrayIndex[matchCount++] = i;
                printContact(addressBook->contacts[i], matchCount); // Display matched contact
                foundEmail = 1;
            }
        }

        if (!foundEmail) // No email found
        {
            printf("\t\t\tEmail Id Not Found!\n\n");
        }
        printf("+------------------------------------------------------------------------------------------------+\n\n");
    }
    else
    {
        printf("Invalid ! Please Enter a Valid Email Id\n");
    }
}
void searchStart()
{
    printf("+------------------------------------------------------------------------------------------------+\n");
    printf("\t\t\t\tSearched Contacts\n");
    printf("+------------------------------------------------------------------------------------------------+\n");
    printf("| %-5s   %-30s   %-15s   %-35s |\n", "Sr.No", "Name", "Phone", "Email");
    printf("+------------------------------------------------------------------------------------------------+\n");
}
void printContact(Contact contact, int match)
{
    printf("| %-5d   %-30s   %-15s   %-35s |\n", match, contact.name, contact.phone, contact.email);
}

void sortName(AddressBook *addressBook)
{
    // Bubble sort contacts by name (case-insensitive)
    for (int i = 0; i < addressBook->contactCount - 1; i++)
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
    // Bubble sort contacts by phone number
    for (int i = 0; i < addressBook->contactCount - 1; i++)
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
    // Bubble sort contacts by email
    for (int i = 0; i < addressBook->contactCount - 1; i++)
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
    if (!size) // If no contacts are stored
    {
        printf("------------------------------------------------\n");
        printf("   There are NO CONTACTS in the Address Book.   \n");
        printf("------------------------------------------------\n");
        return;
    }
    printf("+------------------------------------------------------------------------------------------------+\n");
    printf("\t\t\t\t   Contacts Found\n");
    printf("+------------------------------------------------------------------------------------------------+\n");

    // printf("+-------------------------------------------------------------------------------+\n");
    printf("| %-5s   %-30s   %-15s   %-35s |\n", "Sr.No", "Name", "Phone", "Email");

    printf("+------------------------------------------------------------------------------------------------+\n");
    for (int i = 0; i < size; i++) // Display all contacts
    {
        printf("| %-5d   %-30s   %-15s   %-35s |\n", i + 1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    printf("+------------------------------------------------------------------------------------------------+\n\n");
}

void editName(AddressBook *addressBook)
{
    if (matchCount <= 0) // Ensure a search was done before editing
    {
        printf("No previous search results found! Please search first.\n");
        return;
    }

    int index;
    char newName[50];

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount) // Validate choice
    {
        printf("Invalid serial number!\n");
        return;
    }
    getchar();
    printf("Enter the new name : ");
    scanf("%[^\n]", newName);

    if (validateName(newName)) // Validate new name
    {
        strcpy(addressBook->contacts[arrayIndex[index - 1]].name, newName);
        printf("\nName changed successfully ! \n\n");
    }
    else
    {
        printf("Invalid input ! Please Enter a Valid username\n");
    }
}

void editNumber(AddressBook *addressBook)
{
    if (matchCount <= 0) // Ensure a search was done before editing
    {
        printf("No previous search results found! Please search first.\n");
        return;
    }

    int index;
    char newPhone[20];

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount) // Validate choice
    {
        printf("Invalid serial number!\n");
        return;
    }
    // getchar();
    printf("Enter the new Phone Number : ");
    scanf(" %[^\n]", newPhone);

    if (validateNumber(newPhone)) // Validate new phone number
    {
        // Duplicate check
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (i != arrayIndex[index - 1] && strcmp(addressBook->contacts[i].phone, newPhone) == 0)
            {
                printf("Phone Number Already Exists!\n");
                return;
            }
        }
        strcpy(addressBook->contacts[arrayIndex[index - 1]].phone, newPhone);
        printf("\nPhone Number changed successfully ! \n\n");
    }
    else
    {
        printf("Invalid input ! Please Enter a Valid Number\n");
    }
}

void editEmail(AddressBook *addressBook)
{
    if (matchCount <= 0) // Ensure a search was done before editing
    {
        printf("No previous search results found! Please search first.\n");
        return;
    }

    int index;
    char newEmail[50];

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount) // Validate choice
    {
        printf("Invalid serial number!\n");
        return;
    }
    getchar();
    printf("Enter the new Email Id : ");
    scanf("%[^\n]", newEmail);

    if (validateEmail(newEmail)) // Validate new email
    {
        // Duplicate check
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (i != arrayIndex[index - 1] && strcmp(addressBook->contacts[i].email, newEmail) == 0)
            {
                printf("Email Already Exists!\n");
                return;
            }
        }
        strcpy(addressBook->contacts[arrayIndex[index - 1]].email, newEmail);
        printf("\nEmail Id changed successfully ! \n\n");
    }
    else
    {
        printf("❌ Invalid input ! Please Enter a Valid Email Id\n");
    }
}

void delete(AddressBook *addressBook)
{
    int index;
    if (matchCount <= 0) // Ensure a search was done before deleting
    {
        printf("❌ No previous search results found! Please search again.\n");
        return;
    }

    printf("Enter the serial no (1 - %d): ", matchCount);
    scanf("%d", &index);

    if (index < 1 || index > matchCount) // Validate choice
    {
        printf("❌ Invalid serial number!\n");
        return;
    }

    // Shift all contacts one step left to overwrite the deleted one
    for (int i = arrayIndex[index - 1]; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--; // Decrease total count
    printf("\nContact deleted successfully !\n\n");
}
