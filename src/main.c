#include "header.h"

void mainMenu(struct User user) {
  int option;
  system("clear");
  printf("\n=== ATM ===\n\n");
  printf("Choose one of the options below:\n\n");
  printf("[1]- Create a new account\n");
  printf("[2]- Update account information\n");
  printf("[3]- Check account details\n");
  printf("[4]- List owned accounts\n");
  printf("[5]- Make a transaction\n");
  printf("[6]- Remove existing account\n");
  printf("[7]- Transfer ownership\n");
  printf("[8]- Exit\n");
  option = getIntegerInput();

  switch (option) {
  case 1:
    createNewAccount(user);
    return;
  case 2:
    updateAccountInformation(user);
    return;
  case 3:
    checkAccountDetails(user);
    return;
  case 4:
    checkAllAccounts(user);
    return;
  case 5:
    makeTransaction(user);
    return;
  case 6:
    removeAccount(user);
    return;
  case 7:
    transferAccount(user);
    return;
  case 8:
    exit(0);
  default:
    printf("Invalid option.\n");
  }
}

void initMenu(struct User *user) {
  int authenticated = 0;
  system("clear");
  printf("\n=== ATM ===\n\n");
  printf("Choose one of the options below:\n\n");
  printf("[1]- Login\n");
  printf("[2]- Register\n");
  printf("[3]- Exit\n");
  while (!authenticated) {
    int option = getIntegerInput();
    switch (option) {
    case 1:
      loginMenu(user->name, user->password);
      if (authenticateUser(*user)) {
        printf("\n\nLogin successful!\n");
        authenticated = 1;
      } else {
        printf("\nIncorrect username or password.\n");
        printf("\nEnter 1 to try again or 3 to exit.\n");
      }
      break;
    case 2:
      registerMenu(user->name, user->password);
      if (isUniqueUsername(*user)) {
        FILE *usersData = fopen("./data/users.txt", "a+");
        struct User userChecker;
        int maxId = -1;

        if (usersData == NULL) {
          printf("Error opening users file.\n");
          exit(1);
        }

        while (fscanf(usersData, "%d %49s %49s", &userChecker.id,
                      userChecker.name, userChecker.password) == 3) {
          if (userChecker.id > maxId) {
            maxId = userChecker.id;
          }
        }

        user->id = maxId + 1;
        fseek(usersData, 0, SEEK_END);
        fprintf(usersData, "%d %s %s\n", user->id, user->name, user->password);
        fclose(usersData);
        authenticated = 1;
        printf("\nUser registration successful!\n");
      } else {
        printf("\nUsername already exists.\n");
        printf("\nEnter 2 to try again or 3 to exit.\n");
      }
      break;
    case 3:
      exit(0);
      break;
    default:
      printf("Enter a valid option.\n");
    }
  }
}

int main() {
  struct User user;

  initMenu(&user);
  mainMenu(user);
  return 0;
}
