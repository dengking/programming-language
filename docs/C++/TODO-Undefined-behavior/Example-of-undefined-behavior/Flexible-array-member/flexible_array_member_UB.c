/*
 * flexible_array_member_UB.c
 *
 * token from https://en.cppreference.com/w/c/language/struct
 *
 */
#include<malloc.h>
#include<stdio.h>

struct s {
	int n;
	double d[]; // s.d is a flexible array member
};

int main() {
	struct s t1 = { 0 };         // OK, d is as if double d[1], but UB to access
	// When an element of the flexible array member is accessed (in an expression that uses operator . or -> with the flexible array member's name as the right-hand-side operand), then the struct behaves as if the array member had the longest size fitting in the memory allocated for this object. If no additional storage was allocated, it behaves as if an array with 1 element, except that the behavior is undefined if that element is accessed or a pointer one past that element is produced.
	// the following will print unmeaning value
	printf("%d\n", t1.d[0]);
	printf("%d\n", t1.d[1]);

	//struct s t2 = { 1, { 4.2 } }; // error: initialization ignores flexible array

	// if sizeof (double) == 8
	struct s *s1 = malloc(sizeof(struct s) + 64); // as if d was double d[8]
	struct s *s2 = malloc(sizeof(struct s) + 40); // as if d was double d[5]

	s1 = malloc(sizeof(struct s) + 10); // now as if d was double d[1]. Two bytes excess.
	double *dp = &(s1->d[0]);    // OK
	*dp = 42;                    // OK
	s1->d[1]++; // Undefined behavior. 2 excess bytes can't be accessed as double.

	s2 = malloc(sizeof(struct s) + 6); // same, but UB to access because 2 bytes are
									   // missing to complete 1 double

	dp = &(s2->d[0]);            //  OK, can take address just fine
	*dp = 42;                    //  undefined behavior
	printf("%d\n", s2->d[0]); // the output will not be 42, but a unmeaning value

	*s1 = *s2; // only copies s.n, not any element of s.d except those caught in sizeof (struct s)

}
