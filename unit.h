#ifndef UNIT
#define UNIT
#include <cassert>
#include "number.h"
#include "util.h"

using namespace std;


void unit_test(){
  //testing addition
  assert(num(5) + num(7) == num(12));
  assert(num(50000) + num(7) == num(50007));
  assert(num(83687) + num(0) == num(83687));
  assert(num("846532168798") + num("7965213246") == num("854497382044"));
  assert(num(0) + num(0) == num(0));
  assert(num("1111111111") + num("9999999999") == num("11111111110"));
  //subtraction
  assert(num("846532168798") - num("796521324698") == num("50010844100"));
  assert(num("84653285885458762584515439") - num("84653285885458762584515184") == num("255"));
  //multiplication
  assert(num(5) * num(5) == num(25));
  assert(num(0) * num(836392) == num(0));
  assert(num(1) * num(36628) == num(36628));
  assert(num("836363") * num(62428) == num("52212469364"));
  assert(num("153357401966410801783542") * num("552") == num("84653285885458762584515184"));
  //division
  assert(num("836363") / num(62428) == num(13));
  assert(num("1546513549845132458") / num(1) == num("1546513549845132458"));
  assert(num(5) / num(2) == num(2));
  assert(num(1) / num(6) == num(0));
  assert(num("5221246936452212469364") / num("52212469364") == num("100000000001"));
  assert(num("84653285885458762584515439") / num(552) == num("153357401966410801783542"));
  
  //modular arithmetic
  assert(num(5) % num(5) == num(0));
  assert(num(77) % num(5) == num(2));
  assert(num(682) % num(36527) == num(682));
  assert(num(1) % num(2) == num(1));
  assert(num("264823645823640") % num(2) == num(0));
  assert(num("38776482348233") % num(2) == num(1));
  
  assert(num("84653285885458762584515439") % num(552) == num(255));
  assert(num("84653285885458762584515439") % num("94651987652168") == num("80245336176543"));

  //logic tests
  assert(num("76546876516") <= num("686516846168426"));
  assert(num("923746287342") > num("226"));
  assert(num("0") <= num("68426"));
  assert(num("9292929292") == num("9292929292"));
  assert(num("23947623942") < num("23947623943"));
  assert(num("7") > num("6"));
  assert(num("79") <= num("79"));
  assert(num("-79") <= num("77"));//removes the negative
  assert(num("-79").get_len() == num("77").get_len());//removes the negative
  
  
  //other tests
  assert(num("2876238645284231").is_odd());
  assert(!num("2876238645284230").is_odd());
  assert(!num("0").is_odd());
  assert(num("11").is_odd());

  //sign stuff
  assert(num(33) - num("-22") == num(55));
  assert(num(33) + num("-22") == num(11));
  assert(num(-1).to_positive() == num(1));
  assert(num(0).to_positive() == num(0));
  assert(num(0).to_negative() == num(0));
  assert(num(26).to_negative() == num("-26"));
  assert(num("-3947236").to_positive() == num(3947236));
  assert(num(1) - num(1)*num(-8) == num(9));
	
  
  //testing negatives
  assert(num("-973468273").is_neg());
  assert(num("-1").is_neg());
  assert(num(-2393).is_neg());
  assert(!num("0").is_neg());
  assert(!num(39742).is_neg());
  assert(!num(1).is_neg());
  assert(!num("4982392").is_neg());
	
  
  //testing multiplicative inverses
#define mod_inv modInverse
  assert(mod_inv(num(7), num(60)) == num(43));
  std::cout << std::endl;
  assert(mod_inv(num(3), num(26)) == num(9));
  assert(mod_inv(num(653), num(5846)) == num(2829));
  
}

#endif
