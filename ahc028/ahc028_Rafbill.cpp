#pragma GCC optimize "-O3,omit-frame-pointer,inline,unroll-all-loops,fast-math"
 #pragma GCC target "tune=native"
 #include <bits/stdc++.h>
 #include <sys/time.h>
// #ifndef CP_GEN
// #define BACKWARD_HAS_BFD 1
// #include "backward.hpp"
// backward::SignalHandling sh;
// #endif
using namespace std;
// Macros
using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
// Types
template<class T>
using min_queue = priority_queue<T, vector<T>, greater<T>>;
template<class T>
using max_queue = priority_queue<T>;
// Printing
template<class T>
void print_collection(ostream& out, T const& x);
template<class T, size_t... I>
void print_tuple(ostream& out, T const& a, index_sequence<I...>);
namespace std {
  template<class... A>
  ostream& operator<<(ostream& out, tuple<A...> const& x) {
    print_tuple(out, x, index_sequence_for<A...>{});
    return out;
  }
  template<class... A>
  ostream& operator<<(ostream& out, pair<A...> const& x) {
    print_tuple(out, x, index_sequence_for<A...>{});
    return out;
  }
  template<class A, size_t N>
  ostream& operator<<(ostream& out, array<A, N> const& x) { print_collection(out, x); return out; }
  template<class A>
  ostream& operator<<(ostream& out, vector<A> const& x) { print_collection(out, x); return out; }
  template<class A>
  ostream& operator<<(ostream& out, deque<A> const& x) { print_collection(out, x); return out; }
  template<class A>
  ostream& operator<<(ostream& out, multiset<A> const& x) { print_collection(out, x); return out; }
  template<class A, class B>
  ostream& operator<<(ostream& out, multimap<A, B> const& x) { print_collection(out, x); return out; }
  template<class A>
  ostream& operator<<(ostream& out, set<A> const& x) { print_collection(out, x); return out; }
  template<class A, class B>
  ostream& operator<<(ostream& out, map<A, B> const& x) { print_collection(out, x); return out; }
  template<class A, class B>
  ostream& operator<<(ostream& out, unordered_set<A> const& x) { print_collection(out, x); return out; }
}
template<class T, size_t... I>
void print_tuple(ostream& out, T const& a, index_sequence<I...>){
  using swallow = int[];
  out << '(';
  (void)swallow{0, (void(out << (I == 0? "" : ", ") << get<I>(a)), 0)...};
  out << ')';
}
template<class T>
void print_collection(ostream& out, T const& x) {
  int f = 0;
  out << '[';
  for(auto const& i: x) {
    out << (f++ ? "," : "");
    out << i;
  }
  out << "]";
}
// Random
struct RNG {
  uint64_t s[2];
  RNG(u64 seed) {
    reset(seed);
  }
  RNG() {
    reset(time(0));
  }
  static __attribute__((always_inline)) inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
  }
  inline void reset(u64 seed) {
    struct splitmix64_state {
      u64 s;
      u64 splitmix64() {
        u64 result = (s += 0x9E3779B97f4A7C15);
        result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
        result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
        return result ^ (result >> 31);
      }
    };
    splitmix64_state sm { seed };
    s[0] = sm.splitmix64();
    s[1] = sm.splitmix64();
  }
  uint64_t next() {
    const uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    const uint64_t result = rotl(s0 * 5, 7) * 9;
    s1 ^= s0;
    s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    s[1] = rotl(s1, 37); // c
    return result;
  }
  inline u32 randomInt32() {
    return next();
  }
  inline u64 randomInt64() {
    return next();
  }
  inline u32 random32(u32 r) {
    return (((u64)randomInt32())*r)>>32;
  }
  inline u64 random64(u64 r) {
    return randomInt64()%r;
  }
  inline u32 randomRange32(u32 l, u32 r) {
    return l + random32(r-l+1);
  }
  inline u64 randomRange64(u64 l, u64 r) {
    return l + random64(r-l+1);
  }
  inline double randomDouble() {
    return (double)randomInt32() / 4294967296.0;
  }
  inline float randomFloat() {
    return (float)randomInt32() / 4294967296.0;
  }
  inline double randomRangeDouble(double l, double r) {
    return l + randomDouble() * (r-l);
  }
  template<class T>
  void shuffle(vector<T>& v) {
    i32 sz = v.size();
    for(i32 i = sz; i > 1; i--) {
      i32 p = random32(i);
      swap(v[i-1],v[p]);
    }
  }
  template<class T>
  void shuffle(T* fr, T* to) {
    i32 sz = distance(fr,to);
    for(int i = sz; i > 1; i--) {
      int p = random32(i);
      swap(fr[i-1],fr[p]);
    }
  }
  template<class T>
  inline int sample_index(vector<T> const& v) {
    return random32(v.size());
  }
  template<class T>
  inline T sample(vector<T> const& v) {
    return v[sample_index(v)];
  }
} rng;
// Letrec
template<class Fun>
class letrec_result {
  Fun fun_;
  public:
    template<class T>
    explicit letrec_result(T &&fun): fun_(forward<T>(fun)) {}
    template<class ...Args>
    decltype(auto) operator()(Args &&...args) {
      return fun_(ref(*this), forward<Args>(args)...);
    }
};
template<class Fun>
decltype(auto) letrec(Fun &&fun) {
  return letrec_result<decay_t<Fun>>(forward<Fun>(fun));
}
// Timer
struct timer {
  chrono::high_resolution_clock::time_point t_begin;
  timer() {
    t_begin = chrono::high_resolution_clock::now();
  }
  void reset() {
    t_begin = chrono::high_resolution_clock::now();
  }
  float elapsed() const {
    return chrono::duration<float>(chrono::high_resolution_clock::now() - t_begin).count();
  }
};
// Util
template<class T>
T& smin(T& x, T const& y) { x = min(x,y); return x; }
template<class T>
T& smax(T& x, T const& y) { x = max(x,y); return x; }
template<typename T>
int sgn(T val) {
  if(val < 0) return -1;
  if(val > 0) return 1;
  return 0;
}
static inline
string int_to_string(int val, int digits = 0) {
  string s = to_string(val);
  reverse(begin(s), end(s));
  while((int)s.size() < digits) s.push_back('0');
  reverse(begin(s), end(s));
  return s;
}
// Debug
static inline void debug_impl_seq() {
  cerr << "}";
}
template <class T, class... V>
void debug_impl_seq(T const& t, V const&... v) {
  cerr << t;
  if(sizeof...(v)) { cerr << ", "; }
  debug_impl_seq(v...);
}

