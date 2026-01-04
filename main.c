#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define PASSWORD_DEFAULT_LENGTH 20

int main(int argc, char *argv[]) {
  int password_length = PASSWORD_DEFAULT_LENGTH;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 'l') {
      if (i + 1 < argc) {
        password_length = atoi(argv[i + 1]);
        if (password_length <= 0 || password_length > 100) {
          fprintf(stderr, "Invalid password length. Must be between 1 and 100.\n");
          return 1;
        }
      } else {
        fprintf(stderr, "Option -l requires a length value.\n");
        return 1;
      }
    }
  }

  char password[101] = "password";
  const char charset[] = "abcdefghijklmnopqrstuvwxyz"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "0123456789"
                         "!@#$%^&*()-_=+[]{}.,?";
  int charset_size = sizeof(charset) - 1;

  int fd = open("/dev/urandom", O_RDONLY);
  if (fd < 0) {
    perror("Failed to open /dev/urandom");
    return 1;
  }

  unsigned char random_bytes[100];
  if (read(fd, random_bytes, sizeof(random_bytes)) != sizeof(random_bytes)) {
    perror("Failed to read from /dev/urandom");
    close(fd);
    return 1;
  }

  for (int i = 0; i < password_length; i++) {
    // Modulo bias
    unsigned char limit = 256 - (256 % charset_size);
    unsigned char value;
    do {
      value = random_bytes[i];
      if (value >= limit) {
        read(fd, &value, 1);
      }
    } while (value >= limit);

    password[i] = charset[value % charset_size];
  }
  close(fd);
  password[password_length] = '\0';

  printf("%s\n", password);
  return 0;
}
