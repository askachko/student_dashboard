#include <stdio.h>

struct User {
  char username[50];
  char password[50];
};

char username[50];
char password[50];

void find_user_by_username(char username[50]);

int main() {

  printf("Insert your username:");
  scanf("%49s", username);

  printf("Insert your password:");
  scanf("%49s", password);

  FILE *fp;
  char filename[] = "database.txt";
  char line[256];

  fp = fopen(filename, "r");

  if (fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  while (fgets(line, sizeof(line), fp) != NULL) {
      printf("%s", line);
  }

  fclose(fp);

  return 0;
}

void find_user_by_username(char username[50]) {}
