#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

enum command {
  ADD = 0,
  DELETE,
  SAVE,
  LOAD,
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<pair<command, int>> order(n);
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;
    if (s == "ADD") {
      int x;
      cin >> x;
      order[i].first = ADD;
      order[i].second = x;
    }
    else if (s == "DELETE") {
      order[i].first = DELETE;
      order[i].second = -1;
    }
    else if (s == "SAVE") {
      int x;
      cin >> x;
      order[i].first = SAVE;
      order[i].second = x;
    }
    else if (s == "LOAD") {
      int x;
      cin >> x;
      order[i].first = LOAD;
      order[i].second = x;
    }
  }

  vector<int> ans;
  int ptr = -1;
  vector<int> value;
  vector<int> prev;
  unordered_map<int, int> ckpt;
  for (auto &[cmd, arg]: order) {
    if (cmd == ADD) {
      value.push_back(arg);
      prev.push_back(ptr);
      ptr = value.size()-1;
    }
    else if (cmd == DELETE) {
      if (ptr != -1) ptr = prev[ptr];
    }
    else if (cmd == SAVE) {
      ckpt[arg] = ptr;
    }
    else if (cmd == LOAD) {
      if (ckpt.find(arg) != ckpt.end()) ptr = ckpt[arg];
      else ptr = -1;
    }

    if (ptr != -1) ans.push_back(value[ptr]);
    else ans.push_back(-1);
  }

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
