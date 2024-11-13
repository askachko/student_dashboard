#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct User {
  char username[50];
  char password[50];
};

// for user input
char username[50];
char password[50];

#define NUM_OPTIONS 4
const char *menu_options[NUM_OPTIONS] = {"Grades    ", "Classes   ",
                                         "Upcoming  ", "GPA       "};

int cursor_position = 1;

void move_cursor(int direction){
  cursor_position += direction;
  if (cursor_position < 0) cursor_position = NUM_OPTIONS - 1;
  if (cursor_position >= NUM_OPTIONS) cursor_position = 0;
}

struct User find_user_by_username(char username[50]);

bool verify_user(struct User user, char entered_password[50]);
void header(int col, char text[50]);
void center_text(int width, const char *text);

int main(int argc, char **argv) {

  // clear terminal
  printf("\033[2J\033[1;1H");

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  header(w.ws_col, "Login");
  printf("\n\n   Insert your username: ");
  scanf("%49s", username);

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

  printf("\033[2J\033[1;1H");
  header(w.ws_col, "Dashboard");

  char k;
  while (1) {
    scanf("%c", &k);

    printf("\n\n\n\n");
    center_text(w.ws_col, "Grades    ");
    center_text(w.ws_col, "Classes   ");
    center_text(w.ws_col, "Upcoming  ");
    center_text(w.ws_col, "GPA       ");
    printf("\n\n\n\n");
  }

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
