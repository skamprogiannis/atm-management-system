#include "header.h"

static const char *RECORDS = "./data/records.txt";
static const char *USERS = "./data/users.txt";
static const double SAVINGS_INTEREST = 0.07;
static const double FIXED_ONE_YEAR_INTEREST = 0.04;
static const double FIXED_TWO_YEAR_INTEREST = 0.05;
static const double FIXED_THREE_YEAR_INTEREST = 0.08;

int getAccountFromFile(FILE *file, char ownerName[50], struct Record *record) {
  return fscanf(file, "%d %d %49s %d %d/%d/%d %99s %d %lf %9s", &record->id,
                &record->userId, ownerName, &record->accountNumber,
                &record->deposit.month, &record->deposit.day,
                &record->deposit.year, record->country, &record->phone,
                &record->amount, record->accountType) == 11;
}

void saveAccountToFile(FILE *file, int userId, const char *ownerName,
                       struct Record record) {
  fprintf(file, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", record.id, userId,
          ownerName, record.accountNumber, record.deposit.month,
          record.deposit.day, record.deposit.year, record.country, record.phone,
          record.amount, record.accountType);
}

int getUserFromFile(FILE *file, struct User *user) {
  return fscanf(file, "%d %49s %49s", &user->id, user->name, user->password) ==
         3;
}

int findUserByName(const char *name, struct User *user) {
  FILE *usersFile = fopen(USERS, "r");
  if (usersFile == NULL) {
    printf("\nError: Failed to open the users database.\n");
    exit(1);
  }

  while (getUserFromFile(usersFile, user)) {
    if (strcmp(user->name, name) == 0) {
      fclose(usersFile);
      return 1;
    }
  }

  fclose(usersFile);
  return 0;
}

int isUsersAccount(struct User user, const char *ownerName,
                   struct Record record, int accountNumber) {
  return strcmp(ownerName, user.name) == 0 &&
         record.accountNumber == accountNumber;
}

int getNextAccountNumber(struct User owner) {
  FILE *recordsFile = fopen(RECORDS, "r");
  char ownerName[50];
  struct Record record;
  int maxAccountNumber = -1;

  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  while (getAccountFromFile(recordsFile, ownerName, &record)) {
    if (strcmp(ownerName, owner.name) == 0) {
      if (record.accountNumber > maxAccountNumber) {
        maxAccountNumber = record.accountNumber;
      }
    }
  }

  fclose(recordsFile);

  return maxAccountNumber + 1;
}

int isFixedAccount(const char *accountType) {
  return strcmp(accountType, "fixed01") == 0 ||
         strcmp(accountType, "fixed02") == 0 ||
         strcmp(accountType, "fixed03") == 0;
}

void stayOrReturn(int showRetryMenu, void retryFunction(struct User user),
                  struct User user) {
  int option;
  if (showRetryMenu) {
    system("clear");
    printf("\nRecord not found.\n");
    do {
      printf("\nEnter 0 to try again, 1 to return to the main menu, or 2 to "
             "exit: ");
      option = getIntegerInput();
      if (option == 0) {
        retryFunction(user);
        return;
      } else if (option == 1) {
        mainMenu(user);
        return;
      } else if (option == 2) {
        exit(0);
      } else {
        printf("Enter a valid option.\n");
      }
    } while (option != 0 && option != 1 && option != 2);
  } else {
    do {
      printf("\nEnter 1 to go to the main menu or 0 to exit: ");
      option = getIntegerInput();

      if (option != 0 && option != 1) {
        printf("Enter a valid option.\n");
      }
    } while (option != 0 && option != 1);
  }
  if (option == 1) {
    system("clear");
    mainMenu(user);
    return;
  } else {
    system("clear");
    exit(1);
  }
}

void success(struct User user) {
  int option;
  printf("\nSuccess!\n\n");
  do {
    printf("Enter 1 to go to the main menu or 0 to exit: ");
    option = getIntegerInput();
    system("clear");
    if (option == 1) {
      mainMenu(user);
      return;
    } else if (option == 0) {
      exit(1);
    } else {
      printf("Enter a valid option.\n");
    }
  } while (option != 0 && option != 1);
}

int getIntegerInput() {
  int value;
  while (scanf("%d", &value) != 1) {
    while (getchar() != '\n') {
      // Discard invalid input.
    }
    printf("Invalid input. Try again: ");
  }
  return value;
}

