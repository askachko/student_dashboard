#include <stdio.h>

char username[50] = "Alex";

int main() {
  printf("Insert your username:");
  scanf("%49s", username);

  printf("Username entered: %s\n", username);
  return 0;
}

