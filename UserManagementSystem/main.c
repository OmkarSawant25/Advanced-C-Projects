#include <stdio.h>

int main()
{
    int option;
    do
    {
        printf("Welcome to User Management System\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Select an option : ");
        scanf("%d", &option);
        
        switch(option)
        {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }while(option == 3);

	return 0;
}
