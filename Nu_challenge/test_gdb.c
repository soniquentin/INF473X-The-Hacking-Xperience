#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

int parse()
{
  char greeting_text[128];
  char buf[256] = {0};

  printf("What is your name?\n");

  fgets(buf, sizeof(buf), stdin);
  strcpy(greeting_text, "Hello, dear ");
  strcat(greeting_text, buf);

  return 0;
}

int main(int argc, char** argv)
{

parse();
int x = 2;
  return x;
}
