#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Return 1 if the character has a duplicate byte
// Return 0 if the character has no duplicate bytes
int charDupe(const char* str, char c)
{
  for (; *str != 0; str++)
  {
    if (*str == c)
      return 1;
  }
  return 0;
}

// Return 1 if the string has duplicates
// Return 0 of the string has no duplicates
int hasDupe(const char* str)
{
  for (; *str != 0; str++)
  {
    if (charDupe((str+1), *str))
      return 1;
  }
  return 0;
}

// Output error message and exit(1)
void reportError(const char* err)
{
  fprintf(stderr, err);
  exit(1);
}

// Check for errors using ferror command
void checkIOForError(FILE* f)
{
  if (ferror(f))
    reportError("IO Error\n");
}

// Find character matching c and return the character in the same position in to
// If the character c does not exist in from, just return the char c
char findChar(const char* from, const char* to, size_t length, char c)
{
  size_t n = 0;
  for (; n < length; n++)
  {
    if (c == from[n])
      return to[n];
  }
  return c;
}

int main(int argc, const char* argv[])
{
  char src[2];
  
  const char* from = argv[1];
  const char* to = argv[2];

  int fromLength = strlen(from); 
  
  if (strlen(from) != strlen(to))
    reportError("Arguments need to be the same length\n");
  
  if (!strlen(from))
    reportError("Missing an argument\n");

  if (hasDupe(from))
    reportError("Argument has duplicate bytes\n");

  if (argc != 3)
    reportError("Needs 2 arguments\n");

  for (;;)
  {
    ssize_t r = read(STDIN_FILENO, src, 1);
    if (!r)
      break;
    else if (r < 0)
      reportError("Error with read\n");
    src[0] = findChar(from, to, fromLength, src[0]);
    r = write(STDOUT_FILENO, src, 1);
    if (r < 0)
      reportError("Error with write\n");
  }

  return 0;
}
