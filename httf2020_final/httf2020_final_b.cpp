#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("sse4.2")

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <map>
#include <queue>
#include <functional>
#include <numeric>
#include <random>
#include <chrono>
#include <cstring>
#include <climits>
#include <cassert>

using namespace std;

typedef long long int ll;

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUShortMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x000000000000ffff) * mod) >> 48;
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
  timer() {
    restart();
  }
  inline void restart() {
    t = now();
  }
  inline double time() {
    return now() - t;
  }
  inline double now() {
    unsigned long long l, h;
    __asm__ ("rdtsc" : "=a"(l), "=d"(h));
    return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9
  }
};

#define DEBUG

xorshift64 theRandom;
timer theTimer;
mt19937 theShuffle;

constexpr int n = 1000;
constexpr int s = 1000;

vector<int> x(n), y(n), c(n);
vector<int> k(s);
vector<vector<int> > p(s);

vector<vector<bool> > isUsable(n, vector<bool>(n));
vector<vector<int> > usableAdj(n);
vector<vector<int> > edgeCardinality(n, vector<int>(n));

vector<vector<int> > vertexAssignment(s);

vector<vector<int> > usedBy(n);

vector<vector<int> > bestEdgeCardinality(n, vector<int>(n));
vector<vector<int> > bestVertexAssignment(s);
vector<vector<int> > bestUsedBy(n);

int score, lastScore, bestScore;

