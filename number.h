#ifndef NUM
#define NUM

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class num {
 public:
  num(){
    value = "0";
    sign = 0;
    len = 1;
  };
  num(const std::string &s){
    num_init(s);
  };
  num(const int32_t a) {
    num_init(std::to_string(a));
  };
  num( const num &n ){
    value = n.get_value();//need for constexpr errors
    sign = n.get_sign();
    len = value.size();
  };
  void num_init(const std::string &s){
    if(s[0] == '-'){
      sign = -1;
      value = clean(s.substr(1));//not including '-'
    }
    else{
      sign = 1;
      value = clean(s);
    }
    if(value == "0") sign = 0;
    len = value.size();
  }
  num operator + (const num &n) const {//O(n)
    if(n.is_neg()) return (*this - n.to_positive());
    if((*this).is_neg()){
      if(n.is_neg()){
	return ((*this).to_positive() + n.to_positive()).to_negative();
      }
      return (n + (*this));
    }
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
  num operator * (const num &n) const {
    std::string a = value;
    std::string b = n.get_value();
    int len1 = len;
    int len2 = b.size();
    if(len1 == 0 || len2 == 0) return num(0);
    if((*this) == num(0) || n == num(0)) return num(0);
    if((*this) == num(1)) return n;
    if(n == num(1)) return num(value);
    std::vector<int> result(len1 + len2, 0);
    //const int arrlen = len1 + len2;
    //int* result = new int[arrlen];
    for( int i = len1 - 1; i >= 0; i-- ){
      for( int j = len2 - 1; j >= 0; j-- ){
	result[ i + j + 1 ] += ( b[ j ] - '0') * ( a[ i ] - '0' );
	//result[i+j+1] = 1;
      }
    }
    for( int i = len1 + len2 - 1; i >= 0; i-- ){
      if( result[ i] >= 10 ){
	result[ i - 1 ] += result[ i ] / 10;//carry
	result[ i ] %= 10;
      }
    }
    std::string ret;
    for( int i = 0; i < int(len1 + len2); i++ ){
      ret += (std::to_string(result[i]));
    }
    //std::cout << sign << " " << n.get_sign() << std::endl;
    if(sign != n.get_sign()){
      return num(clean(ret)).to_negative();
    }
    return num(clean(ret));    
  }
  num operator - (const num &n) const {
    if(n.is_neg()) return (*this) + n.to_positive();
    if(n > *this) return (n - *this).to_negative();
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
  num sdiv (const num &n) const {//division a/b where b is small
    int divisor = n.get_int();//requires n is a small int
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
    if(n == num(1)) return *this;
    if(n == num(0)) throw std::invalid_argument("Divide by 0");
    if(n == num(10)) return num(value.substr(0, len - 2));
    //if(n.to_positive() < num(2147483647)){
    //  if(sign != n.get_sign()) return ((*this).sdiv(n)).to_negative();
    //  return (*this).sdiv(n);//optimized for small nums
    //}
    //if(sign != n.get_sign()) return (binSearchDiv(*this,n)).to_negative();
    //return binSearchDiv(*this, n);
    if(sign != n.get_sign()) return ((*this).div(n)).to_negative();
    return (*this).div(n);
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
    if(n == num(1)) return (*this).is_odd() ? 0 : 1;//odds
    if(n == num(2)) return (*this).is_even() ? 0 : 1;//evens
    if(n == num(10))return num(value[len - 2]);
    return ((*this).divmod(n)).second;
  }
  std::pair<num, num> divmod (const num &n) const{
    num quotient = *this / n;
    num remainder = *this - quotient * n;
    return std::make_pair(quotient, remainder);
  }
  std::tuple<num, num> divide(const num& dividend, const num& divisor) const {
    num quotient, remainder, temp;
    temp = divisor;
    quotient = 1;
    while (temp < dividend) {
      quotient = quotient + num(1);
        temp = temp + divisor;
    }
    if (temp > dividend) {
      quotient = quotient - num(1);
      remainder = dividend - (temp - divisor);
    }
    return std::make_tuple(quotient, remainder);
  }
  num div(const num &n) const {
    num abs_dividend = (*this).to_positive();
    num abs_divisor = (n).to_positive();
    if (n == 0)
      throw std::logic_error("Attempted division by zero");
    if (abs_dividend < abs_divisor)
      return num(0);
    num quotient;
    const long long LLONG_MAX = 2147483647;
    if (abs_dividend <= LLONG_MAX && abs_divisor <= LLONG_MAX)
      quotient = num(std::stoll(abs_dividend.get_value()) / std::stoll(abs_divisor.get_value()));
    else if (abs_dividend == abs_divisor)
      quotient = 1;
    else {
      std::string ret = "";// the value is cleared as digits will be appended
      num chunk, chunk_quotient, chunk_remainder;
      size_t chunk_index = 0;
      chunk_remainder = abs_dividend.get_value().substr(chunk_index, abs_divisor.get_len() - 1);
      chunk_index = abs_divisor.get_len() - 1;
      while (chunk_index < abs_dividend.get_len()) {
	chunk = num((chunk_remainder.get_value()).append(1, abs_dividend.get_value()[chunk_index]));
	chunk_index ++;
	while (chunk < abs_divisor) {
	  quotient = num(quotient.get_value().append("0"));
	  if (chunk_index < abs_dividend.get_len()) {
	    chunk = num(chunk.get_value().append(1, abs_dividend.get_value()[chunk_index]));
	    chunk_index++;
	  }
	  else
	    break;
	}
	if (chunk == abs_divisor) {
	  quotient = num(quotient.get_value().append("1"));
	  chunk_remainder = num(0);
	}
	else if (chunk > abs_divisor) {
	  chunk = num(clean(chunk.get_value()));
	  std::tie(chunk_quotient, chunk_remainder) = divide(chunk, abs_divisor);
	  quotient = num(quotient.get_value() + chunk_quotient.get_value());
	}
      }
    }
    return num(clean(quotient.value));
  }
  //for large divisions, log(n) binary search
  num binSearchDiv(const num &dividend, const num &divisor) const {
    //if(dividend.get_len() < divisor.get_len()) return num(0);
    num l = num(0);
    num r = dividend.to_positive();
    while (l <= r) { 
      num m = (r + l).sdiv(num(2)); //2 is a small number
      num possible = m * divisor.to_positive();
      num possible_next = possible + divisor.to_positive();
      if (possible <= dividend.to_positive() && possible_next > dividend.to_positive())//best one 
	return m; 
      if (possible < dividend.to_positive()) 
	l = m;// + num(1);
      else
	r = m;// - num(1);
    }
    //error!
    return num(0);
  }
  
  //comparison operators
  bool operator == (const num &n) const{
    if(sign != n.get_sign()) return false;//must have same sign
    if(len != n.get_len()) return false;//same length (when cleaned)
    return (value.compare(n.get_value()) == 0);
  }
  bool operator < (const num &n) const{
    if(sign > n.get_sign()) return false;//positive < negative == false
    else if (sign < n.get_sign()) return true;//negative < positive == true
    if(len < n.get_len()) return true;
    else if (len > n.get_len()) return false;
    return (value.compare(n.get_value()) < 0);
  }
  bool operator > (const num &n) const{
    if(sign < n.get_sign()) return false;//negative > positive == false
    else if (sign > n.get_sign()) return true;//positive > negative == true
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
  bool is_odd() const{
    return (int(value[len - 1]) % 2 == 1);
  }
  bool is_even() const{
    return !(*this).is_odd();
  }
  bool is_neg() const{
    return (get_sign() == -1);
  }
  num to_positive() const {
    //technically the sign is not carried in the VALUE... which is always positive 
    return num(value);//not carrying over the sign, defaults to positive
  }
  num to_negative() const {
    std::string v = value;
    return num(v.insert(0, "-"));//prepending "-" to front of value
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
    std::string unsign = clean(value);
    if(sign < 0) return unsign.insert(0, "-");
    return unsign;
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

template <typename T>
T & operator<<(T &s, num n) {
  return s << n.print();
}


#endif
