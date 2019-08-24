// WIP
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  string S;
  cin >> S;

  vector<ll> factorial(2*N+2);
  factorial[0] = 1;
  for (ll i = 0; i < 2*N+2; i++) factorial[i] = (factorial[i] * i) % MOD;

  // 端は1回しか反転できない
  ll ans = 1;
  bool flip = false;
  int left = 0;
  int right = 2*N-1;
  while(left < right) {
    if (!flip) {
      while(left < right) {
        if (S[left] == 'W') {
          cout << 0 << endl; // impossible
          return 0;
        }
        if (S[left+1] == 'B') {
          left += 2;
        }
        else {
          break;
        }
      }
      while(left < right) {
        if (S[right] == 'W') {
          cout << 0 << endl; // impossible
          return 0;
        }
        if (S[right-1] == 'B') {
          right -= 2;
        }
        else {
          break;
        }
      }
      if (left < right) {
        ans *= 2 + (right - left - 4);
        left++;
        right--;
        flip = !flip;
      }
    }
    else {
      while(left < right) {
        if (S[left] == 'B') {
          cout << 0 << endl; // impossible
          return 0;
        }
        if (S[left+1] == 'W') {
          left += 2;
        }
        else {
          break;
        }
      }
      while(left < right) {
        if (S[right] == 'B') {
          cout << 0 << endl; // impossible
          return 0;
        }
        if (S[right-1] == 'W') {
          right -= 2;
        }
        else {
          break;
        }
      }
      if (left < right) {
        ans *= 2 + (right - left - 4);
        left++;
        right--;
        flip = !flip;
      }
    }
    cout << ans
  }

  ans = (ans * factorial[N]) % MOD;

  cout << ans << endl;

  return 0;
}
