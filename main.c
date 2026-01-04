#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PASSWORD_DEFAULT_LENGTH 20

void parse_args(int argc, char *argv[], int *password_length, int *numbers_only, int *symbols_only, int *alphabet_only) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 'l') {
      if (i + 1 < argc) {
        *password_length = atoi(argv[i + 1]);
        if (*password_length <= 0 || *password_length > 100) {
          fprintf(stderr, "Invalid password length. Must be between 1 and 100.\n");
          exit(1);
        }
        i++;
      } else {
        fprintf(stderr, "Option -l requires a length value.\n");
        exit(1);
      }
    } else if (argv[i][0] == '-' && argv[i][1] == 'n' && argv[i][2] == '\0') {
      *numbers_only = 1;
    } else if (argv[i][0] == '-' && argv[i][1] == 's' && argv[i][2] == '\0') {
      *symbols_only = 1;
    } else if (argv[i][0] == '-' && argv[i][1] == 'a' && argv[i][2] == '\0') {
      *alphabet_only = 1;
    } else {
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      exit(1);
    }
  }

  int exclusive_options = *numbers_only + *symbols_only + *alphabet_only;
  if (exclusive_options > 1) {
    fprintf(stderr, "Error: Options -a, -s, and -n are mutually exclusive.\n");
    exit(1);
  }
}

void build_charset(char *charset, int numbers_only, int symbols_only, int alphabet_only) {
  const char *lower = "abcdefghijklmnopqrstuvwxyz";
  const char *upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const char *numbers = "0123456789";
  const char *symbols = "!@#$%^&*()-_=+[]{}.,?";

  strcpy(charset, lower);
  strcat(charset, upper);

  if (alphabet_only) {
    return;
  } else if (symbols_only) {
    strcat(charset, symbols);
  } else if (numbers_only) {
    strcat(charset, numbers);
  } else {
    strcat(charset, numbers);
    strcat(charset, symbols);
  }
}

void generate_password(char *password, int length, const char *charset, int charset_size, int fd) {
  unsigned char random_bytes[100];
  if (read(fd, random_bytes,  sizeof(random_bytes)) != sizeof(random_bytes)) {
    perror("Failed to read from /dev/urandom");
    close(fd);
    exit(1);
  }

  for (int i = 0; i < length; i++) {
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
  password[length] = '\0';
}

int main(int argc, char *argv[]) {
  int password_length = PASSWORD_DEFAULT_LENGTH;
  int numbers_only = 0;
  int symbols_only = 0;
  int alphabet_only = 0;
  parse_args(argc, argv, &password_length, &numbers_only, &symbols_only, &alphabet_only);

  char charset[256];
  build_charset(charset, numbers_only, symbols_only, alphabet_only);
  int charset_size = strlen(charset);

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

  char password[101];
  generate_password(password, password_length, charset, charset_size, fd);
  close(fd);

  printf("%s\n", password);
  return 0;
}
