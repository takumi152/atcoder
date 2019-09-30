#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, K;
  cin >> N >> K;
  vector<int> P(N);
  for (auto &x: P) cin >> x;

  /*
  0 1 2 3 7 5 4 6 8 9
  0 1 2 3
    1 2 3 7
      2 3 5 7
        3 4 5 7
          4 5 6 7
            4 5 6 8
              4 6 8 9
  */

  vector<int> partialVector;
  set<int> partialSet;
  for (int i = 0; i < K; i++) {
    partialVector.push_back(P[i]);
    partialSet.insert(P[i]);
  }
  sort(partialVector.begin(), partialVector.end());
  bool sorted = true;
  for (int i = 0; i < K; i++) {
    if (partialVector[i] != P[i]) {
      sorted = false;
      break;
    }
  }
  int decBase = 0;
  for (int i = 0; i < K-1; i++) {
    if (P[i] > P[i+1]) {
      decBase = i + 1;
    }
  }
  int ans = 0;
  if (!sorted) ans++;
  for (int i = K; i < N; i++) {
    decBase = max(0, decBase-1);
    if (P[i-1] > P[i]) {
      decBase = K - 1;
    }
    bool front = false, back = false;
    if (*(partialSet.begin()) == P[i-K]) front = true;
    partialSet.erase(P[i-K]);
    partialSet.insert(P[i]);
    if (*(partialSet.rbegin()) == P[i]) back = true;
    if (decBase > 0){
      if (front && back) ;
      else ans++;
    }
    else {
      if (back) sorted = true;
      else ans++;
    }
    //cerr << decBase << " " << ans << endl;
  }
  if (sorted) ans++;

  cout << ans << endl;

  return 0;
}
