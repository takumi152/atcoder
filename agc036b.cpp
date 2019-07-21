#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll N, K;
  cin >> N >> K;
  vector<int> A(N);
  for (auto &x: A) cin >> x;

  vector<set<int> > occurence(200001);
  for (int i = 0; i < N; i++) {
    occurence[A[i]].insert(i);
  }
  set<int> leftoverSet;
  vector<int> leftoverVector;
  int pointer = -1;
  while (true) {
    if (pointer >= 0) {
      int currentNum = A[pointer];
      pointer = *(occurence[currentNum].lower_bound(0)); // find first occurence / next starting point
    }
    pointer++;
    auto it = leftoverSet.find(pointer);
    if (it != leftoverSet.end()) break;
    leftoverSet.insert(pointer);
    leftoverVector.push_back(pointer);
    while (pointer < N) {
      int currentNum = A[pointer];
      auto it = occurence[currentNum].upper_bound(pointer);
      if (it == occurence[currentNum].end()) break;
      else pointer = (*it) + 1;
    }
    if (pointer == N) pointer = -1;
  }
  ll target = 0;
  if (K <= leftoverVector.size()) target = K-1;
  else {
    ll start = 0;
    while (pointer != leftoverVector[start]) start++;
    ll end = leftoverVector.size();
    ll remaining = K - end - 1;
    ll remainder = remaining % (end - start);
    target = start + remainder;
  }

  pointer = leftoverVector[target];
  vector<int> ans;
  while (pointer < N) {
    int currentNum = A[pointer];
    auto it = occurence[currentNum].upper_bound(pointer);
    if (it == occurence[currentNum].end()) {
      ans.push_back(currentNum);
      pointer++;
    }
    else pointer = (*it) + 1;
  }

  for (int i = 0; i < ans.size(); i++) {
    cout << ans[i];
    if (i < ans.size() - 1) cout << " ";
    else cout << endl;
  }

  return 0;
}
