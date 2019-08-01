#ifndef _GAE_HASHSTRING_H_
#define _GAE_HASHSTRING_H_

typedef unsigned int gae_hashstring;
#define gae_invalid_hashstring (gae_hashstring)(-931176343)

/* Calculate a hashstring based on the given string */
gae_hashstring gae_hashstring_calculate(const char* string);

/* Compare function for hashstrings */
int gae_hashstring_compare(void* const A, void* const B);

#endif
