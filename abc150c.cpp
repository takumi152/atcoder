#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> p(n), q(n);
  for (auto &x: p) cin >> x;
  for (auto &x: q) cin >> x;

  vector<int> a(n);
  for (int i = 0; i < n; i++) a[i] = i+1;
  int c = 0;
  int pc = 0;
  int qc = 0;
  do {
    if (a == p) pc = c;
    if (a == q) qc = c;
    c++;
  } while (next_permutation(a.begin(), a.end()));

  cout << abs(pc - qc) << endl;

  return 0;
}
