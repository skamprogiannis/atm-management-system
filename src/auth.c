#include "header.h"
#include <stdbool.h>
#include <termios.h>

char *USERS = "./data/users.txt";

void registerMenu(char a[50], char pass[50]) {
  system("clear");
  printf(
      "\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\tPick your username:");
  scanf("%49s", a);

  printf("\n\n\n\n\n\t\t\t\tCreate your password:");
  scanf("%49s", pass);
}

void loginMenu(char a[50], char pass[50]) {
  struct termios oflags, nflags;

  system("clear");
  printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
  scanf("%49s", a);

  // disabling echo
  tcgetattr(fileno(stdin), &oflags);
  nflags = oflags;
  nflags.c_lflag &= ~ECHO;
  nflags.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
  printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
  scanf("%49s", pass);

  // restore terminal
  if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
}

bool checkPassword(struct User u) {
  FILE *fp;
  struct User userChecker;

  if ((fp = fopen(USERS, "r")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%d %49s %49s", &userChecker.id, userChecker.name,
                userChecker.password) == 3) {
    if (strcmp(userChecker.name, u.name) == 0) {
      fclose(fp);
      return strcmp(u.password, userChecker.password) == 0;
    }
  }

  fclose(fp);
  printf("no user found");
  return false;
}

bool isUniqueUsername(struct User u) {
  FILE *fp;
  struct User userChecker;

  if ((fp = fopen(USERS, "r")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%d %49s %49s", &userChecker.id, userChecker.name,
                userChecker.password) == 3) {
    if (strcmp(userChecker.name, u.name) == 0) {
      fclose(fp);
      return false;
    }
  }

  fclose(fp);
  return true;
}
