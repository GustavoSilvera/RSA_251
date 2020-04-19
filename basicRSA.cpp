#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include "util.h"
#include "encoding.h"
#include "person.h"

class Encoding E;

class Bob : Person { //reciever
public:
  std::pair<int, int> pub_key;
  int N;
  int phi_N;
  int Exponent;
  void setup(){
    //public stuff
    N = P * Q; 
    phi_N = totient(P, Q); // # of primes from 1 to 77
    Exponent = 7;// \in \mathbb{Z}_{phi_N}^*
    pub_key = std::make_pair(N, Exponent);
    //private stuff
    priv_key = mod_inverse(Exponent, phi_N); //P and Q are relatively prime
  }
  void decode(int ciphertext){
    Message = E.decode(exp_mod(ciphertext, priv_key, N));
  }
  //for printing/debugging only
  std::string print_P(){
    return std::to_string(P);
  }
  std::string print_Q(){
    return std::to_string(Q);
  }
  std::string print_key(){
    return std::to_string(priv_key);
  }
  std::string print_M(){
    return Message;
  }
private:
  int priv_key;
  int P = 7;
  int Q = 11;
  std::string Message;
};
  
class Alice : Person {
public:
  int ciphertext;
  void setup(std::pair<int, int> public_key){
    Message = "at";
    int N = public_key.first;//got from public domain
    int Exponent = public_key.second;//got from public domain
    ciphertext = exp_mod(E.encode(Message), Exponent, N);
  }
  //for printing/debugging only
  std::string print_M(){
    return Message;
  }
private:
  std::string Message;
  int priv_key = 0;
};

int main(){
  std::srand(std::time(nullptr));
  Bob B;
  Alice A;
  B.setup();
  std::cout << "In Bob's locked safe: (P = " << B.print_P() << ", Q = " << B.print_Q() << ")" << std::endl;
  std::cout << "Bob's Private Key: " << B.print_key() << std::endl;
  std::cout << "In the Public Domain: (N = " << B.pub_key.first << ", E = "<< B.pub_key.second << ")" << std::endl;
  A.setup(B.pub_key);
  std::cout << "Alice's Private Message: \"" << A.print_M() << "\"" << std::endl; 
  std::cout << "Alice's Public Ciphertext: " << A.ciphertext << std::endl;
  B.decode(A.ciphertext);
  std::cout << "Bob's Decoded Message: \"" << B.print_M() << "\"" << std::endl;
  return 0;
}
