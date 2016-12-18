#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

// initialize the counter for the number of comparisons
static size_t numberOfComparisons = 0;

// inline function declarations
void reportErrorMsg(const char* msg);
void IOCheck(ssize_t ret);
void memoryCheck(void* ptr);
char decrypt(const char c);
int wFrobCmp(const void* a, const void* b);

// function declarations
int frobcmp(char const* a, char const* b);
void writeAndCheckIO(const char* str);
size_t returnMyLineNumber(char** lineBuffer, char* buf, size_t size);

// main function
int main()
{
  int (* cmp) (const void*, const void*);

  // char pointer declarations
  char* input1;
  char* input2;
  char *line;
  char** lineBuffer;

  // char declarations
  char inputBuffer[2];
  char currentChar;

  // int declarations
  int endOfFile;
  int isSpaceChar;
  
  // size_t declarations
  size_t lineNumber;
  size_t sizeOfLine;
  size_t bufSize;
  size_t sizeOfFile;

  // struct for file informatoin
  struct stat buf;
  
  // ssize_t declaration
  ssize_t ret;
  
  // initializing variables
  bufSize = 0;
  lineNumber = 0;
  sizeOfLine = 0;
  endOfFile = 0;

  // cmp now points to the address of wFrobCmp
  cmp = &wFrobCmp;

  // initialize ret and check for IO errors
  ret = fstat(STDIN_FILENO, &buf);
  IOCheck(ret);
  sizeOfFile = 1 + buf.st_size;

  // allocate memory for input
  input1 = (char*) malloc(sizeof(char) * sizeOfFile);
  memoryCheck(input1);
  line = input1;

  // loop through this while the end of the file isn't reached
  while (!endOfFile) {

    // read input and check for IO errors 
    ret = read(STDIN_FILENO, inputBuffer, 1);
    IOCheck(ret);

    // initialize the current character
    currentChar = inputBuffer[0];
    isSpaceChar = currentChar == ' ';
    endOfFile = !ret;
    if (!sizeOfLine && isSpaceChar)
      continue;

    // allocate memory if the buffer size is equal to the file size
    if (bufSize == sizeOfFile) {
      sizeOfFile *= 2;
      input2 = (char*) realloc(input1, sizeof(char) * sizeOfFile);
      memoryCheck(input2);
      input1 = input2;
    }

    // if the end of the file isn't reached, store character and increment sizeOfLine
    if (!endOfFile) {
      input1[bufSize++] = currentChar;
      sizeOfLine++;
      if (!isSpaceChar)
        continue;
    }

    // if the end of the file is reached, free input1 and return 0
    else {

      // if you encounter empty file or file of only spaces
      if (!bufSize) {
        free(input1);
        return 0;
      }
      if (input1[bufSize-1] != ' ')
        input1[bufSize++] = ' ';
      if (!sizeOfLine)
        break;
    }

    // move on to the next line and reset the sizeOfLine
    lineNumber++;
    sizeOfLine = 0;
  }

  // allocate memory for line buffer
  lineBuffer = (char**) malloc(sizeof(char*) * lineNumber);

  // check for memory allocation errors
  memoryCheck(lineBuffer);

  // check for line number errors
  if (lineNumber != returnMyLineNumber(lineBuffer, input1, bufSize))
    reportErrorMsg("Incorrect Line Number");

  // sort using qsort, and using the wFrobCmp function
  qsort(lineBuffer, lineNumber, sizeof(char*), cmp);

  // produce output using writeAndCheckIO
  size_t n;
  for (n = 0; n < lineNumber; n++)
    writeAndCheckIO(lineBuffer[n]);

  // free all the memory
  free(lineBuffer);
  free(input1);

  // print the number of comparisons
  fprintf(stderr, "Comparisons: %ld\n", numberOfComparisons);
  return 0;
}

// INLINE FUNCTIONS //

// print error message and exit(1)
inline
void reportErrorMsg(const char* errorMsg)
{
  fprintf(stderr, "%s Error\n", errorMsg);
  exit(1);
}

// check for IO error
inline
void IOCheck(ssize_t ret)
{
  if (ret < 0)
    reportErrorMsg("IO");
}

// check for memory allocation error
inline
void memoryCheck(void* pointer)
{
  if (pointer == NULL)
    reportErrorMsg("Memory Allocation");
}

// decrypt a single char with XOR
inline
char decrypt(const char c)
{
  char d = c ^ 42;
  return d;
}

// wrapper function that calls frobcmp
inline
int wFrobCmp(const void* a, const void* b)
{
  return frobcmp(*((const char**) a), *((const char**) b));
}

// FUNCTIONS //

// frobcmp returns:
//    1 if a > b
//   -1 if a < b
//    0 if a = b
int frobcmp(char const* a, char const* b)
{
  // increment comparison count
  numberOfComparisons++;

  for (; *a == *b; a++, b++)
    if (*a == ' ')
      return 0;
  if (decrypt(*a) > decrypt(*b))
    return 1;
  else
    return -1;
}

// write to output and check for IO error
void writeAndCheckIO(const char* myString)
{
  ssize_t ret;
  for (;;) {
    ret = write(STDOUT_FILENO, myString, 1);
    IOCheck(ret);
    if (*myString++ == ' ')
      return;
  }
}

// returns lineNumber, given line buffer
size_t returnMyLineNumber(char** lineBuffer, char* buf, size_t size)
{
  size_t n;
  size_t lineNumber;
  char* line = buf;
  
  for (n = 0, lineNumber = 0; n < size; n++) {
    if (buf[n] == ' ') {
      lineBuffer[lineNumber++] = line;
      line = buf + 1 + n;
    }
  }
  return lineNumber;
}
