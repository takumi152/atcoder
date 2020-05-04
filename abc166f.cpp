#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <deque>
#include <unordered_set>
#include <random>
#include <cmath>
#include <cassert>

#include <x86intrin.h>

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned short nextUShortMod(unsigned long long int mod) {
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

struct timer {
  double t = 0.0;
  double lastStop = 0.0;
  bool stopped = false;
  timer() {
    restart();
  }
  inline void restart() {
    t = now();
    stopped = false;
  }
  inline void start() {
    if (stopped) {
      t += now() - lastStop;
      stopped = false;
    }
  }
  inline void stop() {
    if (!stopped) {
      lastStop = now();
      stopped = true;
    }
  }
  inline double time() {
    return now() - t;
  }
  inline double now() {
    unsigned long long l, h;
    __asm__ ("rdtsc" : "=a"(l), "=d"(h));
    #ifdef LOCAL
    return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
    #else
    //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
    //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
    return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
    #endif
  }
};

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;

//#define DEBUG

struct lazy_segtree {
  int n;
  vector<int> data;
  vector<int> lazy;
  vector<bool> lazyFlag;

  lazy_segtree() {
    init(1);
  }

  lazy_segtree(const int s) {
    init(s);
  }

  lazy_segtree(const vector<int> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = min(data[i*2+1], data[i*2+2]);
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<int>(2*n-1);
    lazy = vector<int>(2*n-1);
    lazyFlag = vector<bool>(2*n-1);
  }

  void evaluate(int p, int a, int b) {
    if (lazyFlag[p]) {
      data[p] += lazy[p];
      if (b - a > 1) {
        lazy[p*2+1] += lazy[p];
        lazy[p*2+2] += lazy[p];
        lazyFlag[p*2+1] = true;
        lazyFlag[p*2+2] = true;
      }
      lazy[p] = 0;
      lazyFlag[p] = false;
    }
  }

  void update(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init

    // evaluate lazy value
    evaluate(p, a, b);

    if (r <= a || b <= l) return; // out of range
    if (l <= a && b <= r) { // fully covered
      lazy[p] += v;
      lazyFlag[p] = true;
      evaluate(p, a, b);
    }
    else {
      update(l, r, v, p*2+1, a, (a + b) / 2); // left
      update(l, r, v, p*2+2, (a + b) / 2, b); // right
      data[p] = min(data[p*2+1], data[p*2+2]); // update
    }
    return;
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x7fffffff; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return min(vl, vr);
  }

  int mindex(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return -1; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1)                                                     return p - (n - 1);
      else if (data[p*2+1] + lazy[p*2+1] <= data[p*2+2] + lazy[p*2+2])  return mindex(l, r, p*2+1, a, (a + b) / 2);
      else                                                              return mindex(l, r, p*2+2, (a + b) / 2, b);
    }

    int vl = mindex(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = mindex(l, r, p*2+2, (a + b) / 2, b); // right
    if (vl == -1 && vr == -1) return -1;
    else if (vl == -1) return vr;
    else if (vr == -1) return vl;
    else if (data[vl+n-1] < data[vr+n-1]) return vl;
    else return vr;
  }

  int first_less_than(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return r; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) { // fully covered
      if (p >= n-1) {
        if (data[p] < v)         return p - (n - 1);
        else                     return r; // not found
      }
      else if (data[p*2+1] + lazy[p*2+1] < v)  return first_less_than(l, r, v, p*2+1, a, (a + b) / 2); // left
      else if (data[p*2+2] + lazy[p*2+2] < v)  return first_less_than(l, r, v, p*2+2, (a + b) / 2, b); // right
      else                                     return r; // not found
    }

    int vl = first_less_than(l, r, v, p*2+1, a, (a + b) / 2); // left
    int vr = first_less_than(l, r, v, p*2+2, (a + b) / 2, b); // right
    if (vl == r && vr == r) return r;
    else if (vl == r) return vr;
    else if (vr == r) return vl;
    else return vl;
  }
};

int n, a, b, c;
vector<string> s;

vector<bool> choice;

lazy_segtree lst_a, lst_b, lst_c;

