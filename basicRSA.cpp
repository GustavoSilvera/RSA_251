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
  void setup(num p, num q){
    //public stuff
    P = p;
    Q = q;
    std::cout << "In Bob's locked safe:" << std::endl << "P = " << P.print() << std::endl << "Q = " << Q.print() << std::endl;
    N = P * Q;
    phi_N = totient(P, Q);// # of primes from 1 to 77
    std::cout << "tot_N = " << phi_N.print() << "" << std::endl;
    Exponent = num(7);// \in \mathbb{Z}_{phi_N}^*    
    std::cout << "In the Public Domain:" << std::endl << "N = " << N.print() << std::endl <<  "E = "<< Exponent.print()  << std::endl;
    pub_key = std::make_pair(N, Exponent);
    //private stuff
    priv_key = modInverse(Exponent, phi_N); //P and Q are relatively prime
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
  num P;
  num Q;
  std::string Message;
};
  
class Sender : Person {
public:
  Sender(){};
  num ciphertext;
  void setup(std::string M, std::pair<num, num> public_key, std::string C){
    if(M != ""){
      Message = M;
      std::cout << "Alice's Private Message: \"" << Message << "\"" << std::endl; 
      num N = public_key.first;//got from public domain
      num Exponent = public_key.second;//got from public domain
      num encoded = E.encode(Message) % N;
      std::cout << "Alice's Encoding: " << encoded.print() << std::endl;
      ciphertext = exp_mod(encoded, Exponent, N);
    }
    else ciphertext = C;//last resort
    std::cout << "Alice's Public Ciphertext: " << ciphertext.print() << std::endl;
  }
  //  ciphertext = num("");//from 251
private:
  std::string Message;
  num priv_key = num(0);
};

bool contains(char* a[], int c, std::string s){
  for(int i = 0; i < c; i++){
    if(std::string(a[i]) == s)
      return true;
  }
  return false;
}

int main(int argc, char* argv[]){
  std::srand(std::time(nullptr));
  Receiver Bob;
  Sender Alice;
  std::cout << "Starting" << std::endl;
  unit_test();
  std::cout << "All Unit Tests Passed Successfully!" << std::endl;
  std::cout << "NOTE: use flag \"PQ\" to directly input P and Q" << std::endl << "      use flag \"cipher\" to directly input Alice's ciphertext" << std::endl << "      use flag \"message\" to directly input Alice's message" << std::endl << "      use flag \"c\" to just use the default (251) ciphertext" << std::endl;
  std::string P = "435958568325940791799951965387214406385470910265220196318705482144524085345275999740244625255428455944579";
  std::string Q = "562545761726884103756277007304447481743876944007510545104946851094548396577479473472146228550799322939273";
  std::string M = "hello there";
  std::string C = "163077576587089932277514178989798339755826189700674110151160860819557757512053108465634676999401755817425637794522932574265893488854028596522889419543378155476439015236106447427921542963150735762104095795184542";
  if(contains(argv, argc, "PQ")){
    std::cout << "Input 'P'" << std::endl;
    std::getline(std::cin, P);
    std::cout << "Input 'Q'" << std::endl;
    std::getline(std::cin, Q);
  }
  if(contains(argv, argc, "cipher")){
    std::cout << "Input Alice's Cipher:" << std::endl;
    std::getline(std::cin, C);
    M = "";
  }
  if(contains(argv, argc, "message")){
    std::cout << "Input Alice's Message:" << std::endl;
    std::getline(std::cin, M);
    C = "";
  }
  if(contains(argv, argc, "c")){
    M = "";//defaults to using ciphertext
  }
  Bob.setup(P, Q);
  Alice.setup(M, Bob.pub_key, C);
  Bob.decode(Alice.ciphertext);
  
  return 0;
}
