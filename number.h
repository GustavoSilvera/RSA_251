#ifndef NUM
#define NUM

#include <string>
#include <vector>

class num {
 public:
  num(){
    value = "0";
    sign = 0;
    len = 1;
  };
  num(const int32_t a) {
    value = std::to_string(a);
    if(a > 0) sign = 1;
    else if (a < 0) sign = -1;
    else sign = 0;
    len = value.size();
  };
  num(const std::string &s){
    if(s[0] == '-'){
      sign = -1;
      value = clean(s.substr(1));//not including '-'
    }
    else{
      sign = 1;
      value = clean(s);
    }
    len = value.size();
  };
  num( const num &n ){
    value = n.get_value();//need for constexpr errors
    sign = n.get_sign();
    len = value.size();
  };
  
  num operator + (const num &n) const {//O(n)
    std::string str1 = value;
    std::string str2 = n.get_value();
    if (str1.length() > str2.length()) 
      std::swap(str1, str2); 
    std::string str = ""; 
    int n1 = str1.length(), n2 = str2.length(); 
    int diff = n2 - n1; 
    int carry = 0; 
    for (int i=n1-1; i>=0; i--) {
        int sum = ((str1[i]-'0') + (str2[i+diff]-'0') + carry); 
        str.push_back(sum%10 + '0'); 
        carry = sum/10; 
    } 
    for (int i=n2-n1-1; i>=0; i--) { 
        int sum = ((str2[i]-'0')+carry); 
        str.push_back(sum%10 + '0'); 
        carry = sum/10; 
    } 
    if (carry != 0) str.push_back(carry+'0'); 
    return clean(rev(str)); 
  }
  
  num operator * (const num &n) const {//O(n^2)
    std::string other = n.get_value();
    int len2 = other.size(); 
    if (len == 0 || len2 == 0) return num("0"); 
    std::vector<int> result(len + len2, 0); 
    int i_n1 = 0;  
    int i_n2 = 0;  
    for (int i = len - 1; i >= 0; i--){ 
        int carry = 0; 
        int n1 = value[i] - '0'; 
        i_n2 = 0;  
        for (int j = len2 - 1; j >= 0; j--){ 
            int n2 = other[j] - '0'; 
            int sum = n1 * n2 + result[i_n1 + i_n2] + carry; 
            carry = sum / 10; 
            result[i_n1 + i_n2] = sum % 10; 
	    i_n2++; 
        } 
	if (carry > 0) result[i_n1 + i_n2] += carry; 
        i_n1++; 
    } 
    int i = result.size() - 1; 
    while (i >= 0 && result[i] == 0) i--; //ignoring 0's
    if (i == -1) //one must have been all 0's
      return num("0"); 
    std::string ret = ""; 
    while (i >= 0) ret += std::to_string(result[i--]); 
    return clean(ret);
  }
  num operator - (const num &n) const {
    std::string str1 = value;
    std::string str2 = n.get_value();
    if (str1.size() < str2.size()) swap(str1, str2); 
    std::string str = ""; 
    int n1 = str1.length(), n2 = str2.length(); 
    int diff = n1 - n2; 
    int carry = 0; 
    for (int i=n2-1; i>=0; i--) { 
      int sub = ((str1[i+diff]-'0') - (str2[i]-'0') - carry); 
      if (sub < 0) { 
	sub = sub+10; 
	carry = 1; 
      } 
      else carry = 0;
      str.push_back(sub + '0'); 
    } 
    for (int i=n1-n2-1; i>=0; i--) { 
      if (str1[i]=='0' && carry) { 
	str.push_back('9'); 
	continue; 
      } 
      int sub = ((str1[i] - '0') - carry); 
      if (i > 0 || sub>0) // remove preceding 0's 
	str.push_back(sub + '0'); 
      carry = 0;   
    }  
    return clean(rev(str)); 
  }
  num sdiv (const num &n) const {
    int divisor = n.get_int();
    std::string number = value;
    std::string ans; 
    size_t idx = 0; 
    int temp = value[idx] - '0'; 
    while (temp < divisor) temp = temp * 10 + (number[++idx] - '0'); 
    while (number.size() > idx) { 
      ans += (temp / divisor) + '0'; 
      temp = (temp % divisor) * 10 + number[++idx] - '0'; 
    } 
    if (ans.length() == 0) return num("0"); 
    return num(ans); 
  }
  num operator / (const num &n) const {
    //if(n < num(2147483647)) return (*this).sdiv(n);
    //num other = n;
    //int count = 0;
    return binSearchDiv(*this, n);
      /*while(*this >= other){
      count++;
      other = other + n;
    }
    return count;*/
  }
  num smod (const num &n) const {
    //for modding a big num with a small num
    int m = n.get_int();//should be small enough
    num res = num(0); 
    for (size_t i = 0; i < len; i++) 
      res = num((res.get_int() * 10 + (int(value[i]) - '0')) % m); 
    return res; 
  }
  num operator % (const num &n) const {
    //if(n < num(2147483647)) return (*this).smod(n);
    //if(len < n.get_len()) return *this;//much smaller
    //wont work nicely with negatives
    //for big bois
    num quotient = *this / n;
    num remainder = *this - quotient * n;
    return remainder;
    //for small bois
    /*num here = *this;
    if(*this < n) return *this;
    while(here >= n){
      here = here - n;
    }
    return here;*/
  }
  //long bois

  num binSearchDiv(const num &dividend, const num &divisor) const {
    num l = num(0);
    num r = dividend;
    while (l <= r) { 
      num m = (r + l).sdiv(num(2)); //2 is a small number
      num possible = m * divisor;
      num possible_next = possible + divisor;
      if (possible <= dividend && possible_next > dividend)//best one 
	return m; 
      if (possible < dividend) 
	l = m + num(1);
      else
	r = m - num(1);
    }
    //error!
    throw std::invalid_argument("failed division!");
    return num(-1);
}
  
  //comparison operators
  bool operator == (const num &n) const{
    if(len != n.get_len()) return false;
    return (value.compare(n.get_value()) == 0);
  }
  bool operator < (const num &n) const{
    if(len < n.get_len()) return true;
    else if (len > n.get_len()) return false;
    return (value.compare(n.get_value()) < 0);
  }
  bool operator > (const num &n) const{
    if(len > n.get_len()) return true;
    else if (len < n.get_len()) return false;
    return (value.compare(n.get_value()) > 0);
  }
  bool operator >= (const num &n) const{
    return (*this > n) || (*this == n);
  }
  bool operator <= (const num &n) const{
    return (*this < n) || (*this == n);
  }
  bool is_odd(){
    return (int(value[len - 1]) % 2 == 1);
  }
  //mutation operators
  void operator = (const num &n){
    value = clean(n.get_value());
    len = value.size();
    sign = n.get_sign();
  }
  //getters
  std::string get_value() const{
    return clean(value);
  }
  int get_int() const {
    //used for debugging, use sparingly
    return std::stoi(value);
  }
  std::string print() const {
    return clean(value);
  }
  int get_sign() const {
    return sign;
  }
  size_t get_len() const{
    return len;
  }
 private:
  //private data
  std::string value;
  int sign;
  size_t len;
  //helper functions
  inline std::string rev(const std::string &r) const{
    const int strlen = r.size();
    std::string ret;
    for(int i = strlen - 1; i>= 0; i--){
      char c = r[i];
      ret += c;
    }
    return ret;
  }
  inline std::string clean(const std::string &r) const{
    const int strlen = r.size();
    int i = 0; 
    while(r[i] == '0' && i < strlen - 1){
      i++;
    }
    return r.substr(i);
  }
};

#endif
