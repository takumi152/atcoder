#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

using namespace std;

typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> A;
  for (int i = 0; i < N; i++) {
    int buf;
    cin >> buf;
    A.push_back(buf);
  }
  map<int, int> cardCount;
  for (int i = 0; i < N; i++) {
    if (cardCount.find(A[i]) != cardCount.end()) cardCount[A[i]] += 1;
    else cardCount[A[i]] = 1;
  }
  auto left = cardCount.begin();
  auto right = cardCount.rbegin();
  while (left != cardCount.end() || right != cardCount.rend()) {
    while (left != cardCount.end()) {
      if (left->second <= 1)  left++;
      else break;
    }
    while (right != cardCount.rend()) {
      if (right->second <= 1)  right++;
      else break;
    }
    if (left == cardCount.end() || right == cardCount.rend()) break;
    left->second -= 1;
    right->second -= 1;
  }
  int cardLeft = 0;
  for (const auto& x : cardCount) cardLeft += x.second;
  cout << cardLeft << endl;
  return 0;
}
