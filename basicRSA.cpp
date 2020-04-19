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
    std::cout << "In Bob's locked safe: (P = " << P.print() << ", Q = " << Q.print() << ", ";
    N = P * Q;
    phi_N = totient(P, Q); // # of primes from 1 to 77
    std::cout << "Tot_N = " << phi_N.print() << ")" << std::endl;
    Exponent = num(7);// \in \mathbb{Z}_{phi_N}^*
    Exponent = Exponent % phi_N;
    
    std::cout << "In the Public Domain: (N = " << N.print() << ", E = "<< Exponent.print() << ")" << std::endl;
    pub_key = std::make_pair(N, Exponent);
    //private stuff
    priv_key = mod_inverse_in(Exponent, phi_N); //P and Q are relatively prime
    std::cout << "Bob's Private Key: " << priv_key.print() << std::endl;
  }
  void decode(num ciphertext){
    Message = E.decode(exp_mod(ciphertext, priv_key, N));
    std::cout << "Bob's Decoded Message: \"" << Message << "\"" << std::endl;
    //std::cout << "C = " << ciphertext.print() << " E = " << 
    //Message = E.decode(exp_mod(num(75), num(7), num(77)));
  }
private:
  num priv_key;
  //P & Q should be sufficiently large primes
  num P = num(3607);
  num Q = num(3943);
  //num P = num("435958568325940791799951965387214406385470910265220196318705482144524085345275999740244625255428455944579");
  //num Q = num("562545761726884103756277007304447481743876944007510545104946851094548396577479473472146228550799322939273");

  std::string Message;
};
  
class Sender : Person {
public:
  Sender(){};
  num ciphertext;
  void setup(std::pair<num, num> public_key){
    Message = "got";
    std::cout << "Alice's Private Message: \"" << Message << "\"" << std::endl; 
    num N = public_key.first;//got from public domain
    num Exponent = public_key.second;//got from public domain
    ciphertext = exp_mod(E.encode(Message) % N, Exponent, N);
    std::cout << "Alice's Public Ciphertext: " << ciphertext.print() << std::endl;
  }
private:
  std::string Message;
  num priv_key = num(0);
};

int main(){
  std::srand(std::time(nullptr));
  Receiver Bob;
  Sender Alice;
  std::cout << "Starting" << std::endl;
  Bob.setup();
  Alice.setup(Bob.pub_key);
  Bob.decode(Alice.ciphertext);
  return 0;
}
