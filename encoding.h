#ifndef ENCODING
#define ENCODING
#include <string>
#include "util.h"

class Encoding {
 public:
  std::string decode(int C){
    std::string ret;
    while(C >= 27){
      int d = C % 27;
      C /= 27;
      ret += int_to_char(d);
    }
    ret += int_to_char(C);
    return reverse(ret);
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
 private:
  inline char int_to_char(int d){
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
};


#endif
