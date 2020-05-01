#ifndef ENCODING
#define ENCODING
#include <string>
#include "util.h"
#include "number.h"

class Encoding {//base 27 encoding
 public:
  std::string decode(num C){
    if (C < num(0)) return "";//empty string
    std::string ret;
    while(C >= num(27)){
      std::pair<num, num> C27_divmod = C.divmod(num(27));
      num d = C27_divmod.second;
      C = C27_divmod.first;
      ret = ret + num_to_char(d);
    }
    ret += num_to_char(C);
    return reverse(ret);
  }
  num encode(std::string M){
    num ret = num(0);
    num M_len = num(M.size());
    if(M_len == num(0)) return num(-1);//empty string
    for(int i = 0; num(i) < M_len; i++){//kept i as int
      char c = M[i];
      num enc = num(0);//from ascii to our weird encoding
      if(c == ' ') enc = num(0);// space = 0
      else enc = num(int(c) - 97 + 1); // (a -> z) lowercase
      ret = ret + enc * (exp(num(27), M_len - num(i) - num(1)));
    }
    return ret;
  }
 private:
  inline char num_to_char(num d){
    int n = d.get_int();
    if(n == 0) return ' ';
    return char(n + 'a' - 1);
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
};


#endif
