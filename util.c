#include <stdio.h>

/**
 * Print an error message to stderr.
 */
void eprint(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}