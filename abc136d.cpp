#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <string>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string S;
  cin >> S;

  int n = S.size();
  vector<int> bucket(n);
  int e = 0, o = 0;
  int point = 0;
  bool rnow = true;
  for (int i = 0; i < n; i++) {
    if (rnow && S[i] == 'L') {
      point = i;
      rnow = false;
    }
    else if (!rnow && S[i] == 'R') {
      if (point % 2 == 0) {
        bucket[point-1] = e;
        bucket[point] = o;
      }
      else {
        bucket[point-1] = o;
        bucket[point] = e;
      }
      e = 0;
      o = 0;
      rnow = true;
    }
    if (i % 2 == 0) o++;
    else e++;
  }
  if (point % 2 == 0) {
    bucket[point-1] = e;
    bucket[point] = o;
  }
  else {
    bucket[point-1] = o;
    bucket[point] = e;
  }

  for (int i = 0; i < n; i++) {
    cout << bucket[i];
    if (i < n-1) cout << " ";
    else cout << endl;
  }

  return 0;
}
