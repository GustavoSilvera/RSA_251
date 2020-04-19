#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

inline int mod_inverse(int a, int m) { 
  a = a % m;//normalize in mod m
  for (int x = 1; x < m; x++){ //try every one, guaranteed to terminate
    if ((a*x) % m == 1){
      return x;
    }
  }
  return -1;// should never get here
}
inline int exp_help(int base, int ex, int acc){
  if(ex == 0) return 1;
  if(ex == 1) return acc;
  return exp_help(base, ex - 1, acc * base);
}
inline int exp(int base, int ex){
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

int encode(std::string M){
  int ret = 0;
  int M_len = M.size();
  for(int i = 0; i < M_len; i++){
    char c = M[i];
    int enc = 0;//from ascii to our weird encoding
    if(c == ' ') enc = 0;// space = 0
    else enc = int(c) - 97 + 1; // (a -> z) lowercase
    ret += enc * (exp(27, M_len - i - 1));//base 27
  }
  return ret;
}
int max_exp(int C, int D){
  int n = 0;
  while(C >= exp(D, n + 1)){
    n++;
  }
  return n;
}
inline char base_27(int d){
  if(d == 0) return ' ';
  return char(d + 'a' - 1);
}
inline std::string reverse(std::string r){
  const int strlen = r.size();
  std::string ret;
  for(int i = strlen - 1; i >= 0; i--){
    char c = r[i];
    ret += c;
  }
  return ret;
}
std::string decode(int C){
  std::string ret = "";
  while(C >= 27){
    int d = C % 27;
    C /= 27;
    ret += base_27(d);
  }
  ret += base_27(C);
  return reverse(ret);
}

std::pair<int, int> Bob_setup(int &P, int &Q, int &priv){
  std::srand(std::time(nullptr));
  P = 7;
  Q = 11;
  int N = P * Q; // = 77
  int phi_N = (P-1) * (Q-1); // # of primes from 1 to 77
  int E = 7;// \in \mathbb{Z}_{phi_N}^*
  int E_inv = mod_inverse(E, phi_N); //P and Q are relatively prime
  std::pair<int, int> publ = std::make_pair(N, E);
  priv = E_inv;//not given out to the world
  return publ;//given out to the world
}

int Alice_setup(std::pair<int, int> publ, std::string &M){
  M = "at";
  int N = publ.first;//got from public domain
  int E = publ.second;//got from public domain
  return exp_mod(encode(M), E, N);
}

std::string Bob_decode(int P, int Q, int priv, int cipher){
  int M = exp_mod(cipher, priv, P*Q);
  return decode(M);
}

int main(){
  int priv_key, P, Q;
  std::pair<int, int> publ = Bob_setup(P, Q, priv_key);
  std::cout << "Public Domain: (N, E) = (" << publ.first << ", "<< publ.second << ")" << std::endl;
  std::cout << "Bob's P & Q: (" << P << ", " << Q << ")" << std::endl;
  std::cout << "Bob's Private Key: " << priv_key << std::endl;
  std::string M;
  int C = Alice_setup(publ, M);
  std::cout << "ALice's Message: \"" << M << "\"" << std::endl; 
  std::cout << "Alice's Ciphertext: " << C << std::endl;
  std::string M_bob = Bob_decode(P, Q, priv_key, C);
  std::cout << "Bob's Decoded Message: \"" << M_bob << "\"" << std::endl;
  /*int test = (4 * exp(27, 3) + 0 + 2 * exp(27, 1) + 3) % 77;
  long CC = exp(test, 3) % 77;
  std::cout << CC << std::endl;
  long CC_inv = mod_inverse(CC, 77);
  std::cout << CC_inv << std::endl;
  long MM = exp(CC, CC_inv);
  std::cout << MM << std::endl;
  std::cout << decode(540) << std::endl;*/
  return 0;
}
