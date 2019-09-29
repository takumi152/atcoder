// WIP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, L, X;
  cin >> N >> L >> X;
  vector<int> A(N);
  for (auto &x: A) cin >> x;

  set<int> fold, real, dist;
  fold.insert(min(X, L-X));
  real.insert(X);
  dist.insert(X);
  for (auto &x: A) {
    int fx = min(x, L-x);
    if (real.find(x) != real.end()) {
      real.insert(x);
      if (fold.find(fx) != fold.end()) {

      }
      else fold.insert(fx);
    }
    else {
      real.erase(x);
    }
  }

  return 0;
}
