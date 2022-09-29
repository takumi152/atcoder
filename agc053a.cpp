// WA

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  string s;
  cin >> s;
  vector<int> a(n+1);
  for (auto &x: a) cin >> x;

  vector<int> b_base(n+1);
  b_base[0] = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] == '<') b_base[i+1] = b_base[i] + 1;
    else b_base[i+1] = b_base[i] - 1;
  }

  int b_min = *min_element(b_base.begin(), b_base.end());
  for (int i = 0; i <= n; i++) b_base[i] += -b_min;

  vector<vector<int> > ans;
  while (true) {
    vector<int> next_b = b_base;

    for (int i = 0; i <= n; i++) {
      if (a[i] - next_b[i] < 0) {
        bool subtract = true;
        if (i > 0) {
          if (s[i-1] != '>') subtract = false;
        }
        if (s[i] != '<') subtract = true;
        if (subtract) {
          next_b[i] = a[i];

          int j = i-1;
          for (; j >= 0; j--) {
            bool good = true;
            if (j > 0) {
              if (s[j-1] == '<' && !(next_b[j-1] < next_b[j+1] + 1)) good = false;
            }

            if (good) next_b[j] = next_b[j+1] + 1;

            if (j > 0) {
              if (s[j-1] == '<') break;
            }
          }

          if (j < 0) j = 0;
          priority_queue<Pii> segment_left;
          for (int k = j; k < i; k++) segment_left.emplace(a[k] - a[k+1], k);
          if (!segment_left.empty()) {
            unordered_map<int, int> additional_cost_left;
            for (int k = next_b[j+1]; k < next_b[j] - 1; k++) {
              auto next_segment = segment_left.top();
              segment_left.pop();
              additional_cost_left[next_segment.second]++;
              next_segment.first--;
              segment_left.push(next_segment);
            }
            int p = next_b[i];
            for (int k = i-1; k > j; k--) {
              p += 1 + additional_cost_left[k];
              next_b[k] = p;
            }
          }

          j = i+1;
          for (; j <= n; j++) {
            bool good = true;
            if (j < n) {
              if (s[j] == '>' && !(next_b[j-1] + 1 > next_b[j+1])) good = false;
            }

            if (good) next_b[j] = next_b[j-1] + 1;

            if (j < n) {
              if (s[j] == '>') break;
            }
          }

          if (j > n) j = n;
          priority_queue<Pii> segment_right;
          for (int k = i; k < j; k++) segment_right.emplace(a[k+1] - a[k], k);
          if (!segment_right.empty()) {
            unordered_map<int, int> additional_cost_right;
            for (int k = next_b[j-1]; k < next_b[j] - 1; k++) {
              auto next_segment = segment_right.top();
              segment_right.pop();
              additional_cost_right[next_segment.second]++;
              next_segment.first--;
              segment_right.push(next_segment);
            }
            int p = next_b[i];
            for (int k = i+1; k < j; k++) {
              p += 1 + additional_cost_right[k-1];
              next_b[k] = p;
            }
          }
        }
      }
    }

    for (int i = 0; i <= n; i++) {
      a[i] -= next_b[i];
    }

    bool good = true;
    for (int i = 0; i < n; i++) {
      if ((s[i] == '<' && !(a[i] < a[i+1])) || (s[i] == '>' && !(a[i] > a[i+1]))) {
        good = false;
        break;
      }
    }

    if (good) ans.push_back(next_b);
    else {
      for (int i = 0; i <= n; i++) {
        a[i] += next_b[i];
      }
      ans.push_back(a);
      break;
    }
  }

  cout << ans.size() << endl;
  for (auto &x: ans) {
    for (auto &y: x) cout << y << " ";
    cout << endl;
  }

  return 0;
}
