#include "header.h"
#include <termios.h>

const char *USERS = "./data/users.txt";

void registerMenu(char name[50], char password[50]) {
  system("clear");
  printf("\n=== Register ===\n\n");
  printf("Choose a username: ");
  scanf("%49s", name);

  printf("Create a password: ");
  scanf("%49s", password);
}

void loginMenu(char name[50], char password[50]) {
  struct termios oflags, nflags;

  system("clear");
  printf("\n=== Login ===\n\n");
  printf("Username: ");
  scanf("%49s", name);

  // Save current terminal settings so password input can be hidden temporarily.
  if (tcgetattr(fileno(stdin), &oflags) != 0) {
    perror("tcgetattr");
    return exit(1);
  }
  nflags = oflags;
  // Disable typed-character echo while keeping Enter/newline behavior normal.
  nflags.c_lflag &= ~ECHO;
  nflags.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
  printf("Password: ");
  scanf("%49s", password);

  // Restore the original terminal settings so later input is visible again.
  if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
}

int authenticateUser(struct User user) {
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
      return strcmp(user.password, userChecker.password) == 0;
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
