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
    /*std::string num1 = value;
    std::string num2 = n.get_value();
    int len1 = num1.size(); 
    int len2 = num2.size(); 
    if (len1 == 0 || len2 == 0) 
      return num("0"); 
  
    // will keep the result number in vector 
    // in reverse order 
    std::vector<int> result(len1 + len2, 0); 
  
    // Below two indexes are used to find positions 
    // in result.  
    int i_n1 = 0;  
    int i_n2 = 0;  
      
    // Go from right to left in num1 
    for (int i=len1-1; i>=0; i--) 
    { 
        int carry = 0; 
        int n1 = num1[i] - '0'; 
  
        // To shift position to left after every 
        // multiplication of a digit in num2 
        i_n2 = 0;  
          
        // Go from right to left in num2              
        for (int j=len2-1; j>=0; j--) 
        { 
            // Take current digit of second number 
            int n2 = num2[j] - '0'; 
  
            // Multiply with current digit of first number 
            // and add result to previously stored result 
            // at current position.  
            int sum = n1*n2 + result[i_n1 + i_n2] + carry; 
  
            // Carry for next iteration 
            carry = sum/10; 
  
            // Store result 
            result[i_n1 + i_n2] = sum % 10; 
  
            i_n2++; 
        } 
  
        // store carry in next cell 
        if (carry > 0) 
            result[i_n1 + i_n2] += carry; 
  
        // To shift position to left after every 
        // multiplication of a digit in num1. 
        i_n1++; 
    } 
  
    // ignore '0's from the right 
    int i = result.size() - 1; 
    while (i>=0 && result[i] == 0) 
    i--; 
  
    // If all were '0's - means either both or 
    // one of num1 or num2 were '0' 
    if (i == -1) 
      return num("0"); 
  
    // generate the result string 
    std::string s = ""; 
      
    while (i >= 0) 
        s += std::to_string(result[i--]); 
  
    return s;*/
    
    std::string other = n.get_value();
    int len2 = other.size(); 
    if (len == 0 || len2 == 0) 
      return num("0"); 
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
      int sub = ((str1[i]-'0') - carry); 
      if (i>0 || sub>0) // remove preceding 0's 
	str.push_back(sub+'0'); 
      carry = 0;   
    }  
    return clean(rev(str)); 
  }
  num operator / (const num &n) const {
    num other = n;
    int count = 0;
    while(*this >= other){
      count++;
      other = other + n;
    }
    return count;
  }
  num operator % (const num &n) const {
    //wont work nicely with negatives
    num here = *this;
    if(*this < n) return *this;
    while(here >= n){
      here = here - n;
    }
    return here;
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
  bool is_odd(){
    return (value[len - 1] % 2 == 1);
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
