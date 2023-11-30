#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
std::vector<T> numberToDigits(T num) {
  if (!num) {
    return {0U};
  }

  std::vector<T> result;
  while (num) {
    result.emplace_back(num % 10U);
    num /= 10U;
  }

  return result;
}

template <typename T>
std::size_t digitsToNumber(const std::vector<T> &digits) {
  if (digits.empty()) {
    return 0U;
  }

  std::size_t number{0U};
  for(auto rIt{std::rbegin(digits)}; rIt != std::rend(digits); ++rIt) {
    number *= 10U;
    number += *rIt;
  }

  return number;
}

template <typename T>
T nextLargest(T num) {
  // break down a number into a vector of digits
  auto digits{numberToDigits(num)};

  // look for the first element that has a drop in values
  const auto it{std::adjacent_find(digits.begin(), digits.end(),
                                   [](T a, T b) { return a > b; })};
  if (it == digits.end()) {
    // this is the largest possible number already
    return num;
  }

  // all digits starting pivot and below, will need reordering
  const auto pivotPos{it + 1U};
  std::vector<T> digitsToReorder{digits.begin(),
                                 pivotPos + 1U}; // as if passing end()

  // get from those digits the ones larger than the pivot as candidates
  // (There must be one digit at least -> The element right before the Pivot)
  std::vector<T> largerThanPivot;
  std::copy_if(digitsToReorder.begin(), digitsToReorder.end(),
               std::back_inserter(largerThanPivot),
               [pivot = *pivotPos](auto a) { return a > pivot; });

  // get the smallest digit of them,
  // since  we're looking for the very next possible number
  const auto pivotReplacement{
      *std::min_element(largerThanPivot.begin(), largerThanPivot.end())};

  // now, place it at the last position of digits to reorder,
  // and sort the remaining elements ascendingly
  digitsToReorder.erase(std::find(digitsToReorder.begin(),
                                  digitsToReorder.end(), pivotReplacement));

  digitsToReorder.emplace_back(pivotReplacement);

  std::sort(digitsToReorder.begin(),
            digitsToReorder.end() - 1U, // to skip the pivot replacement
            std::greater{});

  // lastly, replace those elements with the reordered ones
  std::copy(digitsToReorder.begin(), digitsToReorder.end(), digits.begin());

  return digitsToNumber(digits);
}

int main() {

  // driver code
  std::cout << nextLargest(1234U) << std::endl;
  std::cout << nextLargest(15942U) << std::endl;
  std::cout << nextLargest(1594655432U) << std::endl;

  return 0;
}
