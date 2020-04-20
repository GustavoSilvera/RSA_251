#ifndef UTIL_H
#define UTIL_H
#include <math.h>
#include <stdexcept>
#include <vector>
#include "number.h"
//designed to work with num class
num totient(num P, num Q){
  //check for coprimality of P and Q
  return (P - num(1)) * (Q - num(1));
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
    if (e.is_odd()){
      ret = (ret * base) % mod;
    }
    e = e / num(2);
    base = (base * base) % mod;
  }
  return ret;
}
num gcd(num a, num b) { 
  if (a == num(0)) return b; 
  return gcd(b % a, a); 
}

inline num mod_inverse_fast(num a, num m) {
  //only works if m is prime!
  num g = gcd(a, m); 
  if (g == num(1)) {
    return exp_mod(a, m - num(2), m); 
  }
  throw std::invalid_argument("ERROR: no multiplicative inverse!");
}
//O(log n) mult inv
num gcdExtended(num a, num b, num *x, num *y) { 
  if (a == num(0)) { //base case
    *x = num(0), *y = num(1); 
    return b; 
  } 
  num x1, y1; // To store results of recursive call
  num gcd = gcdExtended(b % a, a, &x1, &y1);
  *x = y1 - (b / a) * x1; 
  *y = x1;
  return gcd; 
} 
//computes multiplicative inverse 
num mod_inverse_in(num a, num m) { 
    num x, y;
    //extended euclidean algorithm!
    num g = gcdExtended(a, m, &x, &y); 
    if (g == num(1))
      return ((x % m + m) % m);
    throw std::invalid_argument("ERROR: no multiplicative inverse!");
} 

//recursive implementation

num ObtainMultiplicativeInverse(num a, num b, num s0, num s1){
  return b==num(0) ? s0 :
    ObtainMultiplicativeInverse(b, a%b, s1, s0 - s1*(a/b));
}
 
num mult_inv_rec(num a, num m){
  return ObtainMultiplicativeInverse(a, m, num(1), num(0)) + num(2);
}

//brute force modular inverse
inline num mod_inverse(num a, num m) {
  //can possibly be optimized to log(n) via binary search
  a = a % m;//normalize in mod m
  for (num x = 1; x < m; x = x + num(1)){
    //try every one, guaranteed to terminate
    if ( (a * x) % m == num(1)){      
      return x;
    }
  }
  throw std::invalid_argument("ERROR: no multiplicative inverse!");
}
#endif
