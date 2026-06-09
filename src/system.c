#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
  return fscanf(ptr, "%d %d %49s %d %d/%d/%d %99s %d %lf %9s", &r->id,
                &r->userId, name, &r->accountNbr, &r->deposit.month,
                &r->deposit.day, &r->deposit.year, r->country, &r->phone,
                &r->amount, r->accountType) == 11;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
  fprintf(ptr, "%d %d %49s %d %d/%d/%d %99s %d %.2lf %9s\n\n", r.id, u.id,
          u.name, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year,
          r.country, r.phone, r.amount, r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
  int option;
  if (notGood == 0) {
    system("clear");
    printf("\n✖ Record not found!!\n");
  invalid:
    printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
    option = getIntInput();
    if (option == 0)
      f(u);
    else if (option == 1)
      mainMenu(u);
    else if (option == 2)
      exit(0);
    else {
      printf("Insert a valid operation!\n");
      goto invalid;
    }
  } else {
    printf("\nEnter 1 to go to the main menu and 0 to exit:");
    option = getIntInput();
  }
  if (option == 1) {
    system("clear");
    mainMenu(u);
  } else {
    system("clear");
    exit(1);
  }
}

void success(struct User u) {
  int option;
  printf("\n✔ Success!\n\n");
invalid:
  printf("Enter 1 to go to the main menu and 0 to exit!\n");
  option = getIntInput();
  system("clear");
  if (option == 1) {
    mainMenu(u);
  } else if (option == 0) {
    exit(1);
  } else {
    printf("Insert a valid operation!\n");
    goto invalid;
  }
}

int getIntInput() {
  int value;
  while (scanf("%d", &value) != 1) {
    while (getchar() != '\n') {
      // Clear buffer
    }
    printf("Invalid input. Try again: ");
  }
  return value;
}

void createNewAcc(struct User u) {
  struct Record r;
  struct Record cr;
  char userName[50];
  int accountExists;

  do {
    accountExists = 0;
    FILE *pf = fopen(RECORDS, "a+");
    if (pf == NULL) {
      printf("Error: Could not open database.\n");
      return;
    }

    system("clear");
    printf("\t\t\t===== New record =====\n");

    int validDate = 0;
    while (!validDate) {
      printf("\nEnter today's date(mm/dd/yyyy): ");
      if (scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day,
                &r.deposit.year) == 3) {
        if (r.deposit.month >= 1 && r.deposit.month <= 12 &&
            r.deposit.day >= 1 && r.deposit.day <= 31) {
          validDate = 1;
        } else {
          printf("Invalid date values! Please try again.\n");
        }
      } else {
        printf("Invalid format! Use mm/dd/yyyy.\n");
        while (getchar() != '\n')
          ; // Clear the "trash" (like letters or dashes)
      }
    }
    printf("\nEnter the account number:");
    r.accountNbr = getIntInput();

    // Check for duplicates
    rewind(pf);
    while (getAccountFromFile(pf, userName, &cr)) {
      if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
        printf("✖ This Account already exists for this user\n\n");
        scanf(" %c", &myChar); // Stop so the user sees the message
        accountExists = 1;
        break;
      }
    }

    fclose(pf);

  } while (accountExists); // if accountExists == 1, restart the loop

  printf("\nEnter the country:");
  scanf("%49s", r.country); // %49s to prevent buffer overflows
  printf("\nEnter the phone number:");
  r.phone = getIntInput();
  printf("\nEnter amount to deposit: $");
  scanf("%lf", &r.amount);
  printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> "
         "fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 "
         "years)\n\n\tEnter your choice:");
  scanf("%s", r.accountType);

  FILE *pf_final = fopen(RECORDS, "a+");
  saveAccountToFile(pf_final, u, r);
  fclose(pf_final);
  success(u);
}

void checkAllAccounts(struct User u) {
  char userName[100];
  struct Record r;

  FILE *pf = fopen(RECORDS, "r");

  system("clear");
  printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
  while (getAccountFromFile(pf, userName, &r)) {
    if (strcmp(userName, u.name) == 0) {
      printf("_____________________\n");
      printf("\nAccount number:%d\n"
             "Deposit Date:%d/%d/%d \n"
             "country:%s \n"
             "Phone number:%d \n"
             "Amount deposited: $%.2f \n"
             "Type Of Account:%s\n",
             r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
             r.country, r.phone, r.amount, r.accountType);
    }
  }
  fclose(pf);
  success(u);
}
