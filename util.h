#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdexcept>

int totient(int P, int Q){
  //check for coprimality of P and Q
  return (P - 1) * (Q - 1);
}

inline int mod_inverse(int a, int m) { 
  a = a % m;//normalize in mod m
  for (int x = 1; x < m; x++){ //try every one, guaranteed to terminate
    if ((a*x) % m == 1){
      return x;
    }
  }
  throw std::invalid_argument("ERROR: no multiplicative inverse!");
}

inline int exp_help(int base, int ex, int acc){
  if(ex == 0) return 1;
  if(ex == 1) return acc;
  return exp_help(base, ex - 1, acc * base);
}

int exp(int base, int ex){
  return exp_help(base, ex, base);
}

int exp_mod(int base, int exp, int mod){
  int ret = 1;
  while (exp > 0){
    if (exp % 2 == 1){
      ret = (ret * base) % mod;
    }
    exp = exp >> 1;
    base = (base * base) % mod;
  }
  return ret;
}
#endif
