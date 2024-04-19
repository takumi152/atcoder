#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUShortMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x0000ffffffffffff) * mod) >> 48;
  }
  inline unsigned int nextUInt() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUIntMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x00000000ffffffff) * mod) >> 32;
  }
  inline unsigned long long int nextULL() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline double nextDouble() {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return (double)x * 5.42101086242752217e-20;
  }
};

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

xorshift64 theRandom;

vector<bool> erathosthenes(int n) {
  vector<bool> mask(n+1, true);
  mask[0] = false;
  mask[1] = false;
  for (int i = 2; i * i <= n; i++) {
    for (int j = i * i; j <= n; j += i) {
      mask[j] = false;
    }
  }
  return mask;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  auto is_prime = erathosthenes(n * n * 2);
  vector<vector<int>> ans(n, vector<int>(n));

  if (n == 3) {
    ans = {{6, 2, 4},
           {8, 7, 5},
           {1, 9, 3}};
  }
  else if (n % 2 == 0) {
    vector<int> odd;
    vector<int> even;
    for (int i = 1; i <= n * n; i++) {
      if (i % 2 != 0) odd.push_back(i);
      else even.push_back(i);
    }

    for (int i = 0; i < n; i++) {
      while (true) {
        auto e = theRandom.nextUIntMod(even.size());
        auto o = theRandom.nextUIntMod(odd.size());
        if (is_prime[even[e] + odd[o]]) continue;
        
        ans[n/2-1][i] = even[e];
        ans[n/2][i] = odd[o];

        swap(even[e], even[even.size()-1]);
        even.pop_back();
        swap(odd[o], odd[odd.size()-1]);
        odd.pop_back();
        break;
      }
    }

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (ans[i][j] != 0) continue;
        if (i * n + j < n * n / 2) {
          ans[i][j] = even[even.size()-1];
          even.pop_back();
        }
        else {
          ans[i][j] = odd[odd.size()-1];
          odd.pop_back();
        }
      }
    }
  }
  else {
    vector<int> odd;
    vector<int> even;
    for (int i = 1; i <= n * n; i++) {
      if (i % 2 != 0) odd.push_back(i);
      else even.push_back(i);
    }

    while (true) {
      auto e1 = theRandom.nextUIntMod(even.size());
      auto e2 = theRandom.nextUIntMod(even.size());
      auto o1 = theRandom.nextUIntMod(odd.size());
      auto o2 = theRandom.nextUIntMod(odd.size());
      if (e1 == e2 || o1 == o2) continue;
      if (is_prime[even[e1] + odd[o1]] || is_prime[even[e2] + odd[o1]] || is_prime[even[e2] + odd[o2]]) continue;
      
      ans[n/2-1][n/2] = even[e1];
      ans[n/2][n/2-1] = even[e2];
      ans[n/2][n/2] = odd[o1];
      ans[n/2+1][n/2-1] = odd[o2];

      if (e1 < e2) swap(e1, e2);
      swap(even[e1], even[even.size()-1]);
      even.pop_back();
      swap(even[e2], even[even.size()-1]);
      even.pop_back();
      if (o1 < o2) swap(o1, o2);
      swap(odd[o1], odd[odd.size()-1]);
      odd.pop_back();
      swap(odd[o2], odd[odd.size()-1]);
      odd.pop_back();
      break;
    }

    for (int i = 0; i < n / 2 - 1; i++) {
      while (true) {
        auto e = theRandom.nextUIntMod(even.size());
        auto o = theRandom.nextUIntMod(odd.size());
        if (is_prime[even[e] + odd[o]]) continue;
        
        ans[n/2][i] = even[e];
        ans[n/2+1][i] = odd[o];

        swap(even[e], even[even.size()-1]);
        even.pop_back();
        swap(odd[o], odd[odd.size()-1]);
        odd.pop_back();
        break;
      }
    }

    for (int i = n / 2 + 1; i < n; i++) {
      while (true) {
        auto e = theRandom.nextUIntMod(even.size());
        auto o = theRandom.nextUIntMod(odd.size());
        if (is_prime[even[e] + odd[o]]) continue;
        
        ans[n/2-1][i] = even[e];
        ans[n/2][i] = odd[o];

        swap(even[e], even[even.size()-1]);
        even.pop_back();
        swap(odd[o], odd[odd.size()-1]);
        odd.pop_back();
        break;
      }
    }

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (ans[i][j] != 0) continue;
        if (i * n + j < n * n / 2) {
          ans[i][j] = even[even.size()-1];
          even.pop_back();
        }
        else {
          ans[i][j] = odd[odd.size()-1];
          odd.pop_back();
        }
      }
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cout << ans[i][j] << " ";
    }
    cout << endl;
  }
}
