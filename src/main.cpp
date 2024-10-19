#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

  int fd = open("data/siftsmall/siftsmall_base.fvecs", O_RDONLY | O_CREAT);
  float c;
  ssize_t sz;

  int items = 0;
  while ((sz = read(fd, &c, sizeof(c))) != 0) {
    items++;
  }
  std::cout << items << std::endl;

  close(fd);

  return 0;

}
