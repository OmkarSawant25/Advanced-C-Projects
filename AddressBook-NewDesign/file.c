#include <stdio.h> // standard I/O functions (printf, scanf, FILE operations)
#include "file.h"  // header file for file handling functions (save/load contacts)

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr;
    fptr = fopen("contacts.csv", "w+"); // open file for writing (overwrite if exists)
    if (fptr == NULL)                   // check if file opened successfully
        return;

    fprintf(fptr, "%d\n", addressBook->contactCount); // write number of contacts
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        // write each contact in CSV format: Name,Phone,Email
        fprintf(fptr, "%s,%s,%s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    fclose(fptr); // close file
}

void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fptr;
    fptr = fopen("contacts.csv", "r+"); // open file for reading
    if (fptr == NULL)                   // if file not found, start empty
    {
        addressBook->contactCount = 0;
        return;
    }
    fscanf(fptr, "%d", &addressBook->contactCount); // read number of contacts
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        // read each contact's Name, Phone, Email
        fscanf(fptr, " %[^,],%[^,],%[^\n]", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    fclose(fptr); // close file
}
