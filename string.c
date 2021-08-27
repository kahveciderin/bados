#include "include/string.h"

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s++) {
    len++;
  }
  return len;
}

int strcmp(const char *s1, const char *s2){
    if(strlen(s1) != strlen(s2)) return 0;
    while(*s1 != '\0'){
        if(*s1 != *s2) return 0;
        s1++;
        s2++;
    }
    return 1;
}