#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define INIT_SIZE 19

// Print Error Messages and Exit
void errorMsg(const char* message) {
  fprintf(stderr, "%s Error: %d\n", message, errno);
  exit(1);
}

// Decrypt a single character
char decrypterino(const char c) {
  char d = c ^ 42;
  return d;
}

// Compare frobbed characters, stopping at space characters
// Return 1 when a > b, return 0 when a == b, and return -1 when a < b
int frobcmp(char const* x, char const* y) {
  for (; *x == *y; x++, y++) {
    if (*x == ' ') {
        return 0;
    }
  }
  if (decrypterino(*x) > decrypterino(*y)) {
      return 1;
  }
  else {
      return -1;
  }
}

// Compare frobbed words, deference a double pointer to get a pointer
// Recast it to const char within the function to avoid error
int compareWords(const void* x, const void* y) {
  return frobcmp(*((const char**) x), *((const char**) y));
}

int main(void)
{
  size_t lineNum = 0;
  size_t lineLen = 0;
  size_t bufSize = 0;

  size_t fileSize = INIT_SIZE;

  int space;
  int endOfFile = feof(stdin);

  char *input;
  char *inputResize;
  char **lineBuffer;
  char curChar;

  // This creates a pointer that points to the function compareWords
  // This cmp pointer is later used when we call qsort so we can compare frobs
  int (*cmp) (const void*, const void*) = &compareWords;

  // Allocate Memory
  input = (char*) malloc(sizeof(char) * fileSize);

  // Memory Check
  if (input == NULL) {
      errorMsg("Memory Allocation Error");
  }

  // Loop through while the end of the file isn't reached
  while (!endOfFile) {

    // IO Check
    if (ferror(stdin)) {
        errorMsg("IO Error");
    }

    curChar = getchar();
    space = curChar == ' ';
    endOfFile = feof(stdin);

    // Multiple Space Situation
    if (lineLen == 0 && space) {
      continue;
    }

    // Modify buffer size
    if (bufSize == fileSize) {
	fileSize *= 2;
        inputResize = (char*) realloc(input, sizeof(char) * fileSize);
        if (inputResize == NULL) {
            errorMsg("Memory Allocation Error");
	}
        input = inputResize;
    }

    // Keep track of current Char
    if (!endOfFile) {
	input[bufSize++] = curChar;
        lineLen++;
        if (!space) {
	    continue;
	}
    }
    else {
      if (bufSize == 0) {
          free(input);
	  return 0;
      }

      // Make spaces if required
      if (input[bufSize-1] != ' ') {
	  input[bufSize++] = ' ';
      }

      if (lineLen == 0) {
	  break;
      }
    }

    // Make a new line
    lineNum++;
    lineLen = 0;
  }

  // Allocate Space based on size of character pointer and number of lines
  lineBuffer = (char**) malloc(sizeof(char*) * lineNum);
  if (lineBuffer == NULL) {
      errorMsg("Memory Allocation Error");
  }

  size_t x;
  size_t line;
  char* lineRes = input;
  
  for (x = 0, line = 0; x < bufSize; x++) {
    // If the input is a space Char
    if (input[x] == ' ') {
        lineBuffer[line++] = lineRes;
        lineRes = input + 1 + x;
    }
  }

  // Use qsort to sort the input
  qsort(lineBuffer, lineNum, sizeof(char*), cmp);

  // Print loop
  for (x = 0; x < lineNum; x++) {
    for (;;) {
      putchar(*lineBuffer[x]);
      if (ferror(stdout)) {
          errorMsg("IO Error");
      }
      if (*lineBuffer[x]++ == ' ') {
	  break;
      }
    }
  }

  // Free all the memory
  free(lineBuffer);
  free(input);
  return 0;
}
