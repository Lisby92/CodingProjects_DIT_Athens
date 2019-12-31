#ifndef _GENERATE_H
#define _GENERATE_H

// Return values of generation functions //
#define GEN_NEXT  0 // OK!, print and continue //
#define GEN_TERM  1 // OK!, terminate //
#define GEN_EMPTY 2 // OK!, print EMPTY SET and continue //
#define GEN_ERROR 3 // An error occured, print an error message and terminate //

//Combinatorial generation functions //
int gen_comb_norep_lex_init(unsigned char *vector, const unsigned char n, const unsigned char k);
int gen_comb_norep_lex_next(unsigned char *vector, const unsigned char n, const unsigned char k);

#endif
