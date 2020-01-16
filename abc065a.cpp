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

  int x, a, b;
  cin >> x >> a >> b;

  if (b <= a) {
    cout << "delicious" << endl;
  }
  else if (b <= a + x) {
    cout << "safe" << endl;
  }
  else {
    cout << "dangerous" << endl;
  }

  return 0;
}
