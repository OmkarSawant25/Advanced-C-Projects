#include <stdio.h>   // Standard input/output functions (printf, scanf, etc.)
#include <stdlib.h>  // Standard library (exit, malloc, free, etc.)
#include <string.h>  // String handling functions (strlen, strcpy, strcmp, etc.)
#include <stdbool.h> // For using bool, true, false
#include "contact.h" // Custom header file for contact structure & functions
#include "file.h"    // Custom header file for file operations (save, load)

// Function to display contacts with sorting options
void listContacts(AddressBook *addressBook)
{
    char choice; // User's menu choice
    printf("----------------------------------------------------\n");
    printf("List & Sort contact\n");
    printf("----------------------------------------------------\n");

    do
    {
        // Display sorting menu
        printf("1. Sort by name\n");
        printf("2. Sort by number\n");
        printf("3. Sort by email\n");
        printf("4. Exit to main\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice); // Get choice from the user
        // while (getchar() != '\n'); // Clear input buffer

        switch (choice)
        {
        case '1':                    // Sort contacts by name
            sortName(addressBook); // Function Call
            display(addressBook);  // Show sorted contacts
            break;
        case '2':                      // Sort contacts by number
            sortNumber(addressBook); // Function Call
            display(addressBook);    // Show sorted contacts
            break;
        case '3':                     // Sort contacts by email
            sortEmail(addressBook); // Function Call
            display(addressBook);   // Show sorted contacts
            break;
        case '4': // Exit menu
            break;
        default :
            printf("Invalid choice ! Try Again \n\n");
        }
    } while (choice != '4'); // Run the loop until user exits
}

// Function to initialize address book
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;     // Set contact count to 0 initially
    loadContactsFromFile(addressBook); // Optional: Load contacts from file
}

// Save contacts and exit program
void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook); // Save all contacts to file
    exit(EXIT_SUCCESS);              // Exit successfully
}

// Function to create a new contact
void createContact(AddressBook *addressBook)
{
    bool nameflag = 0;         // To check if name is valid
    bool mobilenumberflag = 0; // To check if number is valid
    bool emailflag = 0;        // To check if email is valid
    printf("----------------------------------------------------\n");
    printf("Create contact\n");
    printf("----------------------------------------------------\n");

    // Prevent adding if contact list is full
    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("Address Book is Full! Can't Enter more contacts\n");
        return;
    }

    // Input and validate name
    do
    {
        char username[50]; // Temporary storage for name
        printf("Enter the username : ");
        // getchar();                 // Clear buffer (removes newline from previous input)
        scanf(" %[^\n]", username); // Read full string including spaces

        nameflag = validateName(username); // Validate entered name
        if (nameflag)
            strcpy(addressBook->contacts[addressBook->contactCount].name, username); // Save name
        else
            printf("Invalid input ! Please Enter a Valid username\n");
    } while (!nameflag); // Repeat until valid name is entered

    // Input and validate phone number
    do
    {
        char userPhone[20];     // Temporary storage for phone number
        bool duplicate = false; // Check for duplicate number
        printf("Enter the Phone Number : ");
        scanf("%s", userPhone); // Read phone number

        mobilenumberflag = validateNumber(userPhone) && (strlen(userPhone) == 10); // Validate number (only digits, length 10)

        if (mobilenumberflag)
        {
            // Check for duplicates
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
                strcpy(addressBook->contacts[addressBook->contactCount].phone, userPhone); // Save number if no duplicate
            else
                mobilenumberflag = 0; // Reset flag if duplicate found
        }
        else
            printf("Invalid ! Please Enter a Valid Phone Number \n");
    } while (!mobilenumberflag); // Repeat until valid

    // Input and validate email
    do
    {
        char userEmail[50];     // Temporary storage for email
        bool duplicate = false; // Check for duplicate email
        printf("Enter the Email Id : ");
        scanf("%s", userEmail); // Read email

        emailflag = validateEmail(userEmail); // Validate email format
        if (emailflag)
        {
            // Check for duplicates
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
                strcpy(addressBook->contacts[addressBook->contactCount].email, userEmail); // Save email if unique
            else
                emailflag = 0; // Reset flag if duplicate found
        }
        else
            printf("Invalid ! Please Enter a Valid Email Id\n");
    } while (!emailflag); // Repeat until valid

    // Increase contact count after adding new contact
    addressBook->contactCount++;
    printf("\n✅  Contact Created successfully !\n\n");
}