ll calcScore() {
  return min(min(lst_a.first_less_than(0, n+1, 0), lst_b.first_less_than(0, n+1, 0)), lst_c.first_less_than(0, n+1, 0)) - (n+1);
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> n >> a >> b >> c;
  s = vector<string>(n);
  for (auto &x: s) cin >> x;

  /// preprocess
  choice = vector<bool>(n);
  vector<int> a_score(n+1), b_score(n+1), c_score(n+1);
  a_score[0] = a;
  b_score[0] = b;
  c_score[0] = c;
  for (int i = 0; i < n; i++) {
    a_score[i+1] = a_score[i];
    b_score[i+1] = b_score[i];
    c_score[i+1] = c_score[i];
    if (s[i] == "AB") {
      a_score[i+1]++;
      b_score[i+1]--;
    }
    else if (s[i] == "AC") {
      a_score[i+1]++;
      c_score[i+1]--;
    }
    else if (s[i] == "BC") {
      b_score[i+1]++;
      c_score[i+1]--;
    }
  }

  lst_a = lazy_segtree(a_score);
  lst_b = lazy_segtree(b_score);
  lst_c = lazy_segtree(c_score);

  // optimization
  ll score = calcScore();
  ll lastScore = score;
  ll bestScore = score;

  double baseTemperature = 1e-1;
  double temperature = baseTemperature;
  double decayRate = 1e-6;
  double timeLimit = 1.900;
  int iterCount = 0;

  while (theTimer.time() < timeLimit && bestScore < 0) {
    //int r = theRandom.nextUIntMod(n);
    int r = theRandom.nextUIntMod(10) - 5 + (lastScore+n);
    if (r < 0 || r >= n) continue;

    if (s[r] == "AB") {
      if (!choice[r]) {
        choice[r] = true;
        lst_a.update(r+1, n+1, -2);
        lst_b.update(r+1, n+1, 2);
      }
      else {
        choice[r] = false;
        lst_a.update(r+1, n+1, 2);
        lst_b.update(r+1, n+1, -2);
      }
    }
    else if (s[r] == "AC") {
      if (!choice[r]) {
        choice[r] = true;
        lst_a.update(r+1, n+1, -2);
        lst_c.update(r+1, n+1, 2);
      }
      else {
        choice[r] = false;
        lst_a.update(r+1, n+1, 2);
        lst_c.update(r+1, n+1, -2);
      }
    }
    else if (s[r] == "BC") {
      if (!choice[r]) {
        choice[r] = true;
        lst_b.update(r+1, n+1, -2);
        lst_c.update(r+1, n+1, 2);
      }
      else {
        choice[r] = false;
        lst_b.update(r+1, n+1, 2);
        lst_c.update(r+1, n+1, -2);
      }
    }

    score = calcScore();

    #ifdef DEBUG
    if (iterCount % 10000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << exp(double(score - lastScore) / temperature) << endl;
    #endif

    if (score >= lastScore) {
      lastScore = score;
      if (score > bestScore) {
        bestScore = score;
        if (bestScore >= 0) {
          break;
        }
      }
    }
    else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
      lastScore = score;
    }
    else { // rollback
      if (s[r] == "AB") {
        if (!choice[r]) {
          choice[r] = true;
          lst_a.update(r+1, n+1, -2);
          lst_b.update(r+1, n+1, 2);
        }
        else {
          choice[r] = false;
          lst_a.update(r+1, n+1, 2);
          lst_b.update(r+1, n+1, -2);
        }
      }
      else if (s[r] == "AC") {
        if (!choice[r]) {
          choice[r] = true;
          lst_a.update(r+1, n+1, -2);
          lst_c.update(r+1, n+1, 2);
        }
        else {
          choice[r] = false;
          lst_a.update(r+1, n+1, 2);
          lst_c.update(r+1, n+1, -2);
        }
      }
      else if (s[r] == "BC") {
        if (!choice[r]) {
          choice[r] = true;
          lst_b.update(r+1, n+1, -2);
          lst_c.update(r+1, n+1, 2);
        }
        else {
          choice[r] = false;
          lst_b.update(r+1, n+1, 2);
          lst_c.update(r+1, n+1, -2);
        }
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;


  // cerr << lst_a.first_less_than(0, n+1, 0) << " " << lst_b.first_less_than(0, n+1, 0) << " " << lst_c.first_less_than(0, n+1, 0) << endl;
  // for (int i = lst_a.n-1; i < lst_a.n+20; i++) cerr << setw(3) << lst_a.data[i] << " ";
  // cerr << endl;
  // for (int i = lst_a.n-1; i < lst_a.n+20; i++) cerr << setw(3) << lst_a.lazy[i] << " ";
  // cerr << endl;
  // for (int i = lst_b.n-1; i < lst_b.n+20; i++) cerr << setw(3) << lst_b.data[i] << " ";
  // cerr << endl;
  // for (int i = lst_b.n-1; i < lst_b.n+20; i++) cerr << setw(3) << lst_b.lazy[i] << " ";
  // cerr << endl;
  // for (int i = lst_c.n-1; i < lst_c.n+20; i++) cerr << setw(3) << lst_c.data[i] << " ";
  // cerr << endl;
  // for (int i = lst_c.n-1; i < lst_c.n+20; i++) cerr << setw(3) << lst_c.lazy[i] << " ";
  // cerr << endl;

  // postprocess & output
  if (bestScore < 0) {
    cout << "No" << endl;
  }
  else {
    cout << "Yes" << endl;
    for (int i = 0; i < n; i++) {
      if (s[i] == "AB") {
        if (!choice[i]) {
          cout << "A" << endl;
        }
        else {
          cout << "B" << endl;
        }
      }
      else if (s[i] == "AC") {
        if (!choice[i]) {
          cout << "A" << endl;
        }
        else {
          cout << "C" << endl;
        }
      }
      else if (s[i] == "BC") {
        if (!choice[i]) {
          cout << "B" << endl;
        }
        else {
          cout << "C" << endl;
        }
      }
    }
  }

  return 0;
}
