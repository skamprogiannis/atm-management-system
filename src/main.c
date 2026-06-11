#include "header.h"

void mainMenu(struct User user) {
  int option;
  system("clear");
  printHeader("ATM");
  printf("Choose one of the options below:\n\n");
  printf("[1]- Create a new account\n");
  printf("[2]- Update account information\n");
  printf("[3]- Check account details\n");
  printf("[4]- List owned accounts\n");
  printf("[5]- Make a transaction\n");
  printf("[6]- Remove existing account\n");
  printf("[7]- Transfer ownership\n");
  printf("[8]- Exit\n");
  printf("\nEnter your choice: ");
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

int shouldRetryAuth() {
  int option;

  do {
    printf("\nEnter 1 to try again or 0 to exit: ");
    option = getIntegerInput();

    if (option == 1) {
      return 1;
    } else if (option == 0) {
      exit(0);
    } else {
      printf("Enter a valid option.\n");
    }
  } while (option != 0 && option != 1);

  return 0;
}

void initMenu(struct User *user) {
  int authenticated = 0;
  system("clear");
  printHeader("ATM");
  printf("Choose one of the options below:\n\n");
  printf("[1]- Login\n");
  printf("[2]- Register\n");
  printf("[3]- Exit\n");
  while (!authenticated) {
    printf("\nEnter your choice: ");
    int option = getIntegerInput();
    switch (option) {
    case 1:
      do {
        loginMenu(user->name, user->password);
        if (authenticateUser(user)) {
          printf("\n\nLogin successful!\n");
          authenticated = 1;
          break;
        }

        printf("\nIncorrect username or password.\n");
      } while (shouldRetryAuth());
      break;
    case 2:
      do {
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
          fprintf(usersData, "%d %s %s\n", user->id, user->name,
                  user->password);
          fclose(usersData);
          authenticated = 1;
          printf("\nUser registration successful!\n");
          break;
        }

        printf("\nUsername already exists.\n");
      } while (shouldRetryAuth());
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
