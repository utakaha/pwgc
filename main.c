#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
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

  for (int i = 0; i < 20; i++) {
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
  password[20] = '\0';

  printf("%s\n", password);
  return 0;
}
