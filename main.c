#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct User {
  char *username;
  char *password;
};

// for user input
char username[50];
char password[50];

#define NUM_OPTIONS 4
const char *pMenu_options[NUM_OPTIONS] = {"Grades    ", "Classes   ",
                                          "Upcoming  ", "GPA       "};

#define FILE_NAME "database.txt"

int cursor_position = 1;

void move_cursor(int direction) {
  cursor_position += direction;
  if (cursor_position < 0)
    cursor_position = NUM_OPTIONS - 1;
  if (cursor_position >= NUM_OPTIONS)
    cursor_position = 0;
}

struct User find_user_by_username(char username[50]);

bool verify_user(struct User user, char entered_password[50]);
void header(int col, char text[50]);
void center_text(int width, const char *text);

void create_user(struct User new_user) {

  FILE *fptr = fopen(FILE_NAME, "a"); // Open the file for appending

  if (fptr == NULL) {
    perror("Error opening file");
    return;
  }

  // Write the new user's details to the file
  fprintf(fptr, "\"%s\", \"%s\"\n", new_user.username, new_user.password);
  fclose(fptr);
};

void print_register(int width) {

  char username[50];
  char password[50];

  printf("\033[2J\033[1;1H");
  header(width, "Register");

  while (1) {
    printf("\n\n   Enter a username: ");
    scanf("%s", username);

    printf("\n\n   Enter a password: ");
    scanf("%s", password);

    struct User new_user = {username, password};

    create_user(new_user);
    break;
  };
};

void print_menu(int width) {

  printf("\033[2J\033[1;1H");
  header(width, "Dashboard");

  for (int i = 0; i < NUM_OPTIONS; i++) {
    center_text(width, pMenu_options[i]);
  };
};

int main(int argc, char **argv) {

  // clear terminal
  printf("\033[2J\033[1;1H");

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  bool registered = true;

  header(w.ws_col, "Login");
  printf("\n\n   Insert your username: ");
  printf("\n\n\n   Or enter (1) to register\n");

  scanf("%49s", username);

  if (strcmp(username, "1") == 0) {
    registered = false;
  }

  if (!registered)
    print_register(w.ws_col);

  printf("\n\n   Insert your password: ");
  scanf("%49s", password);

  struct User foundUser = find_user_by_username(username);

  bool is_verified;

  // If the username exists
  if (foundUser.username > 0) {
    is_verified = verify_user(foundUser, password);
  } else {
    printf("User does not exist");
    return 0;
  };

  if (is_verified == false) {
    return 1;
  }

  // dashboard

  print_menu(w.ws_col);

  char k;
  while (1) {
  }

  return 0;
}

struct User find_user_by_username(char username[50]) {

  struct User user = {"", ""};

  FILE *fp;
  char filename[] = FILE_NAME;
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

bool verify_user(struct User user, char entered_password[50]) {
  if (strcmp(user.password, entered_password) == 0) {
    return true;
  } else {
    printf("Error, wrong password");
    return false;
  }
};

void header(int col, char text[50]) {
  char line[256] = "";

  for (int i = 0; i < col; i++) {
    line[i] = '=';
  };
  printf("%s", line);
  center_text(col, text);
  printf("%s", line);
}

void center_text(int width, const char *text) {
  int text_length = strlen(text);

  if (text_length > width) {
    printf("%s\n", text);
    return;
  }

  int padding = (width - text_length) / 2;
  int right_padding = width - text_length - padding;

  for (int i = 0; i < padding; i++)
    putchar(' ');
  printf("%s", text);
  for (int i = 0; i < right_padding; i++)
    putchar(' ');
  putchar('\n');
};
