#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
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
    //return (double)(l | h << 32) * 2.7027027027027025e-10; // 1 / 3.7e9, for local (Ryzen 7 1700, overclock @ 3.7 GHz)
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

#define DEBUG

int id, n, k;
vector<vector<short> > board;

vector<vector<int> > ans;

struct unionfind {
  vector<int> group;
  vector<int> rank;

  unionfind() {
    unionfind(0);
  }

  unionfind(int n) {
    group = vector<int>(n);
    rank = vector<int>(n);
    for (int i = 0; i < n; i++) group[i] = i;
  }

  int root(int x) {
    if (group[x] == x) return x;
    return group[x] = root(group[x]);
  }

  void unite(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    if (rank[rx] > rank[ry]) group[ry] = rx;
    else if (rank[ry] > rank[rx]) group[rx] = ry;
    else if (rx != ry) {
      group[ry] = rx;
      rank[rx]++;
    }
  }

  bool same(int x, int y) {
    int rx = root(x);
    int ry = root(y);
    return rx == ry;
  }
};

int solve() {
  int stepCount = 0;
  auto boardNow = board;

  // vector<vector<short> > chunkSize(n, vector<short>(n));
  // vector<vector<bool> > visited(n, vector<bool>(n));
  // for (int i = 0; i < n; i++) {
  //   for (int j = 0; j < n; j++) {
  //     if (visited[i][j]) continue;
  //     short cellCount = 0;
  //     queue<Pii> que;
  //     queue<Pii> visitedCell;
  //     que.emplace(i, j);
  //
  //     while (!que.empty()) {
  //       auto now = que.front();
  //       que.pop();
  //       if (visited[now.first][now.second]) continue;
  //       visitedCell.push(now);
  //       cellCount++;
  //       visited[now.first][now.second] = true;
  //
  //       if (now.first > 0) {
  //         if (!visited[now.first-1][now.second] && board[now.first-1][now.second] == board[i][j]) que.emplace(now.first-1, now.second);
  //       }
  //       if (now.first < n-1) {
  //         if (!visited[now.first+1][now.second] && board[now.first+1][now.second] == board[i][j]) que.emplace(now.first+1, now.second);
  //       }
  //       if (now.second > 0) {
  //         if (!visited[now.first][now.second-1] && board[now.first][now.second-1] == board[i][j]) que.emplace(now.first, now.second-1);
  //       }
  //       if (now.second < n-1) {
  //         if (!visited[now.first][now.second+1] && board[now.first][now.second+1] == board[i][j]) que.emplace(now.first, now.second+1);
  //       }
  //     }
  //
  //     while (!visitedCell.empty()) {
  //       auto now = visitedCell.front();
  //       visitedCell.pop();
  //       chunkSize[now.first][now.second] = cellCount;
  //     }
  //   }
  // }
  while (true) {
    int best_delta = 0;
    int best_i = 0;
    int best_j = 0;
    int best_l = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        for (short l = 1; l <= 9; l++) {
          if (boardNow[i][j] == l) continue;
          unordered_set<int> visited;
          int total = 0;
          int maximum_size = 0;
          auto bfs1 = [&](int x, int y) {
            int size = 0;

            queue<Pii> que;
            que.emplace(x, y);
            while (!que.empty()) {
              auto now = que.front();
              que.pop();
              if (visited.find(now.first*n+now.second) != visited.end()) continue;
              visited.insert(now.first*n+now.second);
              size++;

              if (now.first > 0) {
                if (visited.find((now.first-1)*n+now.second) == visited.end() && boardNow[now.first-1][now.second] == l) que.emplace(now.first-1, now.second);
              }
              if (now.first < n-1) {
                if (visited.find((now.first+1)*n+now.second) == visited.end() && boardNow[now.first+1][now.second] == l) que.emplace(now.first+1, now.second);
              }
              if (now.second > 0) {
                if (visited.find(now.first*n+(now.second-1)) == visited.end() && boardNow[now.first][now.second-1] == l) que.emplace(now.first, now.second-1);
              }
              if (now.second < n-1) {
                if (visited.find(now.first*n+(now.second+1)) == visited.end() && boardNow[now.first][now.second+1] == l) que.emplace(now.first, now.second+1);
              }
            }

            return size;
          };

          // up
          if (i > 0) {
            if (boardNow[i-1][j] == l && visited.find((i-1)*n+j) == visited.end()) {
              int size = bfs1(i-1, j);
              total += size;
              maximum_size = max(maximum_size, size);
            }
          }
          // down
          if (i < n-1) {
            if (boardNow[i+1][j] == l && visited.find((i+1)*n+j) == visited.end()) {
              int size = bfs1(i+1, j);
              total += size;
              maximum_size = max(maximum_size, size);
            }
          }
          // left
          if (j > 0) {
            if (boardNow[i][j-1] == l && visited.find(i*n+(j-1)) == visited.end()) {
              int size = bfs1(i, j-1);
              total += size;
              maximum_size = max(maximum_size, size);
            }
          }
          // right
          if (j < n-1) {
            if (boardNow[i][j+1] == l && visited.find(i*n+(j+1)) == visited.end()) {
              int size = bfs1(i, j+1);
              total += size;
              maximum_size = max(maximum_size, size);
            }
          }

          if (total == 0) continue;

          int delta = total - maximum_size + 1;
          if (delta > best_delta) {
            best_delta = delta;
            best_i = i;
            best_j = j;
            best_l = l;
          }
        }
      }
    }

    if (best_delta == 0) break;

    auto bfs2 = [&](int x, int y, int z) {
      unordered_set<int> visited;
      queue<Pii> que;
      que.emplace(x, y);
      while (!que.empty()) {
        auto now = que.front();
        que.pop();
        if (visited.find(now.first*n+now.second) != visited.end()) continue;
        visited.insert(now.first*n+now.second);
        boardNow[now.first][now.second] = best_l;

        if (now.first > 0) {
          if (visited.find((now.first-1)*n+now.second) == visited.end() && boardNow[now.first-1][now.second] == z) que.emplace(now.first-1, now.second);
        }
        if (now.first < n-1) {
          if (visited.find((now.first+1)*n+now.second) == visited.end() && boardNow[now.first+1][now.second] == z) que.emplace(now.first+1, now.second);
        }
        if (now.second > 0) {
          if (visited.find(now.first*n+(now.second-1)) == visited.end() && boardNow[now.first][now.second-1] == z) que.emplace(now.first, now.second-1);
        }
        if (now.second < n-1) {
          if (visited.find(now.first*n+(now.second+1)) == visited.end() && boardNow[now.first][now.second+1] == z) que.emplace(now.first, now.second+1);
        }
      }
    };

    bfs2(best_i, best_j, boardNow[best_i][best_j]);
    ans.push_back(vector<int>({best_i+1, best_j+1, best_l}));

    stepCount++;

    cerr << stepCount << " " << best_delta << " " << best_i << " " << best_j << " " << best_l << endl;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) cerr << boardNow[i][j];
      cerr << endl;
    }
  }

  return stepCount;
}

