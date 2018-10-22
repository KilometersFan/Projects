#include <string>
#include <locale>

std::string lowerCase(std::string str){
  std::string copy = str;
  std::locale loc;
  for (unsigned int i = 0; i < copy.length(); i++)
    copy[i] = std::tolower(copy[i], loc);
  return copy;
} 
int getSum(std::string str){
  int sum = 0;
  for(unsigned int i = 0; i < str.length(); i++){
    int charVal = static_cast<int> (str[i] - '0');
    if(charVal >=0 && charVal <= 9)
      sum+=charVal;
    else if (charVal >= 49 && charVal <= 74)
      sum += (charVal - 39);
    else 
      sum += 0;
  }
  return sum;
}
struct AlphaStrComp {
  bool operator() (const std::string& lhs, const std::string& rhs)
  { // Uses string's built in operator<
    // to do lexicographic (alphabetical) comparison
    return (lhs < rhs);
  }
};

struct LengthStrComp {
  bool operator()(const std::string& lhs, const std::string& rhs)
  { // Compares strings by size;
    // Breaks ties by lexicographic comparison
    return (   (lhs.size() < rhs.size())
            || (lhs.size() == rhs.size() && lhs < rhs));
  }
};

struct NumStrComp {
  bool operator()(const std::string& lhs, const std::string& rhs){
    std::string lhsCopy = lowerCase(lhs);
    std::string rhsCopy = lowerCase(rhs);
    int lhsSum = getSum(lhsCopy);
    int rhsSum = getSum(rhsCopy);
    if(lhsSum == rhsSum)
      return (lhs < rhs);
    return (lhsSum < rhsSum);
  }
};

struct NumPairFirstComp {
  bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs){
    return (lhs.first > rhs.first);
  }
};

struct NumPairSecondComp {
  bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs){
    return (lhs.second > rhs.second);
  }
};