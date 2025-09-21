#ifndef CONTACT_H // prevent multiple inclusion of this header file
#define CONTACT_H

#define MAX_CONTACTS 100 // maximum number of contacts allowed

// structure to store details of a single contact
typedef struct
{
    char name[50];  // contact name
    char phone[20]; // contact phone number
    char email[50]; // contact email address
} Contact;

// structure to store all contacts in an address book
typedef struct
{
    int contactCount;               // total number of contacts currently saved
    Contact contacts[MAX_CONTACTS]; // array of contacts
} AddressBook;

// function declarations

void createContact(AddressBook *addressBook);      // add a new contact
void searchContact(AddressBook *addressBook);      // search a contact (by name/phone/email)
void editContact(AddressBook *addressBook);        // edit an existing contact
void deleteContact(AddressBook *addressBook);      // delete a contact
void listContacts(AddressBook *addressBook);       // list all contacts
void initialize(AddressBook *addressBook);         // initialize the address book
void saveContactsToFile(AddressBook *addressBook); // save contacts to file

// validation functions
int validateName(char *a);   // validate name string
int my_isalpha(char a);      // check if character is alphabet
int validateNumber(char *a); // validate phone number
int my_num(char a);          // check if character is digit
int validateEmail(char *a);  // validate email string
int dotcom(char *a);         // check for ".com" in email
int dist(char *a);           // check email formatting rules for .com
int dotin(char *a);          // check for ".in" in email
int distin(char *a);         // check email formatting rules for .in

// search functions
void searchName(AddressBook *addressBook);     // search contact by name
void searchNumber(AddressBook *addressBook);   // search contact by phone
void searchEmail(AddressBook *addressBook);    // search contact by email
void searchStart(void);                        // start search menu
void printContact(Contact contact, int match); // print a single contact

// sorting functions
void sortName(AddressBook *addressBook);   // sort contacts by name
void sortNumber(AddressBook *addressBook); // sort contacts by phone
void sortEmail(AddressBook *addressBook);  // sort contacts by email
void display(AddressBook *addressBook);    // display all contacts

// editing functions
void editName(AddressBook *addressBook);   // edit name of a contact
void editNumber(AddressBook *addressBook); // edit phone number of a contact
void editEmail(AddressBook *addressBook);  // edit email of a contact

// delete function
void delete(AddressBook *addressBook); // delete a contact

#endif // end of CONTACT_H