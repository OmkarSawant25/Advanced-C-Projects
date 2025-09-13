#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct
{
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct
{
    int contactCount;
    Contact contacts[100];
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *addressBook);

int validateName(char *a);
int my_isalpha(char a);
int validateNumber(char *a);
int my_num(char a);
int validateEmail(char *a);
int dotcom(char *a);
int dotin(char *a);
int dist(char *a);

void searchName(AddressBook *addressBook);
void searchNumber(AddressBook *addressBook);
void searchEmail(AddressBook *addressBook);
void printContact(Contact contact);

void sortName(AddressBook *addressBook);
void sortNumber(AddressBook *addressBook);
void sortEmail(AddressBook *addressBook);
void display(AddressBook *addressBook);

void editName(AddressBook *addressBook);
void editNumber(AddressBook *addressBook);
void editEmail(AddressBook *addressBook);

void delete(AddressBook *addressBook);

#endif
