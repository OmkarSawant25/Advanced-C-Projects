Address Book Management System

-----------------------------------------------------------------------------------------------------------
🚀 Quick Start & Features
-----------------------------------------------------------------------------------------------------------

The Address Book System is a robust, menu-driven application built to handle contact information effectively. Its core features include:

-> Create Contact: Add new contacts with validation to ensure unique phone numbers and email addresses.

-> Search Contact: Find contacts quickly by searching for a name, phone number, or email. The search functionality supports both exact and partial matches.

-> Edit & Delete: Safely modify or remove existing contact entries.

-> List All Contacts: Display all stored contacts in a clean, tabular format. This feature also allows you to sort contacts by name, number, or email for better organization.

-> Persistent Storage: The system automatically loads contacts from a contacts.csv file on startup and can save all current contacts to the same file, ensuring your data is always safe.

-----------------------------------------------------------------------------------------------------------
🛠️ How to Compile and Run
-----------------------------------------------------------------------------------------------------------

To get the program running, follow these simple steps:

-> Open a Terminal: Navigate to the project directory containing the source files.

-> Compile: Use a C compiler like GCC to compile all source files into a single executable.

Bash

gcc main.c contact.c searchSortEdit.c file.c -o addressbook
Run: Execute the compiled program from the terminal.

Bash

./addressbook
The program will launch and present you with the main menu, guiding you through all available options.

-----------------------------------------------------------------------------------------------------------
📂 Project Structure
-----------------------------------------------------------------------------------------------------------

The project code is organized into separate files to enhance modularity and readability:

-> main.c: Contains the main program loop and user interface.

-> contact.c & contact.h: Handles the core logic for managing contacts, including the data structures and validation functions.

-> searchSortEdit.c: Manages the implementation of all search, sort, edit, and delete functionalities.

-> file.c & file.h: Contains all functions related to file handling for saving and loading data.

-> contacts.csv: The data file where all contact information is stored.