const double TL = 1.95;
timer TM;
const i32 N = 15;
const i32 M = 200;
const i32 K = 5;
i32 A[N][N];
i32 T[M][K];
i32 start;
i32 num_shared[M][M];
vector<u32> letters[26];
const i32 MAX_GROUPS = N*M;
u32 num_groups;
u32 group_base[MAX_GROUPS];
u32 group_end[MAX_GROUPS];
vector<u32> base_groups[M];
u32 dist_pre[MAX_GROUPS][K+1][N*N];
u32 dist_from[MAX_GROUPS][K+1][N*N];
u32 group_dist[MAX_GROUPS][MAX_GROUPS];
void read(){
  { i32 n,m; cin>>n>>m;
    do { if(!(n == N && m == M)) { throw runtime_error("main.cpp" ":" "30" " Assertion failed: " "n == N && m == M"); } } while(0);
  }
  i32 si,sj; cin>>si>>sj;
  start = si*N+sj;
  for(i32 i = 0; i < (i32)(N); ++i) for(i32 j = 0; j < (i32)(N); ++j) {
    char c; cin>>c;
    A[i][j] = c-'A';
  }
  for(i32 i = 0; i < (i32)(M); ++i) for(i32 j = 0; j < (i32)(K); ++j) {
    char c; cin>>c;
    T[i][j] = c-'A';
  }
}
void init_shared(){
  for(i32 a = 0; a < (i32)(M); ++a) for(i32 b = 0; b < (i32)(M); ++b) {
    for(i32 i = 0; i < (i32)(K+1); ++i) {
      bool ok = 1;
      for(i32 j = 0; j < (i32)(i); ++j) if(T[a][K-i+j] != T[b][j]) ok = 0;
      if(ok) num_shared[a][b] = i;
    }
  }
}
void init_letters() {
  for(i32 i = 0; i < (i32)(N); ++i) for(i32 j = 0; j < (i32)(N); ++j) {
    letters[A[i][j]].emplace_back(i*N+j);
  }
}
i32 dist_A(i32 x, i32 y) {
  return abs(x/N-y/N) + abs(x%N-y%N) + 1;
}
void init_groups(){
  group_base[0] = -1;
  group_end[0] = start;
  num_groups++;
  for(i32 i = 0; i < (i32)(M); ++i) for(i32 j = 0; j < (i32)(letters[T[i][K-1]].size()); ++j) {
    group_base[num_groups] = i;
    group_end[num_groups] = letters[T[i][K-1]][j];
    base_groups[i].emplace_back(num_groups);
    num_groups++;
  }
  do { if(!(num_groups <= MAX_GROUPS)) { throw runtime_error("main.cpp" ":" "74" " Assertion failed: " "num_groups <= MAX_GROUPS"); } } while(0);
  do { cerr << "main.cpp" ":" "75" "  {" << "num_groups" << "} = {"; debug_impl_seq(num_groups); cerr << endl << flush; } while(0);
  for(i32 a = 0; a < (i32)(num_groups); ++a) if(a) {
    for(i32 x = 0; x < (i32)(N*N); ++x) {
      dist_pre[a][K][x] = ((u32)x == group_end[a]) ? 0 : 1e9;
      dist_from[a][K][x] = group_end[a];
    }
    for(i32 i = (K-1); i >= (i32)(0); --i) {
      for(i32 x = 0; x < (i32)(N*N); ++x) {
        dist_pre[a][i][x] = 1e9;
        for(auto y : letters[T[group_base[a]][i]]) {
          auto d = dist_pre[a][i+1][y] + dist_A(x,y);
          if(d < dist_pre[a][i][x]) {
            dist_pre[a][i][x] = d;
            dist_from[a][i][x] = y;
          }
        }
      }
    }
  }
  group_dist[0][0] = 0;
  for(i32 b = 0; b < (i32)(num_groups); ++b) if(b) {
    group_dist[0][b] = dist_pre[b][0][start];
  }
  for(i32 a = 0; a < (i32)(num_groups); ++a) if(a) {
    for(i32 b = 0; b < (i32)(num_groups); ++b) {
      if(b) {
        i32 i = num_shared[group_base[a]][group_base[b]];
        group_dist[a][b] = dist_pre[b][i][group_end[a]];
      }else{
        group_dist[a][b] = 0;
      }
    }
  }
}
void init(){
  init_shared();
  init_letters();
  init_groups();
}
struct sa_t {
  float done;
  float temp0;
  float alpha;
  sa_t() = default;
  sa_t(double t0, double a) {
    temp0 = t0;
    alpha = a;
    done = 0;
  }
  bool accept(float x) {
    bool r = (x < 0) || rng.randomDouble() * pow(1-done, alpha) * temp0 > x;
    return r;
  }
};
struct state {
  u32 data[M+2];
  u32 score;
  void random_init(){
    data[0] = 0;
    for(i32 i = 0; i < (i32)(M); ++i) data[i+1] = rng.sample(base_groups[i]);
    rng.shuffle(data+1, data+M+1);
    data[M+1] = 0;
    score = 0;
    for(i32 i = 0; i < (i32)(M+1); ++i) score += group_dist[data[i]][data[i+1]];
  }
  void check_score() const {
    u32 rscore = 0;
    for(i32 i = 0; i < (i32)(M+1); ++i) rscore += group_dist[data[i]][data[i+1]];
    do { if(!(rscore == score)) { throw runtime_error("main.cpp" ":" "152" " Assertion failed: " "rscore == score"); } } while(0);
  }
  void opt3(sa_t& SA){
    u32 a,b,c;
    c = 2 + rng.random32(M-1);
    b = 1 + rng.random32(c-1);
    a = rng.random32(b);
    i32 delta = 0;
    delta -= group_dist[data[a]][data[a+1]];
    delta -= group_dist[data[b]][data[b+1]];
    delta -= group_dist[data[c]][data[c+1]];
    delta += group_dist[data[a]][data[b+1]];
    delta += group_dist[data[b]][data[c+1]];
    delta += group_dist[data[c]][data[a+1]];
    if(SA.accept(delta)) {
      score += delta;
      rotate(data+a+1, data+b+1, data+c+1);
      // check_score();
    }
  }
  void change_end(sa_t& SA){
    u32 a = 1 + rng.random32(M);
    u32 x = rng.sample(base_groups[group_base[data[a]]]);
    i32 delta = 0;
    delta -= group_dist[data[a-1]][data[a]];
    delta -= group_dist[data[a]][data[a+1]];
    delta += group_dist[data[a-1]][x];
    delta += group_dist[x][data[a+1]];
    if(SA.accept(delta)) {
      score += delta;
      data[a] = x;
    }
  }
  void print(){
    u32 last = start;
    vector<u32> ans;
    for(i32 a0 = 0; a0 < (i32)(M); ++a0) {
      auto a = data[a0], b = data[a0+1];
      i32 i = a0 == 0 ? 0 : num_shared[group_base[a]][group_base[b]];
      while(i < K) {
        last = dist_from[b][i][last];
        ans.push_back(last);
        i += 1;
      }
    }
    for(auto p : ans) {
      cout << (p/N) << ' ' << (p%N) << '\n';
    }
    cout << flush;
  }
};
void solve(){
  state bestS; bestS.random_init();
  state S = bestS;
  u64 niter = 0;
  sa_t SA(10, 1.0);
  while(1) {
    niter++;
    if(niter % 1024 == 0) {
      if(TM.elapsed() > TL) break;
      SA.done = TM.elapsed() / TL;
    }
    u32 ty = rng.random32(10);
    if(ty == 0) {
      S.change_end(SA);
    }else{
      S.opt3(SA);
    }
    if(S.score < bestS.score) {
      bestS = S;
      do { cerr << "main.cpp" ":" "235" "  {" << "\"new\", niter, bestS.score" << "} = {"; debug_impl_seq("new", niter, bestS.score); cerr << endl << flush; } while(0);
    }
  }
  do { cerr << "main.cpp" ":" "238" "  {" << "niter" << "} = {"; debug_impl_seq(niter); cerr << endl << flush; } while(0);
  do { cerr << "main.cpp" ":" "240" "  {" << "bestS.score" << "} = {"; debug_impl_seq(bestS.score); cerr << endl << flush; } while(0);
  bestS.print();
}
i32 main(){
  ios::sync_with_stdio(false); cin.tie(0);
  TM.reset();
  read();
  init();
  solve();
  cerr << "Elapsed: " << TM.elapsed() << endl;
  cerr << "[DATA] time = " << TM.elapsed() << endl;
  return 0;
}