int calcScore() {
  int score = 0;
  for (int t = 0; t < s; t++) {
    if (vertexAssignment[t][0] == -1) continue;
    int e = 0;
    for (int i = 0; i < k[t]; i++) {
      for (int j = i+1; j < k[t]; j++) {
        if (p[t][j] == i && edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][j]] == 0) {
          e = 3;
          break;
        }
        else if (p[t][j] != i && edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][j]] > 0) {
          e++;
          break;
        }
      }
      if (e >= 3) break;
    }
    if (e == 0) score += 100;
    else if (e == 1) score += 10;
    else if (e == 2) score += 1;
  }
  return score;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int _n, _s;
  cin >> _n >> _s;
  for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> c[i];
  for (int i = 0; i < s; i++) {
    cin >> k[i];
    p[i] = vector<int>(k[i]);
    for (int j = 1; j < k[i]; j++) {
      cin >> p[i][j];
      p[i][j]--; // zero-indexed
    }
  }

  // check if it's possible to create edge between i and j
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      // sqrt(x^2 + y^2) <= c[i] + c[j]
      // x^2 + y^2 <= (c[i] + c[j])^2
      int dx = x[i] - x[j];
      int dy = y[i] - y[j];
      int cs = c[i] + c[j];
      if (dx * dx + dy * dy <= cs * cs) {
        isUsable[i][j] = true;
        isUsable[j][i] = true;
      }
    }
  }

  // assign vertex
  for (int i = 0; i < s; i++) {
    vertexAssignment[i] = vector<int>(k[i], -1);
  }

  vector<pair<int, int> > degree(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) continue;
      if (isUsable[i][j]) {
        usableAdj[i].push_back(j);
        degree[i].first++;
      }
    }
    degree[i].second = i;
  }
  auto degreeSt = degree;
  sort(degree.rbegin(), degree.rend());

  vector<vector<int> > degreeNeeded(s);
  for (int i = 0; i < s; i++) {
    degreeNeeded[i] = vector<int>(k[i]);
    for (int j = i; j < k[i]; j++) {
      degreeNeeded[i][p[i][j]]++;
    }
  }

  vector<int> nextTarget(s);
  for (int i = 0; i < n; i++) nextTarget[i] = i;
  sort(nextTarget.begin(), nextTarget.end(), [&](auto &a, auto &b){return k[a] < k[b];});

  // create initial solution
  int nb = 0;
  int bb = 0;
  double tle = 4.500;
  while (theTimer.time() < tle) {
    for (int w = nb; w < s; w++) {
      int t = nextTarget[w];
      vector<int> v(k[t], -1);
      int root = 0;
      while (root < n) {
        v = vector<int>(k[t], -1);
        v[0] = degree[root].second;
        for (int i = 1; i < k[t]; i++) {
          for (auto &r: usableAdj[v[p[t][i]]]) {
            if (degreeSt[r].first < 2 * degreeNeeded[t][i]) continue;
            if (edgeCardinality[r][p[t][i]] == 0) continue;
            bool good = true;
            for (int j = 0; j < i; j++) {
              if (j == p[t][i]) continue;
              else if (edgeCardinality[r][v[j]] > 0 || r == v[j]) {
                good = false;
                break;
              }
            }
            if (!good) continue;
            int p1 = 0;
            int p2 = 0;
            while (p1 < usedBy[r].size() && p2 < usedBy[v[p[t][i]]].size()) {
              if (usedBy[r][p1] == usedBy[v[p[t][i]]][p2]) {
                int subj = usedBy[r][p1];
                good = false;
                for (int z = k[subj]-1; z > 0; z--) {
                  if ((vertexAssignment[subj][z] == r && vertexAssignment[subj][p[subj][z]] == v[p[t][i]]) || (vertexAssignment[subj][z] == v[p[t][i]] && vertexAssignment[subj][p[subj][z]] == r)) {
                    good = true;
                    break;
                  }
                }
                if (good) {
                  p1++;
                  p2++;
                }
                else break;
              }
              else if (usedBy[r][p1] < usedBy[v[p[t][i]]][p2]) p1++;
              else p2++;
            }
            if (good) {
              v[i] = r;
              break;
            }
          }
          if (v[i] == -1) {
            for (auto &r: usableAdj[v[p[t][i]]]) {
              if (degreeSt[r].first < 16 * degreeNeeded[t][i]) continue;
              if (edgeCardinality[r][p[t][i]] > 0) continue;
              bool good = true;
              for (int j = 0; j < i; j++) {
                if (j == p[t][i]) continue;
                else if (edgeCardinality[r][v[j]] > 0 || r == v[j]) {
                  good = false;
                  break;
                }
              }
              if (!good) continue;
              int p1 = 0;
              int p2 = 0;
              while (p1 < usedBy[r].size() && p2 < usedBy[v[p[t][i]]].size()) {
                if (usedBy[r][p1] == usedBy[v[p[t][i]]][p2]) {
                  int subj = usedBy[r][p1];
                  good = false;
                  for (int z = k[subj]-1; z > 0; z--) {
                    if ((vertexAssignment[subj][z] == r && vertexAssignment[subj][p[subj][z]] == v[p[t][i]]) || (vertexAssignment[subj][z] == v[p[t][i]] && vertexAssignment[subj][p[subj][z]] == r)) {
                      good = true;
                      break;
                    }
                  }
                  if (good) {
                    p1++;
                    p2++;
                  }
                  else break;
                }
                else if (usedBy[r][p1] < usedBy[v[p[t][i]]][p2]) p1++;
                else p2++;
              }
              if (good) {
                v[i] = r;
                break;
              }
            }
          }
          if (v[i] == -1) break;
        }
        if (v[k[t]-1] == -1) {
          root++;
          continue;
        }
        else break;
      }
      if (v[k[t]-1] == -1) break;
      vertexAssignment[t] = v;
      for (int i = 1; i < k[t]; i++) {
        edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]++;
        edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]++;
      }
      for (int i = 0; i < k[t]; i++) {
        usedBy[vertexAssignment[t][i]].push_back(t);
        sort(usedBy[vertexAssignment[t][i]].begin(), usedBy[vertexAssignment[t][i]].end());
      }
      nb = w+1;
    }
    if (nb > bb) {
      bestEdgeCardinality = edgeCardinality;
      bestVertexAssignment = vertexAssignment;
      bestUsedBy = usedBy;
      bb = nb;
    }
    else {
      edgeCardinality = bestEdgeCardinality;
      vertexAssignment = bestVertexAssignment;
      usedBy = bestUsedBy;
      nb = bb;
    }
    int retryFrom = max(0, nb - (int)theRandom.nextUIntMod(5));
    for (int w = retryFrom; w < nb; w++) {
      int t = nextTarget[w];
      for (int i = 0; i < k[t]; i++) {
        for (auto it = usedBy[vertexAssignment[t][i]].begin(); it != usedBy[vertexAssignment[t][i]].end(); it++) {
          if ((*it) == t) {
            usedBy[vertexAssignment[t][i]].erase(it);
            break;
          }
        }
      }
      for (int i = 1; i < k[t]; i++) {
        edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]--;
        edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]--;
      }
      vertexAssignment[t] = vector<int>(k[t], -1);
    }
    for (int i = 0; i < s; i++) shuffle(usableAdj[i].begin(), usableAdj[i].end(), theShuffle);
    nb = retryFrom;
  }
  edgeCardinality = bestEdgeCardinality;
  vertexAssignment = bestVertexAssignment;
  usedBy = bestUsedBy;


  score = calcScore();
  lastScore = score;
  bestScore = score;

  int iterCount = 0;
  double temperature = 1e-1;
  double decayRate = 1e-5;

  double timeLimit = 4.950;

  while (theTimer.time() < timeLimit) {
    if (bestScore == 100000) break; // full score gained
    iterCount++;

    int t = theRandom.nextUIntMod(s);
    if (vertexAssignment[t][0] == -1) {
      t = nextTarget[score/100];
      vector<int> v(k[t]);
      int a = 0;
      for (int i = 0; i < k[t]; i++) {
        a = 0;
        int r = theRandom.nextUIntMod(n);
        while (true) {
          if (a >= 1000) break;
          a++;
          r = theRandom.nextUIntMod(n);
          if (!isUsable[r][v[p[t][i]]]) continue;
          bool good = true;
          for (int j = 0; j < i; j++) {
            if (j == p[t][i]) continue;
            else if (edgeCardinality[r][v[j]] > 0 || r == v[j]) {
              good = false;
              break;
            }
          }
          if (!good) continue;
          int p1 = 0;
          int p2 = 0;
          while (p1 < usedBy[r].size() && p2 < usedBy[v[p[t][i]]].size()) {
            if (usedBy[r][p1] == usedBy[v[p[t][i]]][p2]) {
              int subj = usedBy[r][p1];
              good = false;
              for (int z = k[subj]-1; z > 0; z--) {
                if ((vertexAssignment[subj][z] == r && vertexAssignment[subj][p[subj][z]] == v[p[t][i]]) || (vertexAssignment[subj][z] == v[p[t][i]] && vertexAssignment[subj][p[subj][z]] == r)) {
                  good = true;
                  break;
                }
              }
              if (good) {
                p1++;
                p2++;
              }
              else break;
            }
            else if (usedBy[r][p1] < usedBy[v[p[t][i]]][p2]) p1++;
            else p2++;
          }
          if (good) break;
        }
        if (a >= 1000) break;
        v[i] = r;
      }
      if (a >= 1000) continue;

      vertexAssignment[t] = v;
      for (int i = 1; i < k[t]; i++) {
        edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]++;
        edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]++;
      }

      score = calcScore();

      //cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;

      if (score >= lastScore + 100) {
        for (int i = 0; i < k[t]; i++) {
          usedBy[vertexAssignment[t][i]].push_back(t);
          sort(usedBy[vertexAssignment[t][i]].begin(), usedBy[vertexAssignment[t][i]].end());
        }
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          bestEdgeCardinality = edgeCardinality;
          bestVertexAssignment = vertexAssignment;
          bestUsedBy = usedBy;
        }
      }
      else {
        for (int i = 0; i < k[t]; i++) {
          for (auto it = usedBy[vertexAssignment[t][i]].begin(); it != usedBy[vertexAssignment[t][i]].end(); it++) {
            if ((*it) == t) {
              usedBy[vertexAssignment[t][i]].erase(it);
              break;
            }
          }
        }
        for (int i = 1; i < k[t]; i++) {
          edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]--;
          edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]--;
        }
        vertexAssignment[t][0] = -1;
      }
    }
    else {
      for (int i = 0; i < k[t]; i++) {
        for (auto it = usedBy[vertexAssignment[t][i]].begin(); it != usedBy[vertexAssignment[t][i]].end(); it++) {
          if ((*it) == t) {
            usedBy[vertexAssignment[t][i]].erase(it);
            break;
          }
        }
      }
      for (int i = 1; i < k[t]; i++) {
        edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]--;
        edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]--;
      }

      vector<int> v(k[t]);
      int a = 0;
      for (int i = 0; i < k[t]; i++) {
        a = 0;
        int r = theRandom.nextUIntMod(n);
        while (true) {
          if (a >= 1000) break;
          a++;
          r = theRandom.nextUIntMod(n);
          if (!isUsable[r][v[p[t][i]]]) continue;
          bool good = true;
          for (int j = 0; j < i; j++) {
            if (j == p[t][i]) continue;
            else if (edgeCardinality[r][v[j]] > 0 || r == v[j]) {
              good = false;
              break;
            }
          }
          if (!good) continue;
          int p1 = 0;
          int p2 = 0;
          while (p1 < usedBy[r].size() && p2 < usedBy[v[p[t][i]]].size()) {
            if (usedBy[r][p1] == usedBy[v[p[t][i]]][p2]) {
              int subj = usedBy[r][p1];
              good = false;
              for (int z = 0; z < k[subj]-1; z++) {
                if ((vertexAssignment[subj][z] == r && vertexAssignment[subj][p[subj][z]] == v[p[t][i]]) || (vertexAssignment[subj][z] == v[p[t][i]] && vertexAssignment[subj][p[subj][z]] == r)) {
                  good = true;
                  break;
                }
              }
              if (good) {
                p1++;
                p2++;
              }
              else break;
            }
            else if (usedBy[r][p1] < usedBy[v[p[t][i]]][p2]) p1++;
            else p2++;
          }
          if (good) break;
        }
        if (a >= 1000) break;
        v[i] = r;
      }
      if (a >= 1000) {
        for (int i = 1; i < k[t]; i++) {
          edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]++;
          edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]++;
        }
        for (int i = 0; i < k[t]; i++) {
          usedBy[vertexAssignment[t][i]].push_back(t);
          sort(usedBy[vertexAssignment[t][i]].begin(), usedBy[vertexAssignment[t][i]].end());
        }
        continue;
      }

      vertexAssignment[t] = v;
      for (int i = 1; i < k[t]; i++) {
        edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]++;
        edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]++;
      }

      score = calcScore();

      //cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;

      if (score >= lastScore) {
        for (int i = 0; i < k[t]; i++) {
          usedBy[vertexAssignment[t][i]].push_back(t);
          sort(usedBy[vertexAssignment[t][i]].begin(), usedBy[vertexAssignment[t][i]].end());
        }
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          bestEdgeCardinality = edgeCardinality;
          bestVertexAssignment = vertexAssignment;
          bestUsedBy = usedBy;
        }
      }
      else {
        for (int i = 0; i < k[t]; i++) {
          for (auto it = usedBy[vertexAssignment[t][i]].begin(); it != usedBy[vertexAssignment[t][i]].end(); it++) {
            if ((*it) == t) {
              usedBy[vertexAssignment[t][i]].erase(it);
              break;
            }
          }
        }
        for (int i = 1; i < k[t]; i++) {
          edgeCardinality[vertexAssignment[t][i]][vertexAssignment[t][p[t][i]]]--;
          edgeCardinality[vertexAssignment[t][p[t][i]]][vertexAssignment[t][i]]--;
        }
        vertexAssignment[t][0] = -1;
      }
    }

  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "bestScore   = " << bestScore << endl;

  edgeCardinality = bestEdgeCardinality;
  vertexAssignment = bestVertexAssignment;
  usedBy = bestUsedBy;

  vector<pair<int, int> > ans;
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      if (edgeCardinality[i][j] > 0) ans.emplace_back(i+1, j+1);
    }
  }

  cout << ans.size() << endl;
  for (auto &x: ans) cout << x.first << " " << x.second << endl;
  for (int i = 0; i < s; i++) {
    if (vertexAssignment[i][0] == -1) {
      for (int j = 0; j < k[i]; j++) cout << j+1 << " ";
    }
    else {
      for (int j = 0; j < k[i]; j++) cout << vertexAssignment[i][j]+1 << " ";
    }
    cout << endl;
  }

  return 0;
}
