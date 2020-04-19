#ifndef UTIL_H
#define UTIL_H
#include <math.h>
#include <stdexcept>
#include "number.h"
//designed to work with num class
num totient(num P, num Q){
  //check for coprimality of P and Q
  return (P - num(1)) * (Q - num(1));
}

inline num mod_inverse(num a, num m) { 
  a = a % m;//normalize in mod m
  for (num x = num(1); x < m.get_value(); x = x + num(1)){
    //try every one, guaranteed to terminate
    if ( (a * x) % m == num(1)){
      return x;
    }
  }
  throw std::invalid_argument("ERROR: no multiplicative inverse!");
}

inline num exp_help(num base, num ex, num acc){
  if(ex == num(0)) return num(1);
  if(ex == num(1)) return acc;
  return exp_help(base, ex - num(1), acc * base);
}

num exp(num base, num ex){
  return exp_help(base, ex, base);
}

num exp_mod(num base, num e, num mod){
  //via repeated squaring
  num ret = num(1);
  while (e > num(0)){
    if (e % 2 == num(1)){
      ret = (ret * base) % mod;
    }
    e = e / 2;
    base = (base * base) % mod;
  }
  return ret;
}
#endif
