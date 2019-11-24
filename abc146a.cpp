#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;

  int ans = 0;
  if (s == "SUN") ans = 7;
  else if (s == "MON") ans = 6;
  else if (s == "TUE") ans = 5;
  else if (s == "WED") ans = 4;
  else if (s == "THU") ans = 3;
  else if (s == "FRI") ans = 2;
  else if (s == "SAT") ans = 1;

  cout << ans << endl;

  return 0;
}
