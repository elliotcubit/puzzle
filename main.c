#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/select.h>

int specials[] = {25, 50, 75, 100};

#define PRIME_COUNT 168
int primes[] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
	67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137,
	139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211,
	223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
	293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379,
	383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461,
	463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563,
	569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643,
	647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739,
	743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829,
	839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937,
	941, 947, 953, 967, 971, 977, 983, 991, 997,
};

void xorSwap(int * x, int * y) {
	if (x != y) {
		*x ^= *y;
		*y ^= *x;
		*x ^= *y;
	}
}

int randint(int min, int max) {
	if (min > max)
		return randint(max, min);
	if (min == max)
		return min;
	return (rand() % (max - min)) + min;
}

void printList(int l, int * v) {
	if (!l) return;
	for (int i=0; i<l-1; i++) {
		printf("%d ", v[i]);
	}
	printf("%d", v[l-1]);
}

void * choice(int vc, int size, void * vv) {
	return vv + (randint(0, vc)*size);
}

void addition(int * workingc, int * working, int range) {
	int addend = randint(3, range);
	if (addend == working[0]) addend++;
	if (addend > working[0]) {
		working[(*workingc)++] = addend - working[0];
		working[0] = addend;
	} else {
		working[(*workingc)++] = working[0] - addend;
		working[0] = addend;
	}
}

int * factor(int n, int * l) {
	int factorc = 0;
	int * factors = malloc(5 * sizeof(int));
	if (factors == NULL) {
		return factors;
	}
	for (int i=0;n>1;i++) {
		if (factorc > 3) {
			factors[factorc++] = n;
			break;
		}
		int candidate = primes[i];
		while (!(n % candidate)) {
			n /= candidate;
			factors[factorc++] = candidate;
		}
	}
	*l=factorc;
	return realloc(factors, factorc);
}

typedef struct {
	double * arr;
	int size;
	int cap;
} DoubleStack;

DoubleStack * newIntStack(int initialSize) {
	DoubleStack * stack = malloc(sizeof(DoubleStack));
	if (!stack) return NULL;
	stack->cap = initialSize;
	stack->arr = malloc(sizeof(double)*stack->cap);
	if (!stack->arr) {
		free(stack);
		return NULL;
	}
	stack->size = 0;
	return stack;
}

int doubleStackPush(DoubleStack * stack, double val) {
	if (stack->size == stack->cap) {
		stack->cap += 10;
		double * tmp = realloc(stack->arr, sizeof(double)*stack->cap);
		if (!tmp) {
			return 1;
		} else {
			stack->arr = tmp;
		}
	}
	stack->arr[stack->size++] = val;
	return 0;
}

double doubleStackPop(DoubleStack * stack) {
	return stack->arr[--stack->size];
}

void doubleStackFree(DoubleStack * stack) {
	free(stack->arr);
	free(stack);
}

typedef struct {
	char * arr;
	int size;
	int cap;
} CharStack;

CharStack * newCharStack(int initialSize) {
	CharStack * stack = malloc(sizeof(CharStack));
	if (!stack) return NULL;
	stack->cap = initialSize;
	stack->arr = malloc(sizeof(char)*stack->cap);
	if (!stack->arr) {
		free(stack);
		return NULL;
	}
	stack->size = 0;
	return stack;
}

int charStackPush(CharStack * stack, char val) {
	if (stack->size == stack->cap) {
		stack->cap += 10;
		char * tmp = realloc(stack->arr, sizeof(char)*stack->cap);
		if (!tmp) {
			return 1;
		} else {
			stack->arr = tmp;
		}
	}
	stack->arr[stack->size++] = val;
	return 0;
}

char charStackPop(CharStack * stack) {
	return stack->arr[--stack->size];
}

char charStackPeek(CharStack * stack) {
	return stack->arr[stack->size-1];
}

void charStackFree(CharStack * stack) {
	free(stack->arr);
	free(stack);
}

int precedence(char operator) {
	switch (operator) {
		case '-':
			return 1;
		case '+':
			return 1;
		case '*':
			return 2;
		case '/':
			return 2;
		case ')':
			return 3;
		default:
			return -1;
	}
}

double eval(CharStack * charStack, DoubleStack * doubleStack) {
	char c = charStackPop(charStack);
	double left = doubleStackPop(doubleStack);
	double right = doubleStackPop(doubleStack);
	switch (c) {
		case '-':
			return right - left;
		case '+':
			return left + right;
		case '*':
			return left * right;
		case '/':
			return right / left;
	}
	return 1;
}

