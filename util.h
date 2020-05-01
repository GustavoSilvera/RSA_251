#ifndef UTIL_H
#define UTIL_H
#include <math.h>
#include <stdexcept>
#include <vector>
#include "number.h"
#include <utility>
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

//O(log n) mult inv
num gcdExtended(num a, num b, num *x, num *y) { 
  if (a == num(0)) { //base case
    *x = num(0);
    *y = num(1); 
    return b;
  }
  num x1 = num(0);
  num y1 = num(1); // To store results of recursive call
  std::pair<num, num> ba_divmod = b.divmod(a);
  num gcd = gcdExtended(ba_divmod.second, a, &(x1), &(y1));
  *(x) = y1 - ((ba_divmod.first) * x1); 
  *(y) = x1;
  return gcd; 
} 
//computes multiplicative inverse
num mod_inverse_in(num a, num m) {
  num x = num(0);
  num y = num(1);
  num g = gcdExtended(a, m, &x, &y);   //extended euclidean algorithm!
  if (g == num(1)) return (x % m);
  throw std::invalid_argument("ERROR: no multiplicative inverse!");
} 

template <typename T>
T modInverse(T a, T m) { 
  T m0 = m; 
  T y = 0, x = 1; 
  if (m == 1) return 0; 
  while (a > 1) { 
    T q = a / m; 
    T t = m;
    m = a % m;
    a = t; 
    t = y;
    y = x - q * y; 
    x = t;
  }
  if (x < 0) x = x + m0; 
  return x; 
}


//brute force modular inverse
//for debugging only... not practical
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