void createNewAccount(struct User user) {
  struct Record record;
  struct Record currentRecord;
  char ownerName[50];
  char continueInput;
  int accountExists;

  do {
    accountExists = 0;
    FILE *recordsFile = fopen(RECORDS, "a+");
    if (recordsFile == NULL) {
      printf("Error: Could not open the accounts database.\n");
      return;
    }

    system("clear");
    printf("\t\t\t===== New account =====\n");

    int validDate = 0;
    while (!validDate) {
      printf("\nEnter today's date (mm/dd/yyyy): ");
      if (scanf("%d/%d/%d", &record.deposit.month, &record.deposit.day,
                &record.deposit.year) == 3) {
        if (record.deposit.month >= 1 && record.deposit.month <= 12 &&
            record.deposit.day >= 1 && record.deposit.day <= 31) {
          validDate = 1;
        } else {
          printf("Invalid date. Please try again.\n");
        }
      } else {
        printf("Invalid date format. Use mm/dd/yyyy.\n");
        while (getchar() != '\n') {
          // Discard invalid input.
        }
      }
    }
    printf("\nEnter the account number: ");
    record.accountNumber = getIntegerInput();

    rewind(recordsFile);
    while (getAccountFromFile(recordsFile, ownerName, &currentRecord)) {
      if (isUsersAccount(user, ownerName, currentRecord,
                         record.accountNumber)) {
        printf("This account already exists for this user.\n\n");
        printf("Enter any character to continue: ");
        scanf(" %c", &continueInput);
        accountExists = 1;
        break;
      }
    }

    fclose(recordsFile);

  } while (accountExists);

  printf("\nEnter the country: ");
  scanf("%49s", record.country);
  printf("\nEnter the phone number: ");
  record.phone = getIntegerInput();
  printf("\nEnter the amount to deposit: $");
  scanf("%lf", &record.amount);
  printf(
      "\nChoose an account type:\n\t-> savings\n\t-> current\n\t-> "
      "fixed01 (for 1 year)\n\t-> fixed02 (for 2 years)\n\t-> fixed03 (for 3 "
      "years)\n\n\tEnter your choice: ");
  scanf("%9s", record.accountType);

  FILE *recordsFile = fopen(RECORDS, "a+");
  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  saveAccountToFile(recordsFile, user.id, user.name, record);
  fclose(recordsFile);
  success(user);
}

void checkAllAccounts(struct User user) {
  char ownerName[100];
  struct Record record;

  FILE *recordsFile = fopen(RECORDS, "r");
  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  system("clear");
  printf("\t\t====== Accounts owned by %s =====\n\n", user.name);
  while (getAccountFromFile(recordsFile, ownerName, &record)) {
    if (strcmp(ownerName, user.name) == 0) {
      printf("_____________________\n");
      printf("\nAccount number: %d\n"
             "Deposit date: %d/%d/%d\n"
             "Country: %s\n"
             "Phone number: %d\n"
             "Amount deposited: $%.2f\n"
             "Type of account: %s\n",
             record.accountNumber, record.deposit.day, record.deposit.month,
             record.deposit.year, record.country, record.phone, record.amount,
             record.accountType);
    }
  }
  fclose(recordsFile);
  success(user);
}

double getInterestRate(const char *accountType) {
  if (strcmp(accountType, "saving") == 0 || strcmp(accountType, "savings") == 0)
    return SAVINGS_INTEREST;
  if (strcmp(accountType, "fixed01") == 0)
    return FIXED_ONE_YEAR_INTEREST;
  if (strcmp(accountType, "fixed02") == 0)
    return FIXED_TWO_YEAR_INTEREST;
  if (strcmp(accountType, "fixed03") == 0)
    return FIXED_THREE_YEAR_INTEREST;
  return 0.0;
}

