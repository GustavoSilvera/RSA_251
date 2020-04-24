#include <iostream>
#include <ctime>
#include "util.h"
#include "encoding.h"
#include "person.h"
#include "number.h"
#include "unit.h"

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
    phi_N = totient(P, Q);// # of primes from 1 to 77
    std::cout << "Tot_N = " << phi_N.print() << ")" << std::endl;
    Exponent = num(7);// \in \mathbb{Z}_{phi_N}^*    
    std::cout << "In the Public Domain: (N = " << N.print() << ", E = "<< Exponent.print() << ")" << std::endl;
    pub_key = std::make_pair(N, Exponent);
    //private stuff
    priv_key = mult_inv_rec(Exponent, phi_N); //P and Q are relatively prime
    std::cout << "Bob's Private Key: " << priv_key.print() << std::endl;
  }
  void decode(num ciphertext){
    num big = exp_mod(ciphertext, priv_key, N);
    std::cout << "Bob's Decoding: " << big.print() << std::endl;
    Message = E.decode(big);
    std::cout << "Bob's Decoded Message: \"" << Message << "\"" << std::endl;
  }
private:
  num priv_key;
  //P & Q should be sufficiently large primes
  //num P = num(11);
  //num Q = num(7);
  num P = num("435958568325940791799951965387214406385470910265220196318705482144524085345275999740244625255428455944579");
  num Q = num("562545761726884103756277007304447481743876944007510545104946851094548396577479473472146228550799322939273");

  std::string Message;
};
  
class Sender : Person {
public:
  Sender(){};
  num ciphertext;
  void setup(std::pair<num, num> public_key){
    Message = "at";
    std::cout << "Alice's Private Message: \"" << Message << "\"" << std::endl; 
    num N = public_key.first;//got from public domain
    num Exponent = public_key.second;//got from public domain
    num encoded = E.encode(Message) % N;
    std::cout << "Alice's Encoding: " << encoded.print() << std::endl;
    //ciphertext = exp_mod(encoded, Exponent, N);
    ciphertext = num("163077576587089932277514178989798339755826189700674110151160860819557757512053108465634676999401755817425637794522932574265893488854028596522889419543378155476439015236106447427921542963150735762104095795184542");//from 251
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
  unit_test();
  std::cout << "All Unit Tests Passed Successfully!" << std::endl;
  Bob.setup();
  Alice.setup(Bob.pub_key);
  Bob.decode(Alice.ciphertext);
  return 0;
}
