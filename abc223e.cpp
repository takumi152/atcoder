#include <iostream>
#include <iomanip>
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

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll x, y, a, b, c;
  cin >> x >> y >> a >> b >> c;

  vector<ll> area_requirement = {a, b, c};
  sort(area_requirement.begin(), area_requirement.end());

  // +-+-+
  // | |B|
  // +A+-+
  // | |C|
  // +-+-+
  do {
    ll a_x_req = (area_requirement[0] + y - 1) / y;
    ll x_rem = x - a_x_req;
    if (x_rem <= 0) continue;

    ll b_y_req = (area_requirement[1] + x_rem - 1) / x_rem;
    ll c_y_req = (area_requirement[2] + x_rem - 1) / x_rem;
    if (b_y_req + c_y_req <= y) {
      cout << "Yes" << endl;
      return 0;
    }
  } while (next_permutation(area_requirement.begin(), area_requirement.end()));

  // +-+-+
  // | A |
  // +-+-+
  // |B|C|
  // +-+-+
  do {
    ll a_y_req = (area_requirement[0] + x - 1) / x;
    ll y_rem = y - a_y_req;
    if (y_rem <= 0) continue;

    ll b_x_req = (area_requirement[1] + y_rem - 1) / y_rem;
    ll c_x_req = (area_requirement[2] + y_rem - 1) / y_rem;
    if (b_x_req + c_x_req <= x) {
      cout << "Yes" << endl;
      return 0;
    }
  } while (next_permutation(area_requirement.begin(), area_requirement.end()));

  // +-+-+-+
  // | | | |
  // +A+B+C+
  // | | | |
  // +-+-+-+
  do {
    ll a_x_req = (area_requirement[0] + y - 1) / y;
    ll b_x_req = (area_requirement[1] + y - 1) / y;
    ll c_x_req = (area_requirement[2] + y - 1) / y;

    if (a_x_req + b_x_req + c_x_req <= x) {
      cout << "Yes" << endl;
      return 0;
    }
  } while (next_permutation(area_requirement.begin(), area_requirement.end()));

  // +-+-+
  // | A |
  // +-+-+
  // | B |
  // +-+-+
  // | C |
  // +-+-+
  do {
    ll a_y_req = (area_requirement[0] + x - 1) / x;
    ll b_y_req = (area_requirement[1] + x - 1) / x;
    ll c_y_req = (area_requirement[2] + x - 1) / x;

    if (a_y_req + b_y_req + c_y_req <= y) {
      cout << "Yes" << endl;
      return 0;
    }
  } while (next_permutation(area_requirement.begin(), area_requirement.end()));

  cout << "No" << endl;

  return 0;
}