void checkAccountDetails(struct User user) {
  char ownerName[100];
  struct Record record;
  int accountNumber;
  int found = 0;
  double interestRate;
  double monthlyInterest;

  FILE *recordsFile = fopen(RECORDS, "r");
  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  system("clear");
  printf("Enter the account number you want to inspect: ");
  accountNumber = getIntegerInput();
  while (getAccountFromFile(recordsFile, ownerName, &record)) {
    if (isUsersAccount(user, ownerName, record, accountNumber)) {
      found = 1;
      printf("\nAccount number: %d\n"
             "Deposit date: %d/%d/%d\n"
             "Country: %s\n"
             "Phone number: %d\n"
             "Amount deposited: $%.2f\n"
             "Type of account: %s\n",
             record.accountNumber, record.deposit.day, record.deposit.month,
             record.deposit.year, record.country, record.phone, record.amount,
             record.accountType);

      if (strcmp(record.accountType, "current") != 0) {
        interestRate = getInterestRate(record.accountType);
        monthlyInterest = record.amount * interestRate / 12;
        printf("You will get $%.2lf as interest on day %d of every month.\n",
               monthlyInterest, record.deposit.day);
      } else {
        printf(
            "You will not get interest because this is a current account.\n");
      }
      break;
    }
  }

  fclose(recordsFile);

  if (!found) {
    stayOrReturn(1, checkAccountDetails, user);
    return;
  }

  success(user);
}

void updateAccountInformation(struct User user) {
  int accountNumber;
  int option;
  int found = 0;
  struct Record record;
  char ownerName[50];
  const char *tempRecords = "./data/records.tmp";

  system("clear");
  printf("\t\tEnter the account number you want to update, %s:\n\n", user.name);
  accountNumber = getIntegerInput();

  do {
    printf("\nWhich information do you want to update?\n");
    printf("\n[1]- Country");
    printf("\n[2]- Phone number");
    printf("\n\nEnter your choice: ");
    option = getIntegerInput();

    if (option != 1 && option != 2) {
      printf("Invalid option.\n");
    }
  } while (option != 1 && option != 2);

  FILE *recordsFile = fopen(RECORDS, "r");
  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  FILE *updatedRecordsFile = fopen(tempRecords, "w");
  if (updatedRecordsFile == NULL) {
    fclose(recordsFile);
    printf("\nError: Failed to open the temporary accounts database.\n");
    exit(1);
  }

  while (getAccountFromFile(recordsFile, ownerName, &record)) {
    if (isUsersAccount(user, ownerName, record, accountNumber)) {
      found = 1;

      if (option == 1) {
        printf("\nEnter the new country: ");
        scanf("%99s", record.country);
      } else {
        printf("\nEnter the new phone number: ");
        record.phone = getIntegerInput();
      }
    }

    saveAccountToFile(updatedRecordsFile, record.userId, ownerName, record);
  }

  fclose(recordsFile);
  fclose(updatedRecordsFile);

  if (!found) {
    remove(tempRecords);
    stayOrReturn(1, updateAccountInformation, user);
    return;
  }

  if (rename(tempRecords, RECORDS) != 0) {
    printf("\nError: Failed to save the updated accounts database.\n");
    exit(1);
  }

  success(user);
}

void removeAccount(struct User user) {
  int accountNumber;
  int found = 0;
  struct Record record;
  char ownerName[50];
  const char *tempRecords = "./data/records.tmp";

  system("clear");
  printf("\t\tEnter the number of the account you want to delete, %s:\n\n",
         user.name);
  accountNumber = getIntegerInput();

  FILE *recordsFile = fopen(RECORDS, "r");
  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  FILE *updatedRecordsFile = fopen(tempRecords, "w");
  if (updatedRecordsFile == NULL) {
    fclose(recordsFile);
    printf("\nError: Failed to open the temporary accounts database.\n");
    exit(1);
  }

  while (getAccountFromFile(recordsFile, ownerName, &record)) {
    if (isUsersAccount(user, ownerName, record, accountNumber)) {
      found = 1;
    } else {
      saveAccountToFile(updatedRecordsFile, record.userId, ownerName, record);
    }
  }

  fclose(recordsFile);
  fclose(updatedRecordsFile);

  if (!found) {
    remove(tempRecords);
    stayOrReturn(1, removeAccount, user);
    return;
  }

  if (rename(tempRecords, RECORDS) != 0) {
    printf("\nError: Failed to save the updated accounts database.\n");
    exit(1);
  }

  success(user);
}

