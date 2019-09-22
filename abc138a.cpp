#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int a;
  string s;
  cin >> a;
  cin >> s;

  if (a < 3200) {
    cout << "red" << endl;
  }
  else {
    cout << s << endl;
  }

  return 0;
}