// Function to search for a contact
void searchContact(AddressBook *addressBook)
{
    char choice; // User's choice for search
    printf("----------------------------------------------------\n");
    printf("Search contact\n");
    printf("----------------------------------------------------\n");

    do
    {
        // Display search options
        printf("1. Search by name\n");
        printf("2. Search by number\n");
        printf("3. Search by email\n");
        printf("4. Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice); // Get user input
        // while (getchar() != '\n'); // Clear input buffer

        switch (choice)
        {
        case '1':
            searchName(addressBook);
            break; // Search by name
        case '2':
            searchNumber(addressBook);
            break; // Search by number
        case '3':
            searchEmail(addressBook);
            break; // Search by email
        case '4':
            break; // Exit search
        default :
            printf("Invalid choice ! Try Again \n\n");
        }
    } while (choice != '4'); // Run the Loop until exit
}

// Function to edit a contact
void editContact(AddressBook *addressBook)
{
    searchContact(addressBook); // First search the contact

    printf("\n----------------------------------------------------\n");
    printf("Edit contact\n");
    printf("----------------------------------------------------\n");

    char choice;
    do
    {
        // Display edit options
        printf("1. Edit name\n");
        printf("2. Edit number\n");
        printf("3. Edit email\n");
        printf("4. Exit\n");
        printf("\nEnter your choice: ");
        scanf(" %c", &choice); // Get user input
        // while (getchar() != '\n'); // Clear input buffer

        switch (choice)
        {
        case '1':
            editName(addressBook);
            break; // Edit name
        case '2':
            editNumber(addressBook);
            break; // Edit number
        case '3':
            editEmail(addressBook);
            break; // Edit email
        case '4':
            break; // Exit
        default :
            printf("Invalid choice ! Try Again \n\n");
        }
    } while (choice != '4'); // Run the Loop until exit
}

// Function to delete a contact
void deleteContact(AddressBook *addressBook)
{
    searchContact(addressBook); // First search contact to delete

    printf("\n----------------------------------------------------\n");
    printf("Delete contact\n");
    printf("----------------------------------------------------\n");

    delete(addressBook); // Call delete function
}

// Validate if name contains only alphabets and spaces
int validateName(char *a)
{
    while (*a != '\0') // Traverse through string
    {
        if (my_isalpha(*a) == 0 && *a != ' ')
            return 0; // Invalid if not alphabet or space
        a++;          // Move to next character
    }
    return 1; // Valid name
}

// Validate if number contains only digits
int validateNumber(char *a)
{
    if (a[0] == '0')
        return 0;
    while (*a != '\0')
    {
        if (my_num(*a) == 0)
            return 0; // Invalid if non-digit found
        a++;
    }
    return 1; // Valid number
}

// Check if character is alphabet (A-Z or a-z)
int my_isalpha(char a)
{
    if ((a >= 65 && a <= 90) || (a >= 97 && a <= 122))
        return 1; // Return true if alphabet
    return 0;     // Otherwise false
}

// Check if character is digit (0-9)
int my_num(char a)
{
    if (a >= 48 && a <= 57)
        return 1; // Return true if digit
    return 0;     // Otherwise false
}

// Validate email format
int validateEmail(char *a)
{
    int at = 0; // Count '@' symbols
    int dotsAfterAt = 0;
    if (a[0] == '@')
        return 0; // Invalid if email starts with '@'
    if (!((dotcom(a) && dist(a))||(dotin(a) && distin(a))))
        return 0; // Must end with ".com" and proper '@' distance

    while (*a != '\0')
    {
        if (*a == ' ')
            return 0; // Email can't have spaces
        else if (*a == '@')
            at++; // Count '@'
        else if (*a == '.')
        {
            if (at > 0) // dot after '@'
                dotsAfterAt++;
        }
        else if (!((*a >= 'a' && *a <= 'z') || *a == '.' || *a >= '0' && *a <= '9'))
            return 0; // Invalid character
        a++;
    }
    // return (at == 1); // Valid only if exactly one '@' present
    if (at != 1)
        return 0;

    // reject if more than 1 dot after '@'
    if (dotsAfterAt > 1)
        return 0;

    return 1;
}

// Check if email ends with ".com"
int dotcom(char *a)
{
    int j = strlen(a) - 1; // Last character index
    if (a[j] == 'm' && a[j - 1] == 'o' && a[j - 2] == 'c' && a[j - 3] == '.')
        return 1; // Return true if ends with ".com"
    else
        return 0;
}

// Ensure '@' comes before ".com" with valid distance
int dist(char *a)
{
    int atpos = 0;         // Position of '@'
    int j = strlen(a) - 1; // Last index
    for (int i = 0; i < j; i++)
    {
        if (a[i] == '@') // Find first '@'
        {
            atpos = i;
            break;
        }
    }
    if (atpos < j - 4) // '@' must come before ".com"
        return 1;
    return 0;
}


int dotin(char *a)
{
    int j = strlen(a) - 1; // Last character index
    if (a[j] == 'n' && a[j - 1] == 'i' && a[j - 2] == '.')
        return 1; // Return true if ends with ".com"
    else
        return 0;
}

int distin(char *a)
{
    int atpos = 0;         // Position of '@'
    int j = strlen(a) - 1; // Last index
    for (int i = 0; i < j; i++)
    {
        if (a[i] == '@') // Find first '@'
        {
            atpos = i;
            break;
        }
    }
    if (atpos < j - 3) // '@' must come before ".in"
        return 1;
    return 0;
}