void makeTransaction(struct User user) {
  int accountNumber;
  char ownerName[50];
  int found = 0;
  int option;
  int transactionCompleted = 0;
  double amount;
  struct Record record;
  const char *tempRecords = "./data/records.tmp";

  FILE *recordsFile = fopen(RECORDS, "r");
  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  FILE *updatedRecordsFile = fopen(tempRecords, "w");
  if (updatedRecordsFile == NULL) {
    fclose(recordsFile);
    printf("\nError: Failed to open the temporary accounts database.\n");
    exit(1);
  }

  system("clear");
  printf("\t\tEnter the account number for the transaction, %s:\n\n",
         user.name);
  accountNumber = getIntegerInput();

  while (getAccountFromFile(recordsFile, ownerName, &record)) {
    if (isUsersAccount(user, ownerName, record, accountNumber)) {
      found = 1;

      if (isFixedAccount(record.accountType)) {
        printf("\nFixed accounts cannot make transactions.\n");
      } else {
        do {
          printf("Would you like to deposit money or withdraw money?");
          printf("\n\t\t[1]- Deposit\n");
          printf("\n\t\t[2]- Withdraw\n");
          printf("\nEnter your choice: ");
          option = getIntegerInput();

          if (option != 1 && option != 2) {
            printf("Invalid option.\n");
          }
        } while (option != 1 && option != 2);

        printf("Enter the amount: ");
        while (scanf("%lf", &amount) != 1) {
          while (getchar() != '\n') {
            // Discard invalid input.
          }
          printf("Invalid input. Try again: ");
        }

        if (amount <= 0) {
          printf("\nTransaction amount must be greater than zero.\n");
        } else if (option == 1) {
          record.amount += amount;
          transactionCompleted = 1;
        } else if (amount > record.amount) {
          printf("\nThe account does not have $%.2lf available to withdraw.\n",
                 amount);
        } else {
          record.amount -= amount;
          transactionCompleted = 1;
        }
      }
    }

    saveAccountToFile(updatedRecordsFile, record.userId, ownerName, record);
  }

  fclose(recordsFile);
  fclose(updatedRecordsFile);

  if (!found) {
    remove(tempRecords);
    stayOrReturn(1, makeTransaction, user);
    return;
  }

  if (!transactionCompleted) {
    remove(tempRecords);
    stayOrReturn(0, makeTransaction, user);
    return;
  }

  if (rename(tempRecords, RECORDS) != 0) {
    printf("\nError: Failed to save the updated accounts database.\n");
    exit(1);
  }

  success(user);
}

void transferAccount(struct User user) {
  int accountNumber;
  char ownerName[50];
  char recipientName[50];
  int found = 0;
  int recipientAccountNumber;
  struct Record record;
  struct User recipient;
  const char *tempRecords = "./data/records.tmp";

  system("clear");
  printf("\t\tEnter the account number you want to transfer, %s:\n\n",
         user.name);
  accountNumber = getIntegerInput();

  printf("Enter the username you want to transfer this account to: ");
  scanf("%49s", recipientName);

  if (strcmp(recipientName, user.name) == 0) {
    printf("\nYou cannot transfer an account to yourself.\n");
    stayOrReturn(0, transferAccount, user);
    return;
  }

  if (!findUserByName(recipientName, &recipient)) {
    printf("\nRecipient user not found.\n");
    stayOrReturn(0, transferAccount, user);
    return;
  }

  recipientAccountNumber = getNextAccountNumber(recipient);

  FILE *recordsFile = fopen(RECORDS, "r");
  if (recordsFile == NULL) {
    printf("\nError: Failed to open the accounts database.\n");
    exit(1);
  }

  FILE *updatedRecordsFile = fopen(tempRecords, "w");
  if (updatedRecordsFile == NULL) {
    fclose(recordsFile);
    printf("\nError: Failed to open the temporary accounts database.\n");
    exit(1);
  }

  while (getAccountFromFile(recordsFile, ownerName, &record)) {
    if (isUsersAccount(user, ownerName, record, accountNumber)) {
      found = 1;
      record.accountNumber = recipientAccountNumber;
      saveAccountToFile(updatedRecordsFile, recipient.id, recipient.name,
                        record);
    } else {
      saveAccountToFile(updatedRecordsFile, record.userId, ownerName, record);
    }
  }

  fclose(recordsFile);
  fclose(updatedRecordsFile);

  if (!found) {
    remove(tempRecords);
    stayOrReturn(1, transferAccount, user);
    return;
  }

  if (rename(tempRecords, RECORDS) != 0) {
    printf("\nError: Failed to save the updated accounts database.\n");
    exit(1);
  }

  success(user);
}
