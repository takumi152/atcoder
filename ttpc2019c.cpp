#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;

int floor2(int x) {
  if (x == 0) return 0;
  int a = 1;
  int xx = x >> 1;
  while (xx > 0) {
    a <<= 1;
    xx >>= 1;
  }
  return a;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, X;
  cin >> N >> X;
  vector<int> a(N);
  for (auto &x: a) cin >> x;

  int xorTotal = 0;
  int missingCount = 0;
  for (auto &x: a) {
    if (x == -1) missingCount++;
    else xorTotal ^= x;
  }

  vector<int> numberToPut;
  if (xorTotal == X) {}
  else if ((xorTotal ^ X) <= X) {
    numberToPut.push_back(xorTotal ^ X);
    missingCount--;
  }
  else {
    if ((xorTotal ^ X ^ floor2(X)) > X) {
      cout << -1 << endl; // impossible
      return 0;
    }
    numberToPut.push_back(floor2(X));
    numberToPut.push_back(xorTotal ^ X ^ floor2(X));
    missingCount -= 2;
  }
  if (missingCount < 0) {
    cout << -1 << endl; // impossible
    return 0;
  }
  while (missingCount > 0) {
    numberToPut.push_back(0);
    missingCount--;
  }

  int pointer = 0;
  vector<int> ans(N);
  for (int i = 0; i < N; i++) {
    if (a[i] == -1) {
      ans[i] = numberToPut[pointer];
      pointer++;
    }
    else ans[i] = a[i];
  }

  for (int i = 0; i < N; i++) {
    cout << ans[i];
    if (i < N-1) cout << " ";
    else cout << endl;
  }


  return 0;
}
