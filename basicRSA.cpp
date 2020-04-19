#include <iostream>
#include <ctime>
#include "util.h"
#include "encoding.h"
#include "person.h"
#include "number.h"

class Encoding E;

class Receiver : Person { 
public:
  Receiver(){}
  std::pair<num, num> pub_key;
  num N;
  num phi_N;
  num Exponent;
  void setup(){
    //public stuff
    N = P * Q; 
    phi_N = totient(P, Q); // # of primes from 1 to 77
    Exponent = num(7);// \in \mathbb{Z}_{phi_N}^*
    pub_key = std::make_pair(N, Exponent);
    //private stuff
    priv_key = mod_inverse(Exponent, phi_N); //P and Q are relatively prime
  }
  void decode(num ciphertext){
    Message = E.decode(exp_mod(ciphertext, priv_key, N));
  }
  //for printing/debugging only
  std::string print_P(){
    return P.print();
  }
  std::string print_Q(){
    return P.print();
  }
  std::string print_key(){
    return P.print();
  }
  std::string print_M(){
    return Message;
  }
private:
  num priv_key;
  //P & Q should be sufficiently large primes
  num P = num(139);
  num Q = num(251);
  
  std::string Message;
};
  
class Sender : Person {
public:
  Sender(){};
  num ciphertext;
  void setup(std::pair<num, num> public_key){
    Message = "att";
    num N = public_key.first;//got from public domain
    num Exponent = public_key.second;//got from public domain
    ciphertext = exp_mod(E.encode(Message) % N, Exponent, N);
  }
  //for printing/debugging only
  std::string print_M(){
    return Message;
  }
private:
  std::string Message;
  num priv_key = num(0);
};

int main(){
  std::srand(std::time(nullptr));
  Receiver Bob;
  Sender Alice;
  Bob.setup();
  std::cout << "In Bob's locked safe: (P = " << Bob.print_P() << ", Q = " << Bob.print_Q() << ")" << std::endl;
  std::cout << "Bob's Private Key: " << Bob.print_key() << std::endl;
  std::cout << "In the Public Domain: (N = " << Bob.pub_key.first.print() << ", E = "<< Bob.pub_key.second.print() << ")" << std::endl;
  Alice.setup(Bob.pub_key);
  std::cout << "Alice's Private Message: \"" << Alice.print_M() << "\"" << std::endl; 
  std::cout << "Alice's Public Ciphertext: " << Alice.ciphertext.print() << std::endl;
  Bob.decode(Alice.ciphertext);
  std::cout << "Bob's Decoded Message: \"" << Bob.print_M() << "\"" << std::endl;
  return 0;
}
