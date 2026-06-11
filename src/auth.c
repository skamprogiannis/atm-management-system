#include "header.h"
#include <termios.h>

const char *USERS = "./data/users.txt";

static void scanHiddenPassword(char password[50]) {
  struct termios oldSettings, hiddenSettings;

  fflush(stdout);
  if (tcgetattr(fileno(stdin), &oldSettings) != 0) {
    perror("tcgetattr");
    return exit(1);
  }
  hiddenSettings = oldSettings;
  hiddenSettings.c_lflag &= ~ECHO;
  hiddenSettings.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &hiddenSettings) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
  scanf("%49s", password);

  if (tcsetattr(fileno(stdin), TCSANOW, &oldSettings) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
}

void registerMenu(char name[50], char password[50]) {
  system("clear");
  printHeader("Register");
  printf("Choose a username: ");
  scanf("%49s", name);

  printf("Create a password: ");
  scanHiddenPassword(password);
}

void loginMenu(char name[50], char password[50]) {
  system("clear");
  printHeader("Login");
  printf("Username: ");
  scanf("%49s", name);

  printf("Password: ");
  scanHiddenPassword(password);
}

int authenticateUser(struct User *user) {
  FILE *usersFile;
  struct User userChecker;

  if ((usersFile = fopen(USERS, "r")) == NULL) {
    printf("Error opening users file.\n");
    exit(1);
  }

  while (fscanf(usersFile, "%d %49s %49s", &userChecker.id, userChecker.name,
                userChecker.password) == 3) {
    if (strcmp(userChecker.name, user->name) == 0) {
      fclose(usersFile);
      if (strcmp(user->password, userChecker.password) == 0) {
        user->id = userChecker.id;
        return 1;
      }
      return 0;
    }
  }

  fclose(usersFile);
  printf("User not found.\n");
  return 0;
}

int isUniqueUsername(struct User user) {
  FILE *usersFile;
  struct User userChecker;

  if ((usersFile = fopen(USERS, "r")) == NULL) {
    printf("Error opening users file.\n");
    exit(1);
  }

  while (fscanf(usersFile, "%d %49s %49s", &userChecker.id, userChecker.name,
                userChecker.password) == 3) {
    if (strcmp(userChecker.name, user.name) == 0) {
      fclose(usersFile);
      return 0;
    }
  }

  fclose(usersFile);
  return 1;
}
