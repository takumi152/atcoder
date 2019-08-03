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

  int M, N, K;
  cin >> M >> N >> K;
  vector<int> x(N);
  for (auto &a: x) cin >> a;

  vector<int> bucket(M+1);
  for (auto a: x) bucket[a]++;
  set<int> xSet;
  for (auto a: x) xSet.insert(a);
  int maxCount = 0;
  // i: target
  // j: movement
  for (int i = 1; i <= M; i++) {
    int count = bucket[i];
    for (int j = 1; j <= K; j++) {
      int l = i - j;
      int r = i + j;
      auto itl = xSet.find(l);
      auto itr = xSet.find(r);
      if (itl != xSet.end() || itr != xSet.end()) count++;
    }
    if (count > maxCount) maxCount = count;
  }

  cout << maxCount << endl;

  return 0;
}
