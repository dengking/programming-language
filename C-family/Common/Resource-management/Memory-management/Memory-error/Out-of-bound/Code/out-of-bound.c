/*
https://en.wikipedia.org/wiki/Aliasing_(computing)
*/
# include <stdio.h>

int main()
{
  int arr[2] = { 1, 2 };
  int i=10;

  /* Write beyond the end of arr. Undefined behaviour in standard C, will write to i in some implementations. */
  arr[2] = 20;

  printf("element 0: %d \t", arr[0]); // outputs 1
  printf("element 1: %d \t", arr[1]); // outputs 2
  printf("element 2: %d \t", arr[2]); // outputs 20, if aliasing occurred
  printf("i: %d \t\t", i); // might also output 20, not 10, because of aliasing, but the compiler might have i stored in a register and print 10
  /* arr size is still 2. */
  printf("arr size: %d \n", (sizeof(arr) / sizeof(int)));
}