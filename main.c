#include <stdio.h>
#include <string.h>

struct User {
  char username[50];
  char password[50];
};

char username[50];
char password[50];

struct User find_user_by_username(char username[50]);
int verify_user(struct User user, char entered_password[50]);

int main() {

  printf("Insert your username:");
  scanf("%49s", username);

  printf("Insert your password:");
  scanf("%49s", password);

  struct User foundUser = find_user_by_username(username);

  // If the username exists
  if (foundUser.username > 0) {
    return verify_user(foundUser, password);
  };

  return 0;
}

struct User find_user_by_username(char username[50]) {

  struct User user = {"", ""};

  FILE *fp;
  char filename[] = "database.txt";
  char line[256];

  fp = fopen(filename, "r");

  if (fp == NULL) {
    perror("Error opening file");
    return user;
  }

  while (fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "\"%49[^\"]\", \"%49[^\"]\"", user.username,
               user.password) == 2) {
      if (strcmp(user.username, username) == 0) {
        fclose(fp);
        return user; // Return the found user
      }
    };
  }

  fclose(fp);
  struct User emptyUser = {"", ""};
  return emptyUser;
}

int verify_user(struct User user, char entered_password[50]) {
  if (strcmp(user.username, username) == 0) {
    printf("Verified, welcome %s", user.username);
    return 0;
  } else {
    printf("Unauthorized");
    return 1;
  }
};
