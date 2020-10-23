#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  set<int> remaining;
  for (auto &x: a) remaining.insert(x);

  while (true) {
    int minimum = *(remaining.begin());
    if (remaining.size() == 1) {
      cout << minimum << endl;
      return 0;
    }
    int maximum = *(remaining.rbegin());
    remaining.erase(maximum);
    int delta = maximum - minimum;
    int dec_time = (delta + minimum - 1) / minimum;
    int next = maximum - minimum * dec_time;
    remaining.insert(next);
  }

  return 0;
}
