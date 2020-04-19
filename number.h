#ifndef NUM
#define NUM

#include <iostream>

class num {
 public:
  num(){
    value = 0;
  };
  num(size_t a) : value(a) {};
  constexpr num( const num& n ) = default;//need for constexpr errors
  
  num operator + (const num &n) const {
    return num(value + n.get_value());
  }
  num operator * (const num &n) const {
    return num(value * n.get_value());
  }
  num operator - (const num &n) const {
    return num(value - n.get_value());
  }
  num operator / (const num &n) const {
    return num(value / n.get_value());
  }
  num operator % (const num &n) const {
    return num(value % n.get_value());
  }
  //comparison operators
  bool operator == (const num &n) const{
    return (value == n.get_value());
  }
  bool operator < (const num &n) const{
    return (value < n.get_value());
  }
  bool operator > (const num &n) const{
    return (value > n.get_value());
  }
  bool operator <= (const num &n) const{
    return (value <= n.get_value());
  }
  bool operator >= (const num &n) const{
    return (value >= n.get_value());
  }
  //mutation operators
  void operator = (const num &n){
    value = n.get_value();
  }
  //getters
  int get_value() const {
    return int(value);
  }
  std::string print() const {
    return std::to_string(value);
  }
   
 private:
  int value;
};

#endif