int solve2() {
  int stepCount = 0;
  auto boardNow = board;

  vector<vector<short> > chunkSize(n, vector<short>(n));
  vector<vector<bool> > visited(n, vector<bool>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (visited[i][j]) continue;
      short cellCount = 0;
      queue<Pii> que;
      queue<Pii> visitedCell;
      que.emplace(i, j);

      while (!que.empty()) {
        auto now = que.front();
        que.pop();
        if (visited[now.first][now.second]) continue;
        visitedCell.push(now);
        cellCount++;
        visited[now.first][now.second] = true;

        if (now.first > 0) {
          if (!visited[now.first-1][now.second] && board[now.first-1][now.second] == board[i][j]) que.emplace(now.first-1, now.second);
        }
        if (now.first < n-1) {
          if (!visited[now.first+1][now.second] && board[now.first+1][now.second] == board[i][j]) que.emplace(now.first+1, now.second);
        }
        if (now.second > 0) {
          if (!visited[now.first][now.second-1] && board[now.first][now.second-1] == board[i][j]) que.emplace(now.first, now.second-1);
        }
        if (now.second < n-1) {
          if (!visited[now.first][now.second+1] && board[now.first][now.second+1] == board[i][j]) que.emplace(now.first, now.second+1);
        }
      }

      while (!visitedCell.empty()) {
        auto now = visitedCell.front();
        visitedCell.pop();
        chunkSize[now.first][now.second] = cellCount;
      }
    }
  }

  int first_i = 0;
  int first_j = 0;
  int largest_chunk = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (chunkSize[i][j] > largest_chunk) {
        first_i = i;
        first_j = j;
        largest_chunk = chunkSize[i][j];
      }
    }
  }

  visited = vector<vector<bool> >(n, vector<bool>(n));
  vector<vector<Pii> > next_adj(10);

  queue<Pii> que;
  que.emplace(first_i, first_j);
  while (!que.empty()) {
    auto now = que.front();
    que.pop();
    if (visited[now.first][now.second]) continue;
    visited[now.first][now.second] = true;
    if (boardNow[now.first][now.second] != boardNow[first_i][first_j]) next_adj[boardNow[now.first][now.second]].emplace_back(now.first, now.second);

    if (now.first > 0) {
      if (!visited[now.first-1][now.second] && boardNow[now.first-1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first-1, now.second);
      else if (!visited[now.first-1][now.second] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first-1, now.second);
    }
    if (now.first < n-1) {
      if (!visited[now.first+1][now.second] && boardNow[now.first+1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first+1, now.second);
      else if (!visited[now.first+1][now.second] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first+1, now.second);
    }
    if (now.second > 0) {
      if (!visited[now.first][now.second-1] && boardNow[now.first][now.second-1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second-1);
      else if (!visited[now.first][now.second-1] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first, now.second-1);
    }
    if (now.second < n-1) {
      if (!visited[now.first][now.second+1] && boardNow[now.first][now.second+1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second+1);
      else if (!visited[now.first][now.second+1] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first, now.second+1);
    }
  }

  int nowColor = boardNow[first_i][first_j];

  while (true) {
    int best_l = 0;
    int best_count = 0;
    for (int l = 1; l <= 9; l++) {
      if (l == nowColor) continue;
      if ((int) next_adj[l].size() > best_count) {
        best_l = l;
        best_count = next_adj[l].size();
      }
    }
    if (best_count == 0) break;

    auto checklist = next_adj[best_l];
    next_adj[best_l].clear();
    for (auto &x: checklist) {
      queue<Pii> que;
      que.emplace(x.first, x.second);
      while (!que.empty()) {
        auto now = que.front();
        que.pop();
        if (visited[now.first][now.second] && now != x) continue;
        visited[now.first][now.second] = true;
        if (boardNow[now.first][now.second] != best_l) next_adj[boardNow[now.first][now.second]].emplace_back(now.first, now.second);

        if (now.first > 0) {
          if (!visited[now.first-1][now.second] && boardNow[now.first-1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first-1, now.second);
          else if (!visited[now.first-1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first-1, now.second);
        }
        if (now.first < n-1) {
          if (!visited[now.first+1][now.second] && boardNow[now.first+1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first+1, now.second);
          else if (!visited[now.first+1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first+1, now.second);
        }
        if (now.second > 0) {
          if (!visited[now.first][now.second-1] && boardNow[now.first][now.second-1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second-1);
          else if (!visited[now.first][now.second-1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second-1);
        }
        if (now.second < n-1) {
          if (!visited[now.first][now.second+1] && boardNow[now.first][now.second+1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second+1);
          else if (!visited[now.first][now.second+1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second+1);
        }
      }
    }

    ans.push_back(vector<int>({first_i+1, first_j+1, best_l}));
    nowColor = best_l;
    stepCount++;
  }

  return stepCount;
}


int solve3() {
  vector<vector<short> > chunkSize(n, vector<short>(n));
  vector<vector<bool> > visited(n, vector<bool>(n));
  vector<Pii> attemptOrder;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (visited[i][j]) continue;
      short cellCount = 0;
      queue<Pii> que;
      queue<Pii> visitedCell;
      que.emplace(i, j);

      while (!que.empty()) {
        auto now = que.front();
        que.pop();
        if (visited[now.first][now.second]) continue;
        visitedCell.push(now);
        cellCount++;
        visited[now.first][now.second] = true;

        if (now.first > 0) {
          if (!visited[now.first-1][now.second] && board[now.first-1][now.second] == board[i][j]) que.emplace(now.first-1, now.second);
        }
        if (now.first < n-1) {
          if (!visited[now.first+1][now.second] && board[now.first+1][now.second] == board[i][j]) que.emplace(now.first+1, now.second);
        }
        if (now.second > 0) {
          if (!visited[now.first][now.second-1] && board[now.first][now.second-1] == board[i][j]) que.emplace(now.first, now.second-1);
        }
        if (now.second < n-1) {
          if (!visited[now.first][now.second+1] && board[now.first][now.second+1] == board[i][j]) que.emplace(now.first, now.second+1);
        }
      }

      while (!visitedCell.empty()) {
        auto now = visitedCell.front();
        visitedCell.pop();
        chunkSize[now.first][now.second] = cellCount;
      }

      attemptOrder.emplace_back(i, j);
    }
  }

  sort(attemptOrder.rbegin(), attemptOrder.rend(), [&](auto &a, auto &b) {return chunkSize[a.first][a.second] < chunkSize[b.first][b.second];});

  int p = 0;
  int best_i = 0;
  int best_j = 0;
  int best_step = 10000;
  while (theTimer.time() < 2.900) {
    if (p >= (int) attemptOrder.size()) break;

    int first_i = attemptOrder[p].first;
    int first_j = attemptOrder[p].second;

    int stepCount = 0;
    auto boardNow = board;

    visited = vector<vector<bool> >(n, vector<bool>(n));
    vector<vector<Pii> > next_adj(10);

    queue<Pii> que;
    que.emplace(first_i, first_j);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now.first][now.second]) continue;
      visited[now.first][now.second] = true;
      if (boardNow[now.first][now.second] != boardNow[first_i][first_j]) next_adj[boardNow[now.first][now.second]].emplace_back(now.first, now.second);

      if (now.first > 0) {
        if (!visited[now.first-1][now.second] && boardNow[now.first-1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first-1, now.second);
        else if (!visited[now.first-1][now.second] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first-1, now.second);
      }
      if (now.first < n-1) {
        if (!visited[now.first+1][now.second] && boardNow[now.first+1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first+1, now.second);
        else if (!visited[now.first+1][now.second] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first+1, now.second);
      }
      if (now.second > 0) {
        if (!visited[now.first][now.second-1] && boardNow[now.first][now.second-1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second-1);
        else if (!visited[now.first][now.second-1] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first, now.second-1);
      }
      if (now.second < n-1) {
        if (!visited[now.first][now.second+1] && boardNow[now.first][now.second+1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second+1);
        else if (!visited[now.first][now.second+1] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first, now.second+1);
      }
    }

    int nowColor = boardNow[first_i][first_j];

    while (true) {
      int best_l = 0;
      int best_count = 0;
      for (int l = 1; l <= 9; l++) {
        if (l == nowColor) continue;
        if ((int) next_adj[l].size() == 0) continue;

        auto next_adj_temp = next_adj;
        auto visited_temp = visited;

        auto checklist = next_adj_temp[best_l];
        next_adj_temp[l].clear();
        for (auto &x: checklist) {
          queue<Pii> que;
          que.emplace(x.first, x.second);
          while (!que.empty()) {
            auto now = que.front();
            que.pop();
            if (visited_temp[now.first][now.second] && now != x) continue;
            visited_temp[now.first][now.second] = true;
            if (boardNow[now.first][now.second] != best_l) next_adj_temp[boardNow[now.first][now.second]].emplace_back(now.first, now.second);

            if (now.first > 0) {
              if (!visited_temp[now.first-1][now.second] && boardNow[now.first-1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first-1, now.second);
              else if (!visited_temp[now.first-1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first-1, now.second);
            }
            if (now.first < n-1) {
              if (!visited_temp[now.first+1][now.second] && boardNow[now.first+1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first+1, now.second);
              else if (!visited_temp[now.first+1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first+1, now.second);
            }
            if (now.second > 0) {
              if (!visited_temp[now.first][now.second-1] && boardNow[now.first][now.second-1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second-1);
              else if (!visited_temp[now.first][now.second-1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second-1);
            }
            if (now.second < n-1) {
              if (!visited_temp[now.first][now.second+1] && boardNow[now.first][now.second+1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second+1);
              else if (!visited_temp[now.first][now.second+1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second+1);
            }
          }
        }

        int best_count_after = 0;
        for (int l2 = 1; l2 <= 9; l2++) {
          if (l2 == l) continue;
          if ((int) next_adj_temp[l2].size() > best_count_after) {
            best_count_after = next_adj_temp[l2].size();
          }
        }

        if ((int) next_adj[l].size() + best_count_after > best_count) {
          best_l = l;
          best_count = best_count_after;
        }
      }
      if (best_count == 0) break;

      auto checklist = next_adj[best_l];
      next_adj[best_l].clear();
      for (auto &x: checklist) {
        queue<Pii> que;
        que.emplace(x.first, x.second);
        while (!que.empty()) {
          auto now = que.front();
          que.pop();
          if (visited[now.first][now.second] && now != x) continue;
          visited[now.first][now.second] = true;
          if (boardNow[now.first][now.second] != best_l) next_adj[boardNow[now.first][now.second]].emplace_back(now.first, now.second);

          if (now.first > 0) {
            if (!visited[now.first-1][now.second] && boardNow[now.first-1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first-1, now.second);
            else if (!visited[now.first-1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first-1, now.second);
          }
          if (now.first < n-1) {
            if (!visited[now.first+1][now.second] && boardNow[now.first+1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first+1, now.second);
            else if (!visited[now.first+1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first+1, now.second);
          }
          if (now.second > 0) {
            if (!visited[now.first][now.second-1] && boardNow[now.first][now.second-1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second-1);
            else if (!visited[now.first][now.second-1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second-1);
          }
          if (now.second < n-1) {
            if (!visited[now.first][now.second+1] && boardNow[now.first][now.second+1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second+1);
            else if (!visited[now.first][now.second+1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second+1);
          }
        }
      }

      //ans.push_back(vector<int>({first_i+1, first_j+1, best_l}));
      nowColor = best_l;
      stepCount++;
    }

    if (stepCount < best_step) {
      best_i = first_i;
      best_j = first_j;
      best_step = stepCount;

      #ifdef DEBUG
      cerr << best_i << " " << best_j << " " << best_step << " " << theTimer.time() << endl;
      #endif
    }

    p++;
  }

  int finalStepCount = 0;
  {
    int first_i = best_i;
    int first_j = best_j;

    int stepCount = 0;
    auto boardNow = board;

    visited = vector<vector<bool> >(n, vector<bool>(n));
    vector<vector<Pii> > next_adj(10);

    queue<Pii> que;
    que.emplace(first_i, first_j);
    while (!que.empty()) {
      auto now = que.front();
      que.pop();
      if (visited[now.first][now.second]) continue;
      visited[now.first][now.second] = true;
      if (boardNow[now.first][now.second] != boardNow[first_i][first_j]) next_adj[boardNow[now.first][now.second]].emplace_back(now.first, now.second);

      if (now.first > 0) {
        if (!visited[now.first-1][now.second] && boardNow[now.first-1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first-1, now.second);
        else if (!visited[now.first-1][now.second] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first-1, now.second);
      }
      if (now.first < n-1) {
        if (!visited[now.first+1][now.second] && boardNow[now.first+1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first+1, now.second);
        else if (!visited[now.first+1][now.second] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first+1, now.second);
      }
      if (now.second > 0) {
        if (!visited[now.first][now.second-1] && boardNow[now.first][now.second-1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second-1);
        else if (!visited[now.first][now.second-1] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first, now.second-1);
      }
      if (now.second < n-1) {
        if (!visited[now.first][now.second+1] && boardNow[now.first][now.second+1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second+1);
        else if (!visited[now.first][now.second+1] && boardNow[now.first][now.second] == boardNow[first_i][first_j]) que.emplace(now.first, now.second+1);
      }
    }

    int nowColor = boardNow[first_i][first_j];

    while (true) {
      int best_l = 0;
      int best_count = 0;
      for (int l = 1; l <= 9; l++) {
        if (l == nowColor) continue;
        if ((int) next_adj[l].size() > best_count) {
          best_l = l;
          best_count = next_adj[l].size();
        }
      }
      if (best_count == 0) break;

      auto checklist = next_adj[best_l];
      next_adj[best_l].clear();
      for (auto &x: checklist) {
        queue<Pii> que;
        que.emplace(x.first, x.second);
        while (!que.empty()) {
          auto now = que.front();
          que.pop();
          if (visited[now.first][now.second] && now != x) continue;
          visited[now.first][now.second] = true;
          if (boardNow[now.first][now.second] != best_l) next_adj[boardNow[now.first][now.second]].emplace_back(now.first, now.second);

          if (now.first > 0) {
            if (!visited[now.first-1][now.second] && boardNow[now.first-1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first-1, now.second);
            else if (!visited[now.first-1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first-1, now.second);
          }
          if (now.first < n-1) {
            if (!visited[now.first+1][now.second] && boardNow[now.first+1][now.second] == boardNow[now.first][now.second]) que.emplace(now.first+1, now.second);
            else if (!visited[now.first+1][now.second] && boardNow[now.first][now.second] == best_l) que.emplace(now.first+1, now.second);
          }
          if (now.second > 0) {
            if (!visited[now.first][now.second-1] && boardNow[now.first][now.second-1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second-1);
            else if (!visited[now.first][now.second-1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second-1);
          }
          if (now.second < n-1) {
            if (!visited[now.first][now.second+1] && boardNow[now.first][now.second+1] == boardNow[now.first][now.second]) que.emplace(now.first, now.second+1);
            else if (!visited[now.first][now.second+1] && boardNow[now.first][now.second] == best_l) que.emplace(now.first, now.second+1);
          }
        }
      }

      ans.push_back(vector<int>({first_i+1, first_j+1, best_l}));
      nowColor = best_l;
      stepCount++;
    }

    finalStepCount = stepCount;
  }

  return finalStepCount;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  cin >> id >> n >> k;
  board = vector<vector<short> >(n, vector<short>(n));
  for (auto &x: board) {
    string s;
    cin >> s;
    for (int i = 0; i < n; i++) x[i] = s[i] - '0';
  }

  // preprocessing

  // optimization
  int score = solve3();

  cerr << "score     = " << score << endl;
  cerr << "timeTaken = " << theTimer.time() << endl;

  // postprocess & output
  cout << ans.size() << endl;
  for (auto &x: ans) cout << x[0] << " " << x[1] << " " << x[2] << endl;

  return 0;
}
