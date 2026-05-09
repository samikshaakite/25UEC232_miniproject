// Advanced Bank Account Management System using Random Access Files
// Features Added:
// 1. Search account
// 2. Deposit and Withdraw options
// 3. View all accounts
// 4. Better validation
// 5. Account existence checking
// 6. Cleaner menu
// 7. Error-free implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[20];
    char firstName[20];
    double balance;
};

// function prototypes
void initializeFile();
unsigned int enterChoice();

void addRecord(FILE *fPtr);
void updateRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchRecord(FILE *fPtr);
void createTextFile(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // initialize file if not present
    initializeFile();

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            addRecord(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            deleteRecord(cfPtr);
            break;

        case 4:
            displayAll(cfPtr);
            break;

        case 5:
            searchRecord(cfPtr);
            break;

        case 6:
            createTextFile(cfPtr);
            break;

        default:
            printf("Invalid Choice!\n");
        }
    }

    fclose(cfPtr);

    printf("\nProgram Ended Successfully.\n");

    return 0;
}

// initialize file with empty records
void initializeFile()
{
    FILE *fPtr;

    struct clientData blankClient = {0, "", "", 0.0};

    fPtr = fopen("credit.dat", "rb");

    // if file does not exist, create it
    if (fPtr == NULL)
    {
        fPtr = fopen("credit.dat", "wb");

        if (fPtr == NULL)
        {
            printf("Unable to create file.\n");
            exit(1);
        }

        for (int i = 0; i < SIZE; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        }
    }

    fclose(fPtr);
}

// menu
unsigned int enterChoice()
{
    unsigned int choice;

    printf("\n========== BANK MANAGEMENT SYSTEM ==========\n");
    printf("1. Add New Account\n");
    printf("2. Update Account Balance\n");
    printf("3. Delete Account\n");
    printf("4. Display All Accounts\n");
    printf("5. Search Account\n");
    printf("6. Create Text File\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}

// add new record
void addRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter Account Number (1 - 100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid Account Number!\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists!\n");
    }
    else
    {
        printf("Enter Last Name: ");
        scanf("%19s", client.lastName);

        printf("Enter First Name: ");
        scanf("%19s", client.firstName);

        printf("Enter Balance: ");
        scanf("%lf", &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account Added Successfully.\n");
    }
}

// update account
void updateRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;
    double amount;

    printf("Enter Account Number to Update: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        printf("\nAccount Found:\n");
        printf("Account No : %u\n", client.acctNum);
        printf("Name       : %s %s\n", client.firstName, client.lastName);
        printf("Balance    : %.2lf\n", client.balance);

        printf("\nEnter Amount (+Deposit / -Withdraw): ");
        scanf("%lf", &amount);

        if ((client.balance + amount) < 0)
        {
            printf("Insufficient Balance!\n");
            return;
        }

        client.balance += amount;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Balance Updated Successfully.\n");
        printf("New Balance: %.2lf\n", client.balance);
    }
}

// delete account
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter Account Number to Delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

        printf("Account Deleted Successfully.\n");
    }
}

// display all accounts
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n====================================================\n");
    printf("%-10s %-15s %-15s %-10s\n",
           "Acc No", "First Name", "Last Name", "Balance");
    printf("====================================================\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10u %-15s %-15s %-10.2lf\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance);
        }
    }
}

// search account
void searchRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;

    printf("Enter Account Number to Search: ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\n========== ACCOUNT DETAILS ==========\n");
        printf("Account Number : %u\n", client.acctNum);
        printf("First Name     : %s\n", client.firstName);
        printf("Last Name      : %s\n", client.lastName);
        printf("Balance        : %.2lf\n", client.balance);
    }
}

// create text file
void createTextFile(FILE *fPtr)
{
    FILE *writePtr;

    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("Unable to create text file.\n");
        return;
    }

    rewind(fPtr);

    fprintf(writePtr,
            "%-10s %-15s %-15s %-10s\n",
            "Acc No",
            "First Name",
            "Last Name",
            "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-10u %-15s %-15s %-10.2lf\n",
                    client.acctNum,
                    client.firstName,
                    client.lastName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt file created successfully.\n");
}