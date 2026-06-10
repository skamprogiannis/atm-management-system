#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Date {
  int month, day, year;
};

struct Record {
  int id;
  int userId;
  char name[100];
  char country[100];
  int phone;
  char accountType[10];
  int accountNumber;
  double amount;
  struct Date deposit;
  struct Date withdraw;
};

struct User {
  int id;
  char name[50];
  char password[50];
};

void loginMenu(char name[50], char password[50]);
void registerMenu(char name[50], char password[50]);
int authenticateUser(struct User user);
int isUniqueUsername(struct User user);

int getIntegerInput();
void createNewAccount(struct User user);
void mainMenu(struct User user);
void checkAllAccounts(struct User user);
void updateAccountInformation(struct User user);
void checkAccountDetails(struct User user);
void makeTransaction(struct User user);
void removeAccount(struct User user);
void transferAccount(struct User user);
