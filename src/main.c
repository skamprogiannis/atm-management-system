#include "header.h"

void mainMenu(struct User u) {
  int option;
  system("clear");
  printf("\n\n\t\t======= ATM =======\n\n");
  printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
  printf("\n\t\t[1]- Create a new account\n");
  printf("\n\t\t[2]- Update account information\n");
  printf("\n\t\t[3]- Check accounts\n");
  printf("\n\t\t[4]- Check list of owned account\n");
  printf("\n\t\t[5]- Make Transaction\n");
  printf("\n\t\t[6]- Remove existing account\n");
  printf("\n\t\t[7]- Transfer ownership\n");
  printf("\n\t\t[8]- Exit\n");
  option = getIntInput();

  switch (option) {
  case 1:
    createNewAcc(u);
    break;
  case 2:
    updateAccountInfo(u);
    break;
  case 3:
    // student TODO : add your **Check the details of existing accounts**
    // function here
    break;
  case 4:
    checkAllAccounts(u);
    break;
  case 5:
    // student TODO : add your **Make transaction** function
    // here
    break;
  case 6:
    // student TODO : add your **Remove existing account** function
    // here
    break;
  case 7:
    // student TODO : add your **Transfer owner** function
    // here
    break;
  case 8:
    exit(1);
    break;
  default:
    printf("Invalid operation!\n");
  }
};

void initMenu(struct User *u) {
  int r = 0;
  int option;
  system("clear");
  printf("\n\n\t\t======= ATM =======\n");
  printf("\n\t\t-->> Feel free to login / register :\n");
  printf("\n\t\t[1]- login\n");
  printf("\n\t\t[2]- register\n");
  printf("\n\t\t[3]- exit\n");
  while (!r) {
    option = getIntInput();
    switch (option) {
    case 1:
      loginMenu(u->name, u->password);
      if (checkPassword(*u)) {
        printf("\n\nPassword Match!");
        r = 1;
      } else {
        printf("\nWrong password or username\n");
        printf("\nEnter 1 to try again or 3 to exit\n");
      }
      break;
    case 2:
      registerMenu(u->name, u->password);
      if (isUniqueUsername(*u)) {
        FILE *usersData = fopen("./data/users.txt", "a+");
        struct User userChecker;
        int maxId = -1;

        if (usersData == NULL) {
          printf("Error opening users file\n");
          exit(1);
        }

        while (fscanf(usersData, "%d %49s %49s", &userChecker.id,
                      userChecker.name, userChecker.password) == 3) {
          if (userChecker.id > maxId) {
            maxId = userChecker.id;
          }
        }

        u->id = maxId + 1;
        fseek(usersData, 0, SEEK_END);
        fprintf(usersData, "%d %s %s\n", u->id, u->name, u->password);
        fclose(usersData);
        r = 1;
        printf("\nSuccessful user registration!\n");
      } else {
        printf("\nUsername already exists\n");
        printf("\nEnter 2 to try again or 3 to exit\n");
      }
      break;
    case 3:
      exit(1);
      break;
    default:
      printf("Insert a valid operation!\n");
    }
  }
};

int main() {
  struct User u;

  initMenu(&u);
  mainMenu(u);
  return 0;
}
