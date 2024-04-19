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
#include <unordered_set>
#include <random>
#include <cmath>
#include <cassert>

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
    if (stopped) return lastStop - t;
    else return now() - t;
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
mt19937 theMersenne(1);

constexpr int n = 50;

int sx, sy;
vector<vector<int> > tile(n, vector<int>(n));
vector<vector<int> > board(n, vector<int>(n));

vector<int> ans;

int calc_score(int penalty_factor) {
  const vector<int> dx = {-1, 1,  0, 0};
  const vector<int> dy = { 0, 0, -1, 1};

  int px = sx;
  int py = sy;

  vector<bool> passed(n*n);
  passed[tile[px][py]] = true;
  int score = board[px][py];
  for (auto &next: ans) {
    px += dx[next];
    py += dy[next];
    if (px < 0 || px >= n || py < 0 || py >= n) return -1000000007;
    if (passed[tile[px][py]]) score -= penalty_factor;
    passed[tile[px][py]] = true;
    score += board[px][py];
  }
  return score;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> sx >> sy;
  for (auto &x: tile) {
    for (auto &y: x) cin >> y;
  }
  for (auto &x: board) {
    for (auto &y: x) cin >> y;
  }

  // preprocessing
  auto dfs1 = [&](auto self, int px, int py, int score, double time_limit, vector<int> &now_ans, int &best_score, vector<int> &best_ans, vector<bool> &passed, vector<vector<int> > &dir_list) {
    const vector<int> dx = {-1, 1,  0, 0};
    const vector<int> dy = { 0, 0, -1, 1};

    score += board[px][py];
    if (score > best_score) {
      // cerr << best_score << endl;
      best_score = score;
      best_ans = now_ans;
    }
    if (theTimer.time() > time_limit) return;

    passed[tile[px][py]] = true;

    vector<int> dir;
         if (px > py && px + py < 50)   dir = dir_list[0];
    else if (px <= py && px + py < 50)  dir = dir_list[1];
    else if (px > py && px + py >= 50)  dir = dir_list[2];
    else if (px <= py && px + py >= 50) dir = dir_list[3];

    for (int i = 0; i < 4; i++) {
      if (px + dx[dir[i]] < 0 || px + dx[dir[i]] >= n || py + dy[dir[i]] < 0 || py + dy[dir[i]] >= n) continue;
      if (passed[tile[px+dx[dir[i]]][py+dy[dir[i]]]]) continue;
      now_ans.push_back(dir[i]);
      self(self, px + dx[dir[i]], py + dy[dir[i]], score, time_limit, now_ans, best_score, best_ans, passed, dir_list);
      now_ans.pop_back();
    }

    passed[tile[px][py]] = false;
  };

  {
    vector<int> now_ans;
    int best_score = 0;
    vector<int> best_ans;
    vector<bool> passed(n*n);
    vector<vector<int> > dir_list;
    dir_list = vector<vector<int> >({{1, 2, 0, 3},
                                     {2, 0, 3, 1},
                                     {3, 1, 2, 0},
                                     {0, 3, 1, 2}});
    dfs1(dfs1, sx, sy, 0, 0.100, now_ans, best_score, best_ans, passed, dir_list);

    dir_list = vector<vector<int> >({{2, 1, 0, 3},
                                     {0, 2, 3, 1},
                                     {1, 3, 2, 0},
                                     {3, 0, 1, 2}});
    dfs1(dfs1, sx, sy, 0, 0.200, now_ans, best_score, best_ans, passed, dir_list);

    dir_list = vector<vector<int> >({{0, 2, 1, 3},
                                     {3, 0, 2, 1},
                                     {2, 1, 3, 0},
                                     {1, 3, 0, 2}});
    dfs1(dfs1, sx, sy, 0, 0.300, now_ans, best_score, best_ans, passed, dir_list);

    dir_list = vector<vector<int> >({{2, 0, 1, 3},
                                     {0, 3, 2, 1},
                                     {1, 2, 3, 0},
                                     {3, 1, 0, 2}});
    dfs1(dfs1, sx, sy, 0, 0.400, now_ans, best_score, best_ans, passed, dir_list);

    ans = best_ans;
  }

  // theRandom.x = ((ll) random_device()() << 32) | (ll) random_device()();

  // optimization
  int score = calc_score(100000);
  int lastScore = score;
  int bestScore = score;

  double baseTemperature = 1e1;
  double temperature = baseTemperature;
  double decayRate = 1e-5;
  double timeLimit = 0.500;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.25) {
      int d = theRandom.nextUIntMod(4);

      ans.push_back(d);

      score = calc_score(100000);

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        ans.pop_back();
        score = lastScore;
      }
    }
    else if (roll < 0.5) {
      if (ans.empty()) continue;
      int d_last = ans[ans.size()-1];

      ans.pop_back();

      score = calc_score(100000);

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        ans.push_back(d_last);
        score = lastScore;
      }
    }
    else if (roll < 0.75) {
      if (ans.empty()) continue;
      int p1 = theRandom.nextUIntMod(ans.size());
      int p2 = p1 + theRandom.nextUIntMod(11) - 5;
      if (p2 < 0 || p2 >= (int) ans.size()) continue;
      if (p1 == p2) continue;
      if (p1 > p2) swap(p1, p2);
      int d = theRandom.nextUIntMod(4);

      ans.insert(ans.begin() + p1, d);
      ans.insert(ans.begin() + p2 + 1, d ^ 1);

      score = calc_score(100000);

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        ans.erase(ans.begin() + p2 + 1);
        ans.erase(ans.begin() + p1);
        score = lastScore;
      }
    }
    else if (roll < 1.0) {
      if (ans.empty()) continue;
      int p1 = theRandom.nextUIntMod(ans.size());
      int p2 = p1 + theRandom.nextUIntMod(11) - 5;
      if (p2 < 0 || p2 >= (int) ans.size()) continue;
      if (ans[p1] != (ans[p2] ^ 1)) continue;
      if (p1 > p2) swap(p1, p2);

      int p1_before = ans[p1];
      int p2_before = ans[p2];

      ans.erase(ans.begin() + p2);
      ans.erase(ans.begin() + p1);

      score = calc_score(100000);

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        ans.insert(ans.begin() + p1, p1_before);
        ans.insert(ans.begin() + p2, p2_before);
        score = lastScore;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;


  auto dfs2 = [&](auto self, int px, int py, int tx, int ty, int score, double time_limit, vector<int> &now_ans, double &best_score, vector<int> &best_ans, vector<bool> &passed, vector<int> &dir) {
    const vector<int> dx = {-1, 1,  0, 0};
    const vector<int> dy = { 0, 0, -1, 1};
    if (theTimer.time() > time_limit) return;

    score += board[px][py];

    passed[tile[px][py]] = true;

    shuffle(dir.begin(), dir.end(), theMersenne);

    for (int i = 0; i < 4; i++) {
      if (px + dx[dir[i]] < 0 || px + dx[dir[i]] >= n || py + dy[dir[i]] < 0 || py + dy[dir[i]] >= n) continue;
      if (passed[tile[px+dx[dir[i]]][py+dy[dir[i]]]]) {
        if (px+dx[dir[i]] == tx && py+dy[dir[i]] == ty) {
          double score_mean = (double) score / (double) now_ans.size() + (double) now_ans.size() * 0.8;
          if (score_mean > best_score) {
            best_score = score_mean;
            best_ans = now_ans;
            best_ans.push_back(dir[i]);
          }
        }
        continue;
      }
      now_ans.push_back(dir[i]);
      self(self, px + dx[dir[i]], py + dy[dir[i]], tx, ty, score, time_limit, now_ans, best_score, best_ans, passed, dir);
      now_ans.pop_back();
    }

    passed[tile[px][py]] = false;
  };

  {
    timeLimit = 1.250;
    vector<int> now_ans;
    double best_score = 0.0;
    vector<int> best_ans;
    vector<bool> passed(n*n);
    vector<int> dir = {0, 1, 2, 3};

    vector<Pii> pos;
    pos.emplace_back(sx, sy);
    passed[tile[sx][sy]] = true;
    {
      const vector<int> dx = {-1, 1,  0, 0};
      const vector<int> dy = { 0, 0, -1, 1};
      for (int i = 0; i < (int) ans.size(); i++) {
        pos.emplace_back(pos[pos.size()-1].first + dx[ans[i]], pos[pos.size()-1].second + dy[ans[i]]);
        passed[tile[pos[pos.size()-1].first][pos[pos.size()-1].second]] = true;
      }
    }
    while (theTimer.time() < timeLimit) {
      int p1 = theRandom.nextUIntMod(pos.size());
      int p2 = p1 + theRandom.nextUIntMod(81) - 40;
      if (p2 < 0 || p2 > (int) ans.size()) continue;
      if (p1 == p2) continue;
      if (p1 > p2) swap(p1, p2);
      if (p2 - p1 < 20) continue;

      int part_score = 0;
      for (int i = p1; i < p2; i++) {
        part_score += board[pos[i].first][pos[i].second];
        passed[tile[pos[i].first][pos[i].second]] = false;
      }
      double part_score_mean = (double) part_score / (p2 - p1) + (double) (p2 - p1) * 0.8;

      now_ans.clear();
      best_score = 0.0;
      best_ans.clear();
      dfs2(dfs2, pos[p1].first, pos[p1].second, pos[p2].first, pos[p2].second, 0, theTimer.time() + 0.0002, now_ans, best_score, best_ans, passed, dir);
      if (best_score > part_score_mean) {
        int score_gain = 0;
        {
          const vector<int> dx = {-1, 1,  0, 0};
          const vector<int> dy = { 0, 0, -1, 1};

          int px = pos[p1].first;
          int py = pos[p1].second;

          int tx = pos[p2].first;
          int ty = pos[p2].second;

          ans.erase(ans.begin() + p1, ans.begin() + p2);
          pos.erase(pos.begin() + p1, pos.begin() + p2);

          for (int i = 0; i < (int) best_ans.size(); i++) {
            score_gain += board[px][py];
            assert(!passed[tile[px][py]]);
            passed[tile[px][py]] = true;
            ans.insert(ans.begin() + p1 + i, best_ans[i]);
            pos.insert(pos.begin() + p1 + i, Pii(px, py));

            px += dx[best_ans[i]];
            py += dy[best_ans[i]];
          }

          assert(px == tx);
          assert(py == ty);
        }

        score += (score_gain - part_score);
      }
      else {
        for (int i = p1; i < p2; i++) {
          passed[tile[pos[i].first][pos[i].second]] = true;
        }
      }
      {
        const vector<int> dx = {-1, 1,  0, 0};
        const vector<int> dy = { 0, 0, -1, 1};

        vector<bool> passed_check(n*n);
        passed_check[tile[pos[0].first][pos[0].second]] = true;
        for (int i = 0; i < (int) ans.size(); i++) {
          assert(pos[i].first + dx[ans[i]] == pos[i+1].first);
          assert(pos[i].second + dy[ans[i]] == pos[i+1].second);
          assert(!passed_check[tile[pos[i+1].first][pos[i+1].second]]);
          passed_check[tile[pos[i+1].first][pos[i+1].second]] = true;
        }
      }
    }
    cerr << "score = " << score << endl;
  }


  auto dfs3 = [&](auto self, int px, int py, int tx, int ty, int score, double time_limit, vector<int> &now_ans, int &best_score, vector<int> &best_ans, vector<bool> &passed, vector<int> &dir) {
    const vector<int> dx = {-1, 1,  0, 0};
    const vector<int> dy = { 0, 0, -1, 1};
    if (theTimer.time() > time_limit) return;

    score += board[px][py];

    passed[tile[px][py]] = true;

    shuffle(dir.begin(), dir.end(), theMersenne);

    for (int i = 0; i < 4; i++) {
      if (px + dx[dir[i]] < 0 || px + dx[dir[i]] >= n || py + dy[dir[i]] < 0 || py + dy[dir[i]] >= n) continue;
      if (passed[tile[px+dx[dir[i]]][py+dy[dir[i]]]]) {
        if (px+dx[dir[i]] == tx && py+dy[dir[i]] == ty) {
          if (score > best_score) {
            best_score = score;
            best_ans = now_ans;
            best_ans.push_back(dir[i]);
          }
        }
        continue;
      }
      now_ans.push_back(dir[i]);
      self(self, px + dx[dir[i]], py + dy[dir[i]], tx, ty, score, time_limit, now_ans, best_score, best_ans, passed, dir);
      now_ans.pop_back();
    }

    passed[tile[px][py]] = false;
  };

  {
    timeLimit = 1.980;
    vector<int> now_ans;
    int best_score = 0;
    vector<int> best_ans;
    vector<bool> passed(n*n);
    vector<int> dir = {0, 1, 2, 3};

    vector<Pii> pos;
    pos.emplace_back(sx, sy);
    passed[tile[sx][sy]] = true;
    {
      const vector<int> dx = {-1, 1,  0, 0};
      const vector<int> dy = { 0, 0, -1, 1};
      for (int i = 0; i < (int) ans.size(); i++) {
        pos.emplace_back(pos[pos.size()-1].first + dx[ans[i]], pos[pos.size()-1].second + dy[ans[i]]);
        passed[tile[pos[pos.size()-1].first][pos[pos.size()-1].second]] = true;
      }
    }
    while (theTimer.time() < timeLimit) {
      int p1 = theRandom.nextUIntMod(pos.size());
      int p2 = p1 + theRandom.nextUIntMod(81) - 40;
      if (p2 < 0 || p2 > (int) ans.size()) continue;
      if (p1 == p2) continue;
      if (p1 > p2) swap(p1, p2);
      if (p2 - p1 < 20) continue;

      int part_score = 0;
      for (int i = p1; i < p2; i++) {
        part_score += board[pos[i].first][pos[i].second];
        passed[tile[pos[i].first][pos[i].second]] = false;
      }

      now_ans.clear();
      best_score = 0;
      best_ans.clear();
      dfs3(dfs3, pos[p1].first, pos[p1].second, pos[p2].first, pos[p2].second, 0, theTimer.time() + 0.0002, now_ans, best_score, best_ans, passed, dir);
      if (best_score > part_score) {
        {
          const vector<int> dx = {-1, 1,  0, 0};
          const vector<int> dy = { 0, 0, -1, 1};

          int px = pos[p1].first;
          int py = pos[p1].second;

          int tx = pos[p2].first;
          int ty = pos[p2].second;

          ans.erase(ans.begin() + p1, ans.begin() + p2);
          pos.erase(pos.begin() + p1, pos.begin() + p2);

          for (int i = 0; i < (int) best_ans.size(); i++) {
            assert(!passed[tile[px][py]]);
            passed[tile[px][py]] = true;
            ans.insert(ans.begin() + p1 + i, best_ans[i]);
            pos.insert(pos.begin() + p1 + i, Pii(px, py));

            px += dx[best_ans[i]];
            py += dy[best_ans[i]];
          }

          assert(px == tx);
          assert(py == ty);
        }

        score += (best_score - part_score);
      }
      else {
        for (int i = p1; i < p2; i++) {
          passed[tile[pos[i].first][pos[i].second]] = true;
        }
      }
      {
        const vector<int> dx = {-1, 1,  0, 0};
        const vector<int> dy = { 0, 0, -1, 1};

        vector<bool> passed_check(n*n);
        passed_check[tile[pos[0].first][pos[0].second]] = true;
        for (int i = 0; i < (int) ans.size(); i++) {
          assert(pos[i].first + dx[ans[i]] == pos[i+1].first);
          assert(pos[i].second + dy[ans[i]] == pos[i+1].second);
          assert(!passed_check[tile[pos[i+1].first][pos[i+1].second]]);
          passed_check[tile[pos[i+1].first][pos[i+1].second]] = true;
        }
      }
    }
    cerr << "score = " << score << endl;
  }

  // postprocess & output
  const vector<char> out_char = {'U', 'D', 'L', 'R'};
  for (auto &x: ans) cout << out_char[x];
  cout << endl;

  return 0;
}
