#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int a, b, c, d, e;
  cin >> a >> b >> c >> d >> e;

  unordered_set<int> threesum;
  threesum.insert(a + b + c);
  threesum.insert(a + b + d);
  threesum.insert(a + b + e);
  threesum.insert(a + c + d);
  threesum.insert(a + c + e);
  threesum.insert(a + d + e);
  threesum.insert(b + c + d);
  threesum.insert(b + c + e);
  threesum.insert(b + d + e);
  threesum.insert(c + d + e);

  vector<int> threesumvec;
  for (auto &x: threesum) threesumvec.push_back(x);

  sort(threesumvec.begin(), threesumvec.end());

  cout << threesumvec[threesumvec.size()-3] << endl;  

  return 0;
}