int evalInfix(double * result, char * expr, int numc, int * nums) {
	DoubleStack * doubleStack = newIntStack(10);
	CharStack * charStack = newCharStack(10);

	int n;
	double left;
	double right;
	char c;

	char * present = malloc(numc*sizeof(char));
	if (!present) {
		doubleStackFree(doubleStack);
		charStackFree(charStack);
		return 2;
	}
	for (int i=0; i<numc; i++)
		present[i] = 0;

	while (*expr != 0xA) {
		switch (*expr) {
			case '-':
				while (charStack->size > 0 && precedence(charStackPeek(charStack)) >= precedence(*expr)) {
					if (doubleStack->size < 2) goto invalid;
					if (doubleStackPush(doubleStack, eval(charStack, doubleStack))) goto fail;
				}
				if (charStackPush(charStack, *expr)) goto fail;
				expr++;
				break;
			case '+':
				while (charStack->size > 0 && precedence(charStackPeek(charStack)) >= precedence(*expr)) {
					if (doubleStack->size < 2) goto invalid;
					if (doubleStackPush(doubleStack, eval(charStack, doubleStack))) goto fail;
				}
				if (charStackPush(charStack, *expr)) goto fail;
				expr++;
				break;
			case '*':
				while (charStack->size > 0 && precedence(charStackPeek(charStack)) >= precedence(*expr)) {
					if (doubleStack->size < 2) goto invalid;
					if (doubleStackPush(doubleStack, eval(charStack, doubleStack))) goto fail;
				}
				if (charStackPush(charStack, *expr)) goto fail;
				expr++;
				break;
			case '/':
				while (charStack->size > 0 && precedence(charStackPeek(charStack)) >= precedence(*expr)) {
					if (doubleStack->size < 2) goto invalid;
					if (doubleStackPush(doubleStack, eval(charStack, doubleStack))) goto fail;
				}
				if (charStackPush(charStack, *expr)) goto fail;
				expr++;
				break;
			case '(':
				if (charStackPush(charStack, *expr)) goto fail;
				expr++;
				break;
			case ')':
				while (charStack->size > 0) {
					if (charStackPeek(charStack) == '(') {
						charStackPop(charStack);
						break;
					} else if (doubleStackPush(doubleStack, eval(charStack, doubleStack))) goto fail;
				}
				expr++;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				n = (int) strtol(expr, &expr, 10);
				char found = 0;
				for (int i=0; i<numc; i++) {
					if (nums[i] == n && !present[i]) {
						present[i] = 1;
						found = 1;
						break;
					}
				}
				if (!found) goto invalid;
				if (doubleStackPush(doubleStack, (double) n)) goto fail;
				break;
			default:
				expr++;
		}
	}

	while (charStack->size > 0) {
		if (doubleStack->size < 2) goto invalid;
		if (doubleStackPush(doubleStack, eval(charStack, doubleStack))) goto fail;
	}

	if (doubleStack->size != 1) goto invalid;

	*result = doubleStackPop(doubleStack);
	free(present);
	doubleStackFree(doubleStack);
	charStackFree(charStack);
	return 0;

	fail:
		free(present);
		doubleStackFree(doubleStack);
		charStackFree(charStack);
		return 2;
	invalid:
		free(present);
		doubleStackFree(doubleStack);
		charStackFree(charStack);
		return 1;
}

double distance(double x, double y) {
	if (x > y) {
		return x - y;
	} else {
		return y - x;
	}
}

int main(int argc, char ** argv) {
	FILE * f = fopen("/dev/urandom", "r");
	if (f == NULL) {
		perror("failed to open /dev/urandom");
		return 1;
	}

	int seed = 0;
	if (!fread((void *) &seed, sizeof(int), 1, f)) {
		perror("failed to read /dev/urandom");
		return 1;
	}
	srand(seed);

	int numc = 0;
	int * nums = malloc(20 * sizeof(int));
	if (nums == NULL) {
		perror("failed to allocate memory");
		return 1;
	}

	int n = randint(117, 987);
	int q = *(int*)choice(4, sizeof(int), specials);
	nums[numc++]=q;
	nums[numc++] = *(int*)choice(4, sizeof(int), specials);

	int workingc = 0;
	int * working = malloc(20 * sizeof(int));

	if (rand()%2) {
		working[workingc++] = n/q;
		working[workingc++] = n-((n/q)*q);
	} else {
		working[workingc++] = n/q+1;
		working[workingc++] = (((n/q)+1)*q)-n;
	}

	while (workingc) {
		// Stop splitting to stop it from getting too big
		if (numc > 5) {
			nums[numc++] = working[0];
			xorSwap(working, working + (--workingc));
			continue;
		}

		if (working[0] < 10) {
			// Final addition split
			addition(&workingc, working, 9);
			nums[numc++] = working[0];
			nums[numc++] = working[--workingc];
			xorSwap(working, working + (--workingc));
		} else {
			int numFactors;
			int * factors = factor(working[0], &numFactors);
			if (factors == NULL) {
				perror("malloc failed");
				return 1;
			}
			if (numFactors == 1) {
				addition(&workingc, working, 5);
				continue;
			} else { 
				for (int i=0; i<numFactors; i++) {
					working[workingc++]=factors[i];
				}
			}
			xorSwap(working, working + (--workingc));
			free(factors);
		}
	}
	free(working);

	printf("Get as close as possible to %d using the numbers ", n);
	printList(numc, nums);
	printf(" (Using only + - * / and parentheses).\n");

	char * line = NULL;
	size_t size = 0;
	double result = 0;

	while (result != (double) n) {
	printf(">>> ");
	getline(&line, &size, stdin);
		switch (evalInfix(&result, line, numc, nums)) {
			case 1:
				fprintf(stderr, "query is invalid\n");
				return 1;
			case 2:
				perror("evalInfix failed");
				return 1;
			default:
				break;
		}
		printf(" = %g (distance: %g)\n", result, distance((double) n, result));
	}

	free(nums);

	// TODO: Add timeout
	
	return 0;
}
