#ifdef __GNUC__
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <random>
#include <functional>
#include <memory>
#include <cmath>
#include <cassert>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

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
    #ifdef _MSC_VER
      #ifdef LOCAL
        return __rdtsc() * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return __rdtsc() * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        //return __rdtsc() * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        return __rdtsc() * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
      #endif
    #else
      unsigned long long l, h;
        __asm__ ("rdtsc" : "=a"(l), "=d"(h));
      #ifdef LOCAL
        return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
      #else
        //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
        //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
        //return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
        return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C6i (Xeon Platinum 8375C)
      #endif
    #endif
  }
};

using namespace std;

typedef long long int ll;
typedef unsigned long long int ull;
typedef pair<int, int> Pii;
typedef unsigned char uchar;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937_64 theMersenne(1);

// libraries
namespace Lib {
  class NeuralNet {
  // NN実行用クラス
  // データ(NCHW)・重みレイアウト(IOHW)などはpytorchと同じにしてある
  public:
    inline static int idx_1d(const int i) {return i;}
    inline static int idx_2d(const int i, const int j, const int jx) {return i * jx + j;}

    class Layer {
    public:
      vector<shared_ptr<Layer>> input_layers;
      vector<float> output;
      bool output_calculated;
      virtual void forward() = 0;
      Layer() {
        output_calculated = false;
      }
      void update_output() {
        if (output_calculated) return;
        for (auto &layer: input_layers) {
          layer->update_output();
        }
        forward();
        output_calculated = true;
      }
      void reset_output() {
        if (!output_calculated) return;
        for (auto &layer: input_layers) {
          layer->reset_output();
        }
        output_calculated = false;
      }
    };

    class Input: public Layer {
    public:
      vector<float> input;
      Input() {}
      Input(const vector<float>& input) {
        this->input = input;
      }
      void forward() {
        assert(input_layers.empty());
        output.resize(input.size());
        for (int i = 0; i < (int) input.size(); i++) {
          output[i] = input[i];
        }
      }
    };

    class Conv1d: public Layer {
    // kernel size == 1で固定
    private:
      int in_ch;
      int out_ch;
      vector<float> weight;
      vector<float> bias;
    public:
      Conv1d(const int in_ch, const int out_ch, const vector<float>& weight, const vector<float>& bias) {
        assert(in_ch > 0);
        assert(out_ch > 0);
        assert((int) weight.size() == in_ch * out_ch);
        assert((int) bias.size() == out_ch);
        this->in_ch = in_ch;
        this->out_ch = out_ch;
        this->weight = weight;
        this->bias = bias;
      }
      void forward() override {
        assert((int) input_layers.size() == 1);
        const vector<float>& input_tensor = input_layers[0]->output;
        assert((int) input_tensor.size() % in_ch == 0);

        int ix = (int) input_tensor.size() / in_ch;
        output.resize(out_ch * ix);

        for (int i = 0; i < ix; i++) {
          for (int oc = 0; oc < out_ch; oc++) {
            output[idx_2d(oc, i, ix)] = bias[idx_1d(oc)];
            for (int ic = 0; ic < in_ch; ic++) {
              output[idx_2d(oc, i, ix)] += input_tensor[idx_2d(ic, i, ix)] * weight[idx_2d(ic, oc, out_ch)];
            }
          }
        }
      }
    };

    class Linear: public Layer {
    private:
      int in_ch;
      int out_ch;
      vector<float> weight;
      vector<float> bias;
    public:
      Linear(const int in_ch, const int out_ch, const vector<float>& weight, const vector<float>& bias) {
        assert(in_ch > 0);
        assert(out_ch > 0);
        assert((int) weight.size() == in_ch * out_ch);
        assert((int) bias.size() == out_ch);
        this->in_ch = in_ch;
        this->out_ch = out_ch;
        this->weight = weight;
        this->bias = bias;
      }
      void forward() override {
        assert((int) input_layers.size() == 1);
        const vector<float>& input_tensor = input_layers[0]->output;
        assert((int) input_tensor.size() == in_ch);

        output.resize(out_ch);

        for (int oc = 0; oc < out_ch; oc++) {
          output[idx_1d(oc)] = bias[idx_1d(oc)];
          for (int ic = 0; ic < in_ch; ic++) {
            output[idx_1d(oc)] += input_tensor[idx_1d(ic)] * weight[idx_2d(ic, oc, out_ch)];
          }
        }
      }
    };

    class ReLU: public Layer {
    public:
      ReLU() {}
      void forward() override {
        assert((int) input_layers.size() == 1);
        const vector<float>& input_tensor = input_layers[0]->output;

        int ix = (int) input_tensor.size();
        output.resize(ix);

        for (int i = 0; i < ix; i++) {
          output[idx_1d(i)] = max(0.0f, input_tensor[idx_1d(i)]);
        }
      }
    };

    class Concat: public Layer {
    // concatを行う次元は第2次元(CHWのH)で固定
    private:
      int ch_num;
    public:
      Concat(const int ch_num) {
        assert(ch_num > 0);
        this->ch_num = ch_num;
      }
      void forward() override {
        assert((int) input_layers.size() >= 1);
        int output_size_per_ch = 0;
        for (const auto& input_layer: input_layers) {
          assert((int) input_layer->output.size() % ch_num == 0);
          output_size_per_ch += (int) input_layer->output.size() / ch_num;
        }
        output.resize(ch_num * output_size_per_ch);
        for (int ch = 0; ch < ch_num; ch++) {
          int i = 0;
          for (const auto& input_layer: input_layers) {
            for (int ii = 0; ii < (int) input_layer->output.size() / ch_num; ii++) {
              output[idx_2d(ch, i, output_size_per_ch)] = input_layer->output[idx_2d(ch, ii, (int) input_layer->output.size() / ch_num)];
              i++;
            }
          }
        }
      }
    };

    class Transpose: public Layer {
    // transposeを行う次元は第1次元・第2次元の間で固定
    private:
      int ch_num; // transpose前の第1次元の大きさ
    public:
      Transpose(const int ch_num) {
        assert(ch_num > 0);
        this->ch_num = ch_num;
      }
      void forward() override {
        assert((int) input_layers.size() == 1);
        const vector<float>& input_tensor = input_layers[0]->output;

        assert((int) input_tensor.size() % ch_num == 0);
        int size_per_ch = (int) input_tensor.size() / ch_num;
        output.resize(input_tensor.size());

        for (int ch = 0; ch < ch_num; ch++) {
          for (int i = 0; i < size_per_ch; i++) {
            output[idx_2d(i, ch, ch_num)] = input_tensor[idx_2d(ch, i, size_per_ch)];
          }
        }
      }
    };

    class Add: public Layer {
    public:
      Add() {}
      void forward() override {
        assert((int) input_layers.size() == 2);
        const vector<float>& input_tensor_0 = input_layers[0]->output;
        const vector<float>& input_tensor_1 = input_layers[1]->output;
        assert(input_tensor_0.size() == input_tensor_1.size());
        
        output.resize(input_tensor_0.size());

        for (int i = 0; i < (int) input_tensor_0.size(); i++) {
          output[idx_1d(i)] = input_tensor_0[idx_1d(i)] + input_tensor_1[idx_1d(i)];
        }
      }
    };

    class Softmax: public Layer {
    public:
      Softmax() {}
      void forward() override {
        assert((int) input_layers.size() == 1);
        const vector<float>& input_tensor = input_layers[0]->output;

        output.resize(input_tensor.size());

        float max_val = -1e30f;
        for (int i = 0; i < (int) input_tensor.size(); i++) {
          max_val = max(max_val, input_tensor[idx_1d(i)]);
        }
        float exp_sum = 0.0f;
        for (int i = 0; i < (int) input_tensor.size(); i++) {
          float exp_val = exp(input_tensor[idx_1d(i)] - max_val);
          output[idx_1d(i)] = exp_val;
          exp_sum += exp_val;
        }
        for (int i = 0; i < (int) input_tensor.size(); i++) {
          output[idx_1d(i)] /= exp_sum;
        }
      }
    };
  };
}

// hyper parameters
namespace HyperParameter {
  double money_efficiency_factor = 0.546260114179096;
  double worth_finish_threshold_factor = 0.9599318319706979;
  double wasted_power_penalty_factor = 0.5195423542140517;
  double insufficient_power_penalty_factor = 0.718051159465462;
  double work_all_utilization_rate_threshold = 0.5178315618971655;
  double worth_buy_work_single_efficiency_threshold_factor = 1.1514333326038413;
  double worth_buy_work_all_efficiency_threshold_factor = 1.4004751824228796;
  double worth_buy_cancel_hand_num_threshold_factor = 0.3774333842575627;
  ll worth_buy_cancel_price_threshold_factor = 3;
  ll worth_buy_invest_extra_money_threshold_factor = 0;
  ll worth_buy_invest_price_threshold_factor = 428;
  int invest_stop_turn = 888;
  int buy_stop_turn = 995;
  int monte_carlo_simulation_turn = 30;
  int monte_carlo_score_calculation_turn = 30;
  int monte_carlo_money_only_score_turn = 0;
  double monte_carlo_work_single_worth = 0.5150802117168888;
  double monte_carlo_work_all_worth = 0.2989364156609707;
  int monte_carlo_interval_turn = 14;
  void load_hyper_parameter(int argc, char *argv[]) {
    if (argc >  1) money_efficiency_factor = strtod(argv[1], nullptr);
    if (argc >  2) worth_finish_threshold_factor = strtod(argv[2], nullptr);
    if (argc >  3) wasted_power_penalty_factor = strtod(argv[3], nullptr);
    if (argc >  4) insufficient_power_penalty_factor = strtod(argv[4], nullptr);
    if (argc >  5) work_all_utilization_rate_threshold = strtod(argv[5], nullptr);
    if (argc >  6) worth_buy_work_single_efficiency_threshold_factor = strtod(argv[6], nullptr);
    if (argc >  7) worth_buy_work_all_efficiency_threshold_factor = strtod(argv[7], nullptr);
    if (argc >  8) worth_buy_cancel_hand_num_threshold_factor = strtod(argv[8], nullptr);
    if (argc >  9) worth_buy_cancel_price_threshold_factor = atoll(argv[9]);
    if (argc > 10) worth_buy_invest_extra_money_threshold_factor = atoll(argv[10]);
    if (argc > 11) worth_buy_invest_price_threshold_factor = atoll(argv[11]);
    if (argc > 12) invest_stop_turn = atoi(argv[12]);
    if (argc > 13) buy_stop_turn = atoi(argv[13]);
    if (argc > 14) monte_carlo_simulation_turn = atoi(argv[14]);
    if (argc > 15) monte_carlo_score_calculation_turn = atoi(argv[15]);
    if (argc > 16) monte_carlo_money_only_score_turn = atoi(argv[16]);
    if (argc > 17) monte_carlo_work_single_worth = strtod(argv[17], nullptr);
    if (argc > 18) monte_carlo_work_all_worth = strtod(argv[18], nullptr);
    if (argc > 19) monte_carlo_interval_turn = atoi(argv[19]);
  }
}

// enums
enum CardType {
  WorkSingle = 0,
  WorkAll = 1,
  CancelSingle = 2,
  CancelAll = 3,
  Invest = 4
};

std::istream& operator>>(std::istream& is, CardType& o) {
  int tmp;
  if (is >> tmp) o = static_cast<CardType>(tmp);
  return is;
}

// structs
struct Card {
  CardType type;
  ll work_power;
  ll price;

  Card(): type(WorkSingle), work_power(0), price(0) {}
  Card(CardType type, ll work_power, ll price): type(type), work_power(work_power), price(price) {}
};

struct Project {
  ll work_remaining;
  ll reward;

  Project(): work_remaining(0), reward(0) {}
  Project(ll work_remaining, ll reward): work_remaining(work_remaining), reward(reward) {}
};

// constants
struct Consts {
  static constexpr int max_invest_num = 20;
  static constexpr double global_time_limit = 1.950;

  static constexpr int max_hand_card_num = 7;
  static constexpr int max_project_num = 8;
  static constexpr int max_deck_card_num = 5;
};

// inputs
struct InitialInput {
  int hand_card_num;
  int project_num;
  int deck_card_num;
  int turn_max;
  vector<Card> initial_hand_cards;
  vector<Project> initial_projects;
};

struct TurnInput {
  vector<Project> project_status;
  ll current_money;
  vector<Card> deck_cards;
};

// outputs
struct UseCardOutput {
  int card_id;
  int target_project_id;

  UseCardOutput(): card_id(0), target_project_id(0) {}
  UseCardOutput(int card_id, int target_project_id): card_id(card_id), target_project_id(target_project_id) {}
};

struct GetCardOutput {
  int card_id;

  GetCardOutput(): card_id(0) {}
  GetCardOutput(int card_id): card_id(card_id) {}
};

// internals
struct GameEnvironment {
  int hand_card_num;
  int project_num;
  int deck_card_num;
  int turn_max;

  GameEnvironment(int hand_card_num, int project_num, int deck_card_num, int turn_max)
    : hand_card_num(hand_card_num), project_num(project_num), deck_card_num(deck_card_num), turn_max(turn_max) {}
};

struct GameState {
  ll money;
  vector<Card> hand_cards;
  vector<Project> projects;
  int current_turn;
  ll invest_count;

  GameState(ll money, vector<Card> hand_cards, vector<Project> projects)
    : money(money), hand_cards(hand_cards), projects(projects), current_turn(0), invest_count(0) {}
};

struct MonteCarloEnvironment {
  static constexpr int card_pool_size = 2048;
  static constexpr ll card_work_power_min = 1;
  static constexpr ll card_work_power_max = 50;
  static constexpr ll card_work_price_min = 1;
  static constexpr ll card_work_price_max = 10000;
  static constexpr double card_work_price_stddev_factor = 3.0;

  static constexpr int project_pool_size = 2048;
  static constexpr double project_work_factor_min = 2.0;
  static constexpr double project_work_factor_max = 8.0;
  static constexpr double project_reward_factor_min = 0.0;
  static constexpr double project_reward_factor_max = 10.0;
  static constexpr double project_reward_stddev_factor = 0.5;

  static constexpr ll cancel_price_min = 0;
  static constexpr ll cancel_price_max = 10;

  vector<Card> work_single_card_pool;
  vector<Card> work_all_card_pool;
  vector<Project> project_pool;

  MonteCarloEnvironment(const GameEnvironment& game_environment) {
    work_single_card_pool = vector<Card>(card_pool_size);
    for (int c = 0; c < card_pool_size; c++) {
      ll work_power = theRandom.nextUIntMod(card_work_power_max - card_work_power_min + 1) + card_work_power_min;
      ll price = clamp((ll) round(normal_distribution((double) work_power, (double) work_power / card_work_price_stddev_factor)(theMersenne)), card_work_price_min, card_work_price_max);
      work_single_card_pool[c] = Card(WorkSingle, work_power, price);
    }
    work_all_card_pool = vector<Card>(card_pool_size);
    for (int c = 0; c < card_pool_size; c++) {
      ll work_power = theRandom.nextUIntMod(card_work_power_max - card_work_power_min + 1) + card_work_power_min;
      ll price = clamp((ll) round(normal_distribution((double) (work_power * game_environment.project_num), (double) (work_power * game_environment.project_num) / card_work_price_stddev_factor)(theMersenne)), card_work_price_min, card_work_price_max);
      work_all_card_pool[c] = Card(WorkAll, work_power, price);
    }
    project_pool = vector<Project>(project_pool_size);
    for (int p = 0; p < project_pool_size; p++) {
      double work_factor = project_work_factor_min + theRandom.nextDouble() * (project_work_factor_max - project_work_factor_min);
      ll work_remaining = (ll) round(pow(2.0, work_factor));
      ll reward = (ll) round(pow(2.0, clamp(normal_distribution(work_factor, project_reward_stddev_factor)(theMersenne), project_reward_factor_min, project_reward_factor_max)));
      project_pool[p] = Project(work_remaining, reward);
    }
  }
};

struct MonteCarloState {
  int work_single_weight = 4;
  int work_all_weight = 2;
  int cancel_single_weight = 2;
  int cancel_all_weight = 1;

  void update_weight(const GameEnvironment& game_environment, const TurnInput& turn_input) {
    for (int c = 1; c < game_environment.deck_card_num; c++) {
      const auto& card = turn_input.deck_cards[c];
      switch (card.type) {
        case WorkSingle:
          work_single_weight++;
          break;
        case WorkAll:
          work_all_weight++;
          break;
        case CancelSingle:
          cancel_single_weight++;
          break;
        case CancelAll:
          cancel_all_weight++;
          break;
        default:
          break;
      }
    }
  }

  Project create_random_project(const GameState& game_state, const MonteCarloEnvironment& monte_carlo_environment) const {
    const int p = theRandom.nextUIntMod(monte_carlo_environment.project_pool_size);
    const auto& base_project = monte_carlo_environment.project_pool[p];
    return Project(
      base_project.work_remaining << game_state.invest_count,
      base_project.reward << game_state.invest_count
    );
  }

  Card create_random_card(const GameState& game_state, const MonteCarloEnvironment& monte_carlo_environment) const {
    int roll = theRandom.nextUIntMod(work_single_weight + work_all_weight + cancel_single_weight + cancel_all_weight);
    if (roll < work_single_weight) {
      const int c = theRandom.nextUIntMod(monte_carlo_environment.card_pool_size);
      const auto& base_card = monte_carlo_environment.work_single_card_pool[c];
      return Card(
        base_card.type,
        base_card.work_power << game_state.invest_count,
        base_card.price << game_state.invest_count
      );
    }
    else if (roll < work_single_weight + work_all_weight) {
      const int c = theRandom.nextUIntMod(monte_carlo_environment.card_pool_size);
      const auto& base_card = monte_carlo_environment.work_all_card_pool[c];
      return Card(
        base_card.type,
        base_card.work_power << game_state.invest_count,
        base_card.price << game_state.invest_count
      );
    }
    else if (roll < work_single_weight + work_all_weight + cancel_single_weight) {
      ll base_price = (ll) theRandom.nextUIntMod(monte_carlo_environment.cancel_price_max - monte_carlo_environment.cancel_price_min + 1) + monte_carlo_environment.cancel_price_min;
      return Card(
        CancelSingle,
        0,
        base_price << game_state.invest_count
      );
    }
    else {
      ll base_price = (ll) theRandom.nextUIntMod(monte_carlo_environment.cancel_price_max - monte_carlo_environment.cancel_price_min + 1) + monte_carlo_environment.cancel_price_min;
      return Card(
        CancelAll,
        0,
        base_price << game_state.invest_count
      );
    }
  }
};

struct GetCardOutputNN {
  const vector<float> hand_card_conv_1_weight = {-0.10287783, -0.01908713, -0.33325759,  0.33466738,  0.17119932,
        -0.04089164, -0.29705039, -1.54272103,  0.67165864, -0.18781912,
        -0.25511834, -0.01243169,  0.19717811, -0.13328354,  0.65271425,
         0.60032731, -0.84265465, -0.66760600,  0.51012707,  0.07747476,
         0.52488869,  0.08928225, -0.18268891,  0.09183789, -0.16165105,
         0.17471558,  0.19543467, -0.49899986,  0.14358006, -0.63132852,
        -0.75524187, -0.19432080, -0.88974708, -2.36420679, -0.28306076,
         0.20897175, -0.16631585,  0.01177451, -0.05883073, -0.22931471,
        -0.10106787, -0.25057140, -0.12926349, -0.04297473, -0.11807015,
        -0.09361841, -0.05618687, -0.28148088, -0.42374775, -0.94195205,
         0.01365242, -0.02511757, -0.04134110,  0.26088321,  0.28489339,
        -0.49687791,  0.17957850,  0.18612294, -0.12935600, -0.20926276,
         0.95631146,  0.16679393, -0.48782492, -0.19432119, -0.35230446,
        -0.48218396, -0.45352176, -0.46949580, -2.36955500, -0.23345633,
         0.19153039,  0.16630073, -0.09615719, -0.05921767,  0.06936213,
         0.10167990, -0.27164197,  0.12484617, -0.21125980, -0.22368154,
        -0.00690881,  0.08682368,  0.01722866,  0.11737216, -0.46386597,
        -0.47161603, -0.44338632,  0.25719553, -0.43534255,  0.13672900,
        -0.36057127,  0.26690820,  0.31272143, -0.33519566,  0.26374525,
        -0.25083491, -0.09696452, -0.07445428, -0.27851120, -0.37397227,
         0.42172772,  0.03821802, -0.30253616, -0.10435791,  0.23022257,
        -0.24980797, -0.59434563,  0.13911994, -0.51030380, -0.67868859,
        -0.92189616, -0.37959892};
  const vector<float> hand_card_conv_1_bias = {-0.15654773, -0.14888255, -0.08897321, -0.45751363, -0.08377287,
        -0.00498876,  0.10311733, -0.77044171, -0.43175319, -0.61762887,
        -0.36605489, -0.32066306, -0.09078799, -0.34640816, -0.14658566,
        -0.17417009};
  const vector<float> hand_card_conv_2_weight = {2.34776139e-02, -5.68087399e-02, -2.12139085e-01,  1.45352036e-01,
        -1.02547929e-01,  6.58876747e-02, -1.09076641e-01,  1.30107924e-01,
        -2.10383325e-03,  1.04192555e-01, -1.58366486e-01,  1.89519852e-01,
         2.08468288e-02, -3.20664048e-02,  8.00014809e-02, -1.16768487e-01,
        -4.99697924e-02, -4.21057820e-01,  3.75491418e-02, -2.69152784e+00,
        -1.21286869e+00,  1.25823349e-01, -1.88225344e-01,  6.33007288e-01,
         5.98051131e-01, -3.27110469e-01,  1.85781315e-01,  4.66190418e-03,
        -3.70761842e-01,  1.45991951e-01, -1.04778326e+00, -3.89211625e-02,
        -1.57467008e-01,  1.83633268e-02,  6.10736191e-01,  2.84201026e-01,
        -4.29600143e+00,  1.28218368e-01, -4.34330814e-02,  5.89174867e-01,
        -1.46712840e-01,  8.66294280e-02,  2.25121677e-01,  4.19189706e-02,
        -5.26321054e-01,  1.77413136e-01,  1.04747698e-01, -1.49157733e-01,
         1.20823294e-01,  2.59549022e-01,  4.15496290e-01, -3.80225033e-02,
        -5.06448388e-01,  1.91546883e-02, -6.02355748e-02,  8.64695013e-02,
         1.64243549e-01, -1.40639067e-01,  1.10858038e-01,  2.88574919e-02,
        -1.18895836e-01, -1.66605353e-01,  4.92484681e-02, -1.34108588e-01,
         2.18380034e-01,  1.68597907e-01, -9.71352756e-02,  1.81953851e-02,
        -1.24692619e-01, -4.97815013e-02,  1.75311565e-01, -1.21971615e-01,
        -1.15880758e-01,  4.37287986e-02,  2.16410756e-01, -1.60204303e-02,
        -1.16412990e-01, -2.44684637e-01,  4.32436392e-02, -1.04070216e-01,
         2.05048561e-01, -9.82125551e-02,  7.18587190e-02, -3.08342189e-01,
        -1.19046204e-01,  1.76418200e-01, -9.85501781e-02,  8.60403776e-02,
        -4.64621894e-02, -6.04087785e-02,  6.48367479e-02,  2.61789020e-02,
         4.89095077e-02, -2.03882694e-01,  2.67800838e-02, -1.78887755e-01,
        -3.17539871e-02,  1.29289567e-01, -2.83767492e-01, -2.44221196e-01,
        -1.44820750e-01,  1.24547847e-01,  8.54125619e-02, -1.57517761e-01,
         1.74865365e-01,  1.13988675e-01,  2.35201299e-01, -2.28885382e-01,
        -8.16254541e-02,  3.21259499e-02,  1.48672089e-01,  1.07745722e-01,
         1.43412679e-01, -6.12320378e-02, -1.20654964e+00, -1.79362297e-01,
         9.07226086e-01, -8.05469602e-02,  6.40663803e-02, -3.56181234e-01,
        -1.65691562e-02,  4.28011686e-01,  1.12752445e-01, -5.95147945e-02,
        -1.71862133e-02,  2.19173312e-01, -6.54445365e-02,  2.56954163e-01,
        -6.16520941e-02, -1.56456858e-01,  8.51584077e-02, -3.20139080e-01,
         6.86864331e-02, -1.40276074e-01,  6.12028837e-02, -1.04985707e-01,
        -7.07128048e-02, -2.63739657e-03, -1.36401519e-01, -7.33855665e-02,
         1.62266754e-02, -1.23414904e-01,  9.13723707e-02, -1.63389444e-01,
        -1.62778258e-01, -1.69787526e-01, -2.79588908e-01, -7.12204278e-02,
         1.37074858e-01,  1.40701234e-01, -2.35854357e-01, -1.75784990e-01,
         7.34459236e-02, -2.36397654e-01,  1.34864300e-01, -2.23940104e-01,
        -2.06165418e-01, -6.48329556e-02,  1.31041378e-01,  5.89490645e-02,
         1.24488890e-01,  9.27876532e-02, -3.73696595e-01, -3.38853806e-01,
        -8.19465816e-02, -7.54305720e-02, -3.64483178e-01, -1.15088403e-01,
         8.13152343e-02, -3.55239481e-01, -1.48314014e-01, -1.84607312e-01,
        -6.78809211e-02,  1.45560414e-01,  1.25138992e-02, -8.17826670e-03,
         3.38947773e-02, -1.00983627e-01, -6.20279610e-02,  3.15174945e-02,
         2.13039592e-01, -1.17611833e-01,  1.80577859e-01,  1.19340211e-01,
        -2.56156474e-01, -3.78925446e-03,  1.07886061e-01,  9.46274400e-02,
        -2.62685180e-01, -1.48055494e-01,  1.06883362e-01, -1.41666442e-01,
         3.56614292e-02, -1.03854649e-01,  7.54996657e-01, -4.06718105e-01,
         1.00039995e+00,  1.44256726e-01, -4.96700144e+00, -2.08355516e-01,
         3.25613141e-01,  1.25776637e+00,  4.50582132e-02, -1.04880743e-01,
        -3.43551564e+00,  1.51863724e-01, -5.69672918e+00,  2.15906262e-01,
        -1.00483388e-01,  5.53133845e-01,  6.13512136e-02,  5.85906915e-02,
        -3.07600379e-01, -1.11866621e-02,  2.37596095e-01,  8.29501390e-01,
        -3.36790860e-01, -4.23255295e-01, -1.87377870e-01, -1.60132110e-01,
         3.93120535e-02, -1.76677495e-01, -7.55960792e-02, -2.00557068e-01,
         1.80362970e-01, -1.32853523e-01, -2.40559503e-01, -2.58351505e-01,
         4.84522246e-02, -1.89511776e-01, -1.75767541e-01,  1.88019395e-01,
         5.57745919e-02,  8.59610662e-02, -1.31484985e-01, -1.12510502e-01,
         4.84198742e-02,  8.59913528e-02, -4.15783346e-01, -3.08732018e-02,
        -9.11659300e-02, -2.26234937e+00, -6.76110536e-02, -8.24532807e-02,
        -3.81949842e-01,  1.83068693e-01, -5.70389175e+00, -4.50076014e-01,
         2.46584788e-01,  6.73979595e-02,  4.59560640e-02, -1.67829022e-01,
        -4.56406139e-02, -6.42566979e-02,  4.52039659e-01,  2.42615730e-01};
  const vector<float> hand_card_conv_2_bias = {-0.28911003, -0.27372706, -0.30398655,  0.26672897, -0.30496812,
        -0.15940863, -0.27764186,  0.78867471, -0.34894058, -0.13336317,
        -0.05256807, -0.23978671,  0.49978754, -0.17590874, -0.13151956,
         0.38520238};
  const vector<float> project_conv_1_weight = {0.02711332, -0.10267448,  0.26436096,  1.84114325, -0.27267453,
         0.91998118, -0.49945700,  0.71902573,  0.55513531, -0.01316956,
        -0.42174926, -0.31700069,  0.40302134,  0.72807407,  0.89947361,
        -2.81243658,  0.17298704, -1.45664382,  1.16860521, -2.20630121,
        -0.34028256,  0.95308179,  0.25449648, -0.51307422,  0.55331451,
        -1.39443767, -0.24321240,  0.82105178,  0.83356512, -2.12842894,
         1.51266944,  0.04024612,  0.76426810,  0.09797642, -0.30637410,
         0.32264081,  0.32886198, -0.28599349,  0.93897754,  1.55870855,
        -0.77419460,  0.08046525, -0.01142344, -0.15771787, -0.90249687,
         2.05478406, -2.77119708, -0.58813578};
  const vector<float> project_conv_1_bias = {-3.74349833e-01,  4.88941878e-01, -1.26218051e-01, -5.56184351e-01,
        -6.11153431e-04, -5.43330371e-01, -6.48618996e-01, -1.05506885e+00,
         1.07587166e-01, -1.53478205e+00, -4.96352725e-02, -1.35740370e-01,
        -4.88977060e-02, -6.54994743e-03,  5.26255548e-01, -1.52836889e-01};
  const vector<float> project_conv_2_weight = {-2.85674840e-01, -1.90006107e-01,  5.54806627e-02, -1.48910940e-01,
        -2.93315351e-01,  1.30506337e-01, -1.88701153e-01, -1.51096255e-01,
        -2.47106984e-01,  7.82369524e-02,  1.01276390e-01, -3.69910151e-01,
        -1.90036446e-01, -3.36270928e-02, -9.22695398e-02,  8.40630829e-02,
         3.03955555e-01,  1.06869447e+00, -3.82948190e-01,  5.30736744e-02,
        -3.46897006e-01,  1.31644893e+00, -3.05793476e+00,  8.29235464e-03,
         1.04485106e+00,  1.24417949e+00, -6.65812492e-01,  1.22998345e+00,
         5.00412762e-01, -7.83622786e-02, -2.34726596e+00, -2.56999350e+00,
        -2.89166421e-01, -2.93819249e-01, -1.19591072e-01, -1.46540642e-01,
        -1.23874322e-01,  1.86747741e-02,  1.25369981e-01,  2.44317949e-02,
        -8.77973288e-02, -2.45577380e-01, -1.70088813e-01, -2.55650282e-01,
        -4.05032754e-01, -8.57433975e-02,  1.43899061e-02, -1.19807176e-01,
        -1.76229939e-01, -1.67438880e-01, -3.49433899e-01,  2.00257450e-01,
         9.91985723e-02,  2.34513879e-02, -1.12604611e-01,  5.87712489e-02,
         8.41889605e-02,  3.18325609e-02, -1.36452522e-02, -1.83385089e-01,
        -2.41038483e-02, -1.87340602e-01,  1.04534775e-01,  3.14020216e-02,
        -1.78672343e-01, -2.19371423e-01, -1.41535029e-01, -2.24449575e-01,
         4.33742115e-03, -1.50686139e-02, -1.46265579e-02,  1.12340361e-01,
         8.64189640e-02, -9.83399600e-02, -5.70154190e-03, -2.53022879e-01,
        -5.78299239e-02,  5.86107820e-02,  1.40318409e-01, -5.40013462e-02,
         7.28062451e-01, -1.03629291e+00,  7.32856214e-01,  2.23539650e-01,
         5.04291534e-01, -2.30792975e+00,  4.07873487e+00, -1.75750172e+00,
        -9.39275563e-01, -9.50895488e-01,  6.39259815e-01,  8.14703226e-01,
         1.79077899e+00, -8.67096424e-01, -6.62128210e-01, -8.38480282e+00,
        -1.66802958e-01,  4.09556404e-02, -1.65072232e-02,  6.40036464e-02,
        -5.77693023e-02,  1.73242688e-01, -9.89061296e-02,  4.49830899e-03,
         1.40214330e-02,  1.13053038e-03, -2.35274300e-01, -1.22854531e-01,
        -2.07418025e-01, -1.48306906e-01,  1.95859432e-01, -7.68903792e-02,
        -2.69986808e-01, -9.74447802e-02, -8.66751596e-02,  8.70498419e-02,
        -1.48774683e-01,  4.81373295e-02, -1.57007232e-01, -2.01087054e-02,
        -5.42706028e-02, -1.68868601e-02,  4.21788692e-02, -2.61452720e-02,
         1.29134938e-01,  1.09501071e-01,  1.83969334e-01, -1.33181363e-01,
        -9.95273888e-02, -1.78307429e-01, -5.27515709e-02, -3.86663675e-02,
        -2.62956005e-02,  1.30898684e-01,  7.27345347e-02, -9.17482823e-02,
        -2.99945354e-01,  9.76124331e-02, -5.66426069e-02, -7.43928254e-02,
        -9.78625044e-02,  1.70950130e-01,  3.74552906e-02, -2.50314385e-01,
         5.97336181e-02, -2.73205340e-01, -1.24773718e-01, -5.60178459e-02,
        -2.25951344e-01, -2.18154877e-01, -7.40259439e-02, -2.27072015e-02,
        -1.12317875e-01, -1.59121856e-01,  6.13561459e-02, -1.84410691e-01,
         1.93825856e-01, -2.57143974e-02, -6.68995380e-02,  7.82569349e-02,
         6.36377871e-01,  6.78124428e-02,  1.13829350e+00, -1.50455236e-01,
         1.40162754e+00, -1.36631060e+01,  8.61405277e+00, -6.64017081e-01,
        -6.45899057e-01, -1.22163129e+00, -1.49060857e+00, -6.07529581e-01,
         1.51952350e+00, -3.16429675e-01, -1.32185948e+00, -1.01884632e+01,
        -1.81923643e-01,  1.54603624e+00,  4.55229789e-01, -8.02341402e-02,
        -1.60148668e+00,  3.35203838e+00, -1.07990944e+00,  4.55390543e-01,
        -8.47182646e-02, -1.20815003e+00, -3.18640083e-01, -5.59548698e-02,
        -2.83528149e-01, -1.04022002e+00,  9.60643232e-01,  6.39522374e-01,
         5.75946048e-02, -4.27368730e-01, -4.71580714e-01,  1.43713325e-01,
         3.90587538e-01, -7.89937496e-01, -1.07009172e+00, -7.33017921e-03,
         3.74502569e-01, -2.36447766e-01,  4.13856864e-01,  1.26071647e-01,
         1.93389937e-01, -3.22857797e-01, -8.32646370e-01,  1.22603822e+00,
        -5.87429069e-02, -3.75909090e-01,  1.96771517e-01, -4.02684510e-02,
        -1.44057805e-02, -1.25609493e+00, -2.46474648e+00, -4.63040888e-01,
        -5.67863166e-01,  2.27089524e-01, -5.80093488e-02, -5.41525960e-01,
         1.32361025e-01,  1.24261528e-01, -1.49883485e+00,  4.79830652e-01,
         5.01708873e-02, -5.30822754e-01, -6.70006394e-01,  2.20911741e-01,
         9.83791277e-02, -1.70863128e+00,  1.85709804e-01,  1.58442080e-01,
         1.59331828e-01,  3.61563474e-01,  2.47803882e-01, -3.22250947e-02,
        -4.32318933e-02,  5.59408486e-01,  8.15220892e-01, -3.99112068e-02,
         1.53434724e-01, -1.21834111e+00,  2.11027458e-01,  5.87483943e-02,
         9.05370355e-01, -1.64727902e+00, -3.77253675e+00, -8.56477499e-01,
        -4.47241962e-01, -1.72035658e+00,  1.84435093e+00,  2.57215858e-01,
         6.90307856e-01, -2.29729557e+00,  3.55543166e-01, -3.24564457e-01};
  const vector<float> project_conv_2_bias = {-0.10920168,  0.48981726, -0.03369982, -0.14750774, -0.03008478,
         0.91148698, -0.29803780, -0.22022980, -0.35341156, -0.17533085,
         0.68651003,  0.62625676,  0.50516498,  1.02138937,  0.69242036,
         0.81202036};
  const vector<float> deck_card_conv_1_weight = {-2.97398329e-01,  2.19826564e-01, -4.30723459e-01,  1.49965361e-01,
        -1.47045657e-01,  1.12812030e+00, -1.47434664e+00,  1.24312150e+00,
        -3.11500162e-01, -2.00262219e-01,  3.12904298e-01, -2.64934480e-01,
        -6.14393651e-01, -1.08998217e-01,  6.69091582e-01, -2.96374965e+00,
         1.83374357e+00, -9.09662426e-01, -5.07081628e-01,  9.01515782e-01,
        -6.61583126e-01, -1.07584894e+00,  3.88926566e-02, -9.58384350e-02,
        -1.26674330e+00,  1.25667620e+00, -2.43695408e-01, -8.06197673e-02,
         1.22169577e-01, -1.08502507e-01, -3.16541851e-01, -2.81976163e-02,
         6.26941696e-02,  8.54407474e-02, -1.68087944e-01, -3.68160039e-01,
        -9.18533146e-01,  6.03931725e-01, -3.87228876e-01, -9.23485458e-02,
        -2.02185297e+00, -1.73839164e+00,  1.71278334e+00,  4.47842777e-02,
        -7.22988322e-02,  7.88699031e-01,  2.14669704e-01,  6.17753327e-01,
         3.68773878e-01, -6.35366082e-01,  3.40275884e-01,  2.03439474e-01,
        -1.32601500e+00,  5.52494884e-01,  2.15395644e-01,  6.55828416e-01,
        -4.23393160e-01, -1.27923238e+00, -1.51392007e+00, -2.75051022e+00,
         1.67803079e-01, -4.20836478e-01, -3.39155465e-01,  2.08187103e-01,
         1.09091163e-01, -2.36660600e-01, -2.95848697e-01, -3.29881042e-01,
        -1.31793618e-01, -6.17293529e-02, -2.78665245e-01, -2.90820420e-01,
        -1.20019093e-01,  1.83051556e-01,  1.12663162e+00, -7.66142368e-01,
         1.95950940e-01,  8.14553261e-01, -7.66691744e-01, -1.48792875e+00,
         6.68340385e-01, -7.06390321e-01, -8.67719769e-01,  5.48737228e-01,
        -8.89053404e-01, -2.79038382e+00, -3.73044521e-01,  1.47508681e+00,
         3.87482494e-01, -3.07474554e-01, -4.70233411e-02, -3.44063818e-01,
        -1.20677352e-01, -5.76181412e-01,  1.66737810e-01,  3.32110822e-01,
        -4.60432768e+00,  4.53058988e-01, -6.57381833e-01,  3.29674304e-01,
        -4.08606672e+00,  1.86158895e-01, -1.35451540e-01, -1.32414758e+00,
         1.08213007e+00,  3.37980121e-01,  8.91613901e-01, -1.07662179e-01,
         1.36296044e-03,  1.82727143e-01,  4.48522042e-04, -4.30289596e-01,
        -2.73543739e+00, -3.23299795e-01,  3.41667747e+00,  1.05268431e+00,
         1.12276949e-01, -3.63623530e-01, -1.08560777e+00, -8.39413285e-01,
         1.40083289e+00,  5.05934656e-01, -7.89200068e-01,  9.12806317e-02,
        -2.03395057e+00,  7.11134136e-01,  9.98090029e-01,  1.90313026e-01,
        -4.22508925e-01,  4.16175097e-01,  1.16911657e-01, -2.71522254e-01,
         9.32229757e-01, -1.14394414e+00, -4.91086602e-01, -4.53420639e-01,
        -3.06573123e-01, -4.09525000e-02, -3.97610605e-01, -1.91240355e-01,
         5.85384429e-01, -3.62537861e+00,  1.09612978e+00, -6.80290103e-01};
  const vector<float> deck_card_conv_1_bias = {-0.47806618, -1.11679256, -0.63396907, -0.32175329,  0.23292974,
         0.47683209, -0.54598540, -0.22644544, -0.23230122,  0.81740046,
        -0.99765885, -0.00147018, -0.02914002, -0.12968819,  0.76637703,
        -0.81112778};
  const vector<float> deck_card_conv_2_weight = {5.03659248e-01, -1.86869666e-01,  5.37551083e-02,  1.03923284e-01,
         1.00396566e-01, -1.64235997e+00, -1.27246350e-01, -2.15268016e-01,
         5.40836692e-01,  8.83117020e-01,  2.52799988e-01, -2.71038949e-01,
        -6.35203838e-01, -2.13572145e+00, -3.61791641e-01,  8.42369735e-01,
        -1.67451397e-01,  2.73774415e-01, -2.52346218e-01,  1.54896960e-01,
        -1.24363653e-01,  4.65481319e-02,  7.16718063e-02,  6.53551519e-02,
        -4.26456600e-01,  6.82768971e-02, -1.68769807e-01, -8.44565779e-02,
        -1.87522009e-01,  5.22295460e-02, -3.15715432e-01, -3.01743656e-01,
        -8.85749757e-02, -2.12180233e+00,  1.26092225e-01,  7.92046189e-02,
        -2.69516468e-01,  4.65500236e-01,  1.69561103e-01, -2.01634943e-01,
        -6.44492134e-02,  2.63059407e-01, -1.01402247e+00,  7.00484693e-01,
         2.77918071e-01,  9.77781951e-01, -1.11860859e+00, -1.14011729e+00,
        -5.08762956e-01,  8.53658676e-01,  4.79985267e-01, -2.69140303e-01,
        -3.37439835e-01,  9.86861363e-02,  5.30644953e-01, -1.32180303e-01,
         3.31212521e-01, -3.69804323e-01,  3.61556602e+00, -7.38687754e-01,
        -2.43335152e+00,  4.84269321e-01, -6.86239815e+00,  1.41476548e+00,
        -5.09426951e-01,  4.91827995e-01, -3.88383530e-02, -4.05373462e-02,
        -3.56142581e-01, -1.84387788e-01, -4.09292668e-01, -4.38843966e-02,
         1.65892821e-02, -2.84562379e-01,  2.37050819e+00, -3.79254580e-01,
        -1.43362486e+00, -7.52619624e-01, -9.84160364e-01,  1.14990413e+00,
        -1.65954437e-02, -5.70441484e-02, -2.00910747e-01, -2.02278927e-01,
        -8.01962093e-02, -7.19770193e-01, -3.13788325e-01,  6.55641854e-02,
        -3.91963124e-01, -1.06768630e-01,  1.39931545e-01,  1.11744888e-01,
        -1.50967345e-01,  5.18468022e-02,  9.77334306e-02,  2.43887931e-01,
        -6.12170458e-01,  8.01397040e-02,  9.70237702e-02,  1.05269328e-01,
         4.60835472e-02, -2.66237110e-02,  7.16871396e-02, -2.29956537e-01,
        -4.91563082e+00, -4.02917445e-01, -2.00567227e-02, -4.08277959e-01,
        -1.11019149e-01, -3.89908820e-01,  4.36299562e-01, -6.61197186e-01,
        -1.13699771e-01,  1.83174327e-01,  1.60472751e-01, -1.31609961e-01,
         6.63701475e-01, -4.80931066e-02,  6.88058376e-01,  2.33316451e-01,
         3.74262065e-01,  2.83448935e-01,  2.15870714e+00, -1.41618252e-01,
         2.27845311e-02, -2.67784357e+00, -4.24482107e+00,  3.13162714e-01,
         2.42393389e-01,  5.48608840e-01, -3.09457183e-01,  1.02360956e-01,
         4.01296943e-01,  1.96347892e-01, -7.76259422e-01,  1.74979240e-01,
        -3.15009862e-01,  1.32370099e-01,  1.31330526e+00, -5.76552212e-01,
        -1.71806946e-01, -2.15978146e+00, -5.70559931e+00,  1.18214464e+00,
        -4.38679600e+00, -1.52269328e+00,  5.45580447e-01, -9.69021693e-02,
         1.88536823e+00, -1.10645676e+00,  1.16082501e+00,  1.66833788e-01,
        -3.44307327e+00,  1.24296057e+00, -2.59749055e+00, -6.62825465e-01,
        -1.42749321e+00, -4.96966410e+00,  2.25280643e-01, -6.56279027e-01,
         1.29532909e+00, -7.69206822e-01, -1.30536878e+00, -5.22477329e-02,
        -3.64105493e-01,  4.63475525e-01,  1.60524929e+00,  2.28744775e-01,
        -8.36901546e-01,  2.09409103e-01, -6.83072209e-01, -6.07946455e-01,
         5.80236986e-02, -8.50347336e-03,  5.85500710e-02,  2.66360134e-01,
        -1.42834365e-01,  2.45043948e-01, -6.23670444e-02,  1.30373329e-01,
         1.33360522e-02, -3.32326204e-01, -4.53914031e-02, -6.65709376e-03,
        -3.10320646e-01,  1.26057029e-01, -1.29035369e-01, -2.30053961e-01,
        -2.00037181e-01, -1.20338105e-01, -2.19460912e-02, -1.26467019e-01,
        -2.85319895e-01,  7.92117655e-01,  8.88858214e-02, -2.88167521e-02,
         3.03592950e-01,  3.57869267e-01,  6.34669960e-01, -2.01514691e-01,
        -1.55569643e-01, -2.54848003e+00,  1.44490874e+00,  1.13619141e-01,
        -5.84735692e-01, -4.24277496e+00, -5.14290428e+00,  7.05880448e-02,
         1.51447606e+00,  9.85025764e-01,  1.19479954e+00, -9.37617868e-02,
        -2.26242280e+00, -1.12082326e+00, -1.27384782e+00,  1.85804993e-01,
        -8.35804790e-02, -6.24131739e-01,  3.52898896e-01, -1.45859134e+00,
         2.68001258e-01, -2.76379257e-01,  3.25397968e-01,  1.73970342e-01,
        -1.38425040e+00, -2.78567165e-01, -8.20416272e-01,  1.23511299e-01,
         2.42994145e-01,  4.11919177e-01,  5.28256238e-01, -1.06123507e-01,
         9.50332060e-02, -2.48613432e-02,  3.43136525e+00,  1.18241394e+00,
         1.50093675e-01,  7.53755808e-01, -1.04145741e+00,  1.38426840e+00,
        -1.93958968e-01, -1.44429719e+00, -1.22614288e+00, -3.36318016e-01,
        -1.24060370e-01,  6.32153094e-01,  1.97480989e+00, -7.62366354e-02,
        -1.55949801e-01,  2.28097820e+00,  1.85359621e+00, -1.62826598e-01,
        -1.47444212e+00, -3.70078385e-01,  1.80301809e+00,  7.66723812e-01};
  const vector<float> deck_card_conv_2_bias = {0.08044079, -0.37042633,  2.97395706, -0.73323697, -0.23177101,
        -0.39613274,  0.52753031, -0.61197191,  0.04649861,  0.72139090,
         1.55344999, -0.36004391,  0.07583855,  1.13982904,  3.27725554,
         1.55547333};
  const vector<float> combined_conv_1_weight = {1.24136060e-01,  1.83659181e-01,  5.40176988e-01,  3.79632384e-01,
        -7.97469735e-01, -3.14433947e-02, -3.17921221e-01, -2.42804304e-01,
        -3.31561774e-01,  5.83840236e-02,  2.40053479e-02,  6.11740910e-02,
         9.76360776e-03,  6.30388930e-02,  3.09465546e-02,  1.38299838e-01,
         4.46215309e-02, -2.29893684e-01, -1.32046029e-01, -2.82540143e-01,
        -2.82393098e-01,  3.99089903e-02,  2.41390541e-02,  7.11987391e-02,
        -1.13127597e-01, -4.46062051e-02, -2.20541488e-02, -3.82749364e-02,
        -3.64557691e-02, -8.84234086e-02, -1.26715377e-01, -8.41729403e-01,
        -7.87695706e-01, -8.03856969e-01, -8.21582139e-01, -7.97424555e-01,
        -8.26952755e-01, -8.53247583e-01, -6.98638380e-01,  2.16717333e-01,
        -1.57302058e+00,  2.62233138e-01,  2.55193233e-01,  2.56600022e-01,
         2.24370599e+00,  9.05023795e-03,  1.00635004e+00,  9.79254246e-01,
         1.00657535e+00,  1.10168374e+00,  1.09824467e+00,  1.03101325e+00,
         1.02249265e+00,  5.88844478e-01,  6.09770775e-01,  6.10684156e-01,
         6.38535261e-01,  6.22353554e-01,  5.87456346e-01,  5.67082286e-01,
         7.11622119e-01,  2.86565155e-01,  4.87925142e-01, -1.15728021e+00,
         3.38095427e-01,  3.69686186e-01,  3.88572328e-02,  6.80861995e-02,
        -5.35251945e-02, -4.49298695e-02,  5.35849296e-02, -6.75255284e-02,
        -5.32744341e-02, -1.37854189e-01, -1.15073942e-01, -9.80668664e-01,
        -9.23749924e-01, -9.81729686e-01, -8.77665520e-01, -9.37956989e-01,
        -9.36238647e-01, -8.63953054e-01, -9.76464808e-01,  3.09815049e-01,
         2.80458838e-01,  2.85181373e-01, -1.87222672e+00,  2.87098825e-01,
         1.32513988e+00,  3.55322249e-02,  1.04412639e+00,  1.15139461e+00,
         1.05794883e+00,  1.19558501e+00,  1.13316905e+00,  1.08400798e+00,
         1.14466739e+00,  7.79043436e-01,  6.17518067e-01,  5.42962313e-01,
         7.22157240e-01,  6.37277246e-01,  7.11739361e-01,  5.80742002e-01,
         7.57530808e-01,  5.40057778e-01,  3.97024363e-01,  3.73656809e-01,
         1.53369144e-01, -1.12774634e+00,  1.45681989e+00, -5.37257316e-03,
         5.69659948e-01,  7.55171239e-01,  7.60693312e-01,  6.28034711e-01,
         7.68071771e-01,  7.48187661e-01,  7.99738944e-01,  4.56672311e-01,
         3.60928953e-01,  5.43235302e-01,  4.84170377e-01,  6.51110232e-01,
         4.44548756e-01,  6.53726339e-01,  4.10458505e-01,  5.69703043e-01,
        -2.29221374e-01, -5.68228364e-01,  6.41367555e-01, -9.80096757e-02,
         2.57934313e-02,  6.73059374e-02, -1.11230150e-01, -7.00107291e-02,
        -8.57488364e-02, -1.45251989e-01,  4.95743100e-03, -3.10762655e-02,
        -1.89044829e-02, -8.05516243e-01, -7.74872720e-01, -8.04772973e-01,
        -8.61415982e-01, -7.94409573e-01, -8.46304893e-01, -7.81067848e-01,
        -8.56559098e-01,  2.36586928e-01,  2.54682988e-01,  2.40841493e-01,
         2.63073504e-01, -1.62819397e+00,  2.23448920e+00,  1.37934247e-02,
         1.02510011e+00,  9.91195798e-01,  1.00171483e+00,  1.12261713e+00,
         1.08113790e+00,  1.00083482e+00,  1.04591215e+00,  6.25752330e-01,
         6.01826251e-01,  6.28787935e-01,  6.99549258e-01,  5.90419948e-01,
         6.13434792e-01,  5.65948069e-01,  6.46769583e-01,  3.40662479e-01,
        -1.10203052e+00,  4.17693496e-01,  3.75937998e-01,  3.25839221e-01,
         1.86218035e+00,  1.15769301e-02,  9.78686094e-01,  1.05355680e+00,
         1.03180206e+00,  1.03386927e+00,  1.10326493e+00,  9.93370533e-01,
         1.05591989e+00,  5.73926806e-01,  6.13733232e-01,  6.12809062e-01,
         6.87072873e-01,  6.28475904e-01,  6.48955524e-01,  6.29369378e-01,
         6.71360552e-01, -1.17270064e+00,  3.77709657e-01,  4.01889324e-01,
         3.31474155e-01,  3.84177744e-01,  2.14496732e+00,  3.30049284e-02,
         9.61979091e-01,  9.45045590e-01,  9.57747161e-01,  1.06183851e+00,
         9.84747469e-01,  9.43072617e-01,  9.65740144e-01,  6.03258014e-01,
         6.15393460e-01,  5.92067659e-01,  6.21439934e-01,  6.07875228e-01,
         5.29846549e-01,  6.22785866e-01,  5.97181022e-01,  2.88334131e-01,
         3.89073193e-01,  4.24944043e-01, -1.09428108e+00,  3.46159548e-01,
         6.80596232e-02,  6.77206889e-02, -3.64545099e-02, -5.19242696e-02,
        -6.92969337e-02, -8.59587453e-03, -4.02031541e-02, -5.37098348e-02,
        -4.46315445e-02, -7.53878117e-01, -9.88071918e-01, -8.52542102e-01,
        -8.81395757e-01, -8.15571487e-01, -9.07781422e-01, -8.75741243e-01,
        -8.31487834e-01, -1.74662447e+00,  2.26326495e-01,  2.62300760e-01,
         2.51750022e-01,  2.92798102e-01,  2.43414903e+00, -2.22615339e-03,
         8.31480384e-01,  7.18873799e-01,  9.33398008e-01,  8.77969265e-01,
         8.41628551e-01,  8.53580892e-01,  8.47934783e-01,  4.59050357e-01,
         5.50735593e-01,  5.59122384e-01,  5.50969899e-01,  5.10485172e-01,
         5.22435188e-01,  6.46820843e-01,  5.22031128e-01,  2.28473302e-02,
         3.50051343e-01,  4.48460400e-01,  5.26646316e-01, -9.47756410e-01,
         1.64112222e+00, -1.98504422e-02,  7.91038692e-01,  1.00330639e+00,
         9.47380364e-01,  5.48577666e-01,  9.15681839e-01,  8.01092565e-01,
         8.93180013e-01,  5.12309492e-01,  5.51486135e-01,  7.14681566e-01,
         4.98644471e-01,  5.21042645e-01,  6.76398396e-01,  5.55172205e-01,
         6.60043716e-01,  7.86299646e-01, -1.58860922e-01, -3.47053945e-01,
        -4.57235157e-01,  6.11137211e-01, -5.78215607e-02,  2.26222768e-01,
         3.50388139e-01,  7.98281193e-01,  1.83242425e-01, -1.99454010e-01,
        -2.63851006e-02,  5.42085111e-01,  5.08121967e-01,  2.69120514e-01,
         2.07377926e-01,  3.83141875e-01,  3.06000095e-02,  5.86337030e-01,
         2.66520500e-01,  3.33185732e-01,  1.71635538e-01,  3.67758498e-02,
        -4.24603194e-01,  6.76350296e-01, -1.25820354e-01,  7.44180754e-02,
         8.96843374e-02,  4.13360037e-02, -3.94565687e-02, -6.58731535e-02,
         4.60126251e-03, -1.23079345e-01, -3.23120132e-03, -3.24950181e-02,
        -3.46474461e-02, -8.52017105e-01, -8.87467802e-01, -9.14179087e-01,
        -8.69116843e-01, -8.17311823e-01, -9.23309088e-01, -9.10857975e-01,
        -8.35467815e-01,  2.47509554e-01,  2.78620243e-01, -1.63817656e+00,
         2.60783762e-01,  2.60386586e-01, -1.45816207e-01, -7.02534139e-01,
         7.62897193e-01,  3.83525401e-01,  1.05058157e+00,  1.08164144e+00,
         5.96457601e-01,  5.63799918e-01,  6.71955109e-01,  7.21134484e-01,
         6.26512349e-01,  3.68799537e-01,  3.79888088e-01,  4.62425381e-01,
         5.50619423e-01,  5.48686802e-01,  4.10099328e-01, -3.71129632e-01,
        -1.00777306e-01, -4.86240506e-01,  6.10513747e-01,  7.75600851e-01};
  const vector<float> combined_conv_1_bias = {0.17399465, 2.10232711, 1.50658917, 2.39896631, 0.96175426, 0.82695293,
        2.16609144, 1.51943219, 1.24749136, 1.46740973, 2.24315834, 1.57914257,
        1.04402530, 0.71478474, 2.17362952, 0.91352803};
  const vector<float> combined_conv_2_weight = {-1.59654260e-01,  3.72717008e-02, -6.66590361e-03,  3.95731367e-02,
        -2.10221335e-01,  1.11245766e-01,  1.49206117e-01, -3.39469835e-02,
         2.13637188e-01,  2.57381760e-02,  3.06690708e-02, -3.23458761e-02,
         5.54821966e-03, -1.36668179e-02, -2.81419419e-02,  7.19050132e-03,
         1.40525341e+00,  2.91134249e-02,  5.82116365e-01, -2.62787700e-01,
        -1.25308108e+00, -2.12174773e-01, -1.37293756e-01,  7.08688855e-01,
        -8.11273873e-01, -7.66098976e-01, -1.15052827e-01, -3.02594334e-01,
         2.58850157e-01,  1.57625580e+00, -7.47978687e-01,  1.29071400e-01,
         7.27159619e-01, -1.26764798e+00,  5.18315971e-01,  7.71739304e-01,
        -1.15683579e+00,  8.02671671e-01,  4.80890304e-01,  3.24969701e-02,
        -1.55005187e-01, -2.95436174e-01,  1.94619685e-01,  7.95517564e-01,
        -6.27441034e-02, -1.79400295e-02, -4.99305218e-01, -6.44547582e-01,
        -2.05954957e+00,  2.04148926e-02,  9.41852331e-01, -8.74861300e-01,
        -1.23988308e-01, -1.34006190e+00,  1.93274260e+00, -7.62329772e-02,
        -1.54641426e+00,  1.59847999e+00,  1.45573676e+00, -8.16646290e+00,
        -2.92621851e+00,  1.08897054e+00,  3.69820309e+00, -1.40341043e+00,
        -1.35298359e+00,  3.37631196e-01,  6.73029482e-01,  4.49712992e-01,
        -1.28941512e+00,  1.69837296e+00, -6.18766367e-01,  5.58020353e-01,
         8.35892677e-01,  6.09308124e-01, -4.76619095e-01, -8.64491820e-01,
         1.13173939e-01, -8.27576578e-01,  6.57152608e-02, -9.76275131e-02,
         9.84901607e-01,  2.61282146e-01, -3.26140523e-01, -1.02443540e+00,
         6.40138388e-01,  3.17384481e-01, -7.63715565e-01,  2.26604319e+00,
        -3.84449422e-01, -5.50937235e-01,  1.90617055e-01, -1.98027387e-01,
        -2.09054708e+00,  3.21815163e-03,  5.15977919e-01,  4.85105366e-02,
         8.05631801e-02,  1.14420748e+00, -6.27675414e-01,  7.57883117e-02,
        -8.16832483e-01, -6.10565662e-01, -5.64227402e-01,  1.38319775e-01,
        -9.28505063e-01, -1.16803132e-01,  3.85171682e-01, -3.80953759e-01,
        -5.83851710e-02,  6.17815793e-01, -2.17353124e-02,  2.03772098e-01,
         1.11592785e-01,  1.96990716e+00, -9.58429396e-01,  1.61189228e-01,
        -2.14153457e+00, -4.25747395e-01, -8.50736380e-01,  3.82705837e-01,
        -1.37006652e+00, -6.59074262e-02,  8.66046548e-01,  3.75376016e-01,
        -2.16193333e-01,  6.23335958e-01,  5.34012258e-01,  7.89047405e-02,
         3.61278385e-01,  3.03112566e-01, -6.71009719e-01,  8.67069483e-01,
        -1.35923672e+00, -5.94316840e-01, -1.87837946e+00,  2.19266918e-02,
         1.75728011e+00,  2.87652373e-01,  5.20108044e-01, -1.24127507e-01,
         1.92016196e-02, -4.78113502e-01,  6.78314805e-01, -5.49110062e-02,
         5.55088639e-01,  6.21508420e-01,  5.41941106e-01, -1.52575696e+00,
         2.16787243e+00, -1.01279140e+00, -4.14694101e-01, -7.02538788e-01,
        -1.06478870e+00, -4.01664168e-01,  1.13216579e-01,  6.55955136e-01,
        -1.13400944e-01,  2.36688688e-01, -4.16897863e-01,  8.35582912e-01,
        -5.41509748e-01, -9.42327380e-01,  5.98145008e-01, -9.06624138e-01,
         4.08014178e-01,  2.65218592e+00, -3.04896403e-02, -2.57998794e-01,
        -2.83668160e-01, -1.58032596e-01, -2.43843937e+00,  2.91033179e-01,
        -6.62828162e-02,  7.94947505e-01,  6.23370647e-01,  2.58644432e-01,
         1.00323784e+00, -3.19740176e-02, -6.43893421e-01, -9.67779398e-01,
         1.00629997e+00,  8.07835937e-01,  3.03175044e+00, -4.59928662e-01,
        -2.47082806e+00, -2.07793728e-01,  5.83743691e-01,  8.03370625e-02,
        -1.83092415e-01,  4.99773994e-02,  9.88985226e-02, -2.10267901e-01,
        -8.07488203e-01, -1.39929220e-01, -8.18886280e-01, -1.11756183e-01,
        -1.31697428e+00, -6.81489646e-01,  2.83593941e+00, -1.08026125e-01,
         8.22572529e-01,  8.36860418e-01, -3.67861032e-01,  1.40760899e-01,
         6.85579926e-02, -3.37915346e-02, -1.52598202e+00,  1.17405450e+00,
         6.44327840e-03,  5.13491035e-01, -5.04463255e-01, -2.29996419e+00,
         1.53115201e+00, -4.19964284e-01,  7.66361207e-02, -7.12424874e-01,
        -3.46544772e-01, -8.09446126e-02,  8.19393277e-01,  1.19659913e+00,
        -1.19774118e-01,  3.80648285e-01,  3.59554827e-01, -5.00715911e-01,
        -3.43094885e-01, -6.68680251e-01,  1.39926732e-01,  4.56699222e-01,
        -7.49835253e-01,  2.51315737e+00,  1.83941945e-01,  1.14398431e-02,
         5.47137618e-01,  6.99562356e-02, -1.88034248e+00,  4.71065164e-01,
        -1.82728544e-02, -5.20491540e-01, -4.25021559e-01, -3.77878733e-02,
         3.83191288e-01, -6.55066252e-01,  9.46365297e-01,  7.33962119e-01,
        -2.77417541e+00,  4.86274511e-01, -2.87150693e+00,  3.05063009e-01,
         2.57282138e+00,  5.63411891e-01, -4.35844176e-02, -3.17192972e-01,
         3.22698653e-02,  5.52704930e-01, -2.92888805e-02, -6.85142055e-02};
  const vector<float> combined_conv_2_bias = {6.11342907, -5.39038754, -2.30999088,  2.02456760,  0.20255445,
        -4.91605186,  2.03551245, -0.71253127, -0.92475903, -1.30164731,
        -2.61004257,  0.83861542, -3.29337525, -0.50927752, -0.26907164,
         2.01281762};
  const vector<float> combined_conv_3_weight = {-9.74447668e-01,  2.20057711e-01,  2.83167571e-01,  2.61599362e-01,
         4.85969424e-01,  3.54701430e-01, -5.20264745e-01, -2.26678208e-01,
        -3.79126757e-01, -6.31029665e-01,  9.70641151e-02,  5.86733997e-01,
        -2.57146150e-01, -2.79365983e-02, -3.08737308e-01, -1.54575050e-01,
        -1.86778590e-01, -1.43572345e-01, -1.15859257e-02, -8.32844675e-02,
        -1.57301258e-02, -2.12681517e-01, -1.26862481e-01,  2.70894892e-03,
        -2.60781258e-01, -9.66480896e-02, -1.39128849e-01,  4.39873151e-02,
         3.10553052e-02, -2.34508708e-01, -1.83666021e-01, -1.72467381e-01,
         2.45356709e-01, -6.01052821e-01, -2.22466365e-01, -4.31542337e-01,
         6.00216463e-02, -1.25160003e+00, -8.03759694e-01,  7.22394168e-01,
         9.79586765e-02, -8.59360456e-01, -8.04987371e-01, -4.10959050e-02,
        -2.83470929e-01, -2.79448330e-01,  8.74813974e-01, -5.84912673e-03,
         4.85999107e-01,  2.22525567e-01, -1.62440479e-01,  2.29875892e-01,
         3.25196505e-01,  3.90824564e-02, -5.97615898e-01, -6.98724568e-01,
         9.51623023e-01, -1.86301321e-01, -1.60578370e-01,  2.02228174e-01,
         4.07133788e-01,  4.87193018e-01,  3.02111089e-01,  1.56640738e-01,
         6.99577332e-01,  1.45219401e-01,  1.17030668e+00, -3.05013031e-01,
         2.66019166e-01,  1.70260876e-01, -5.29406726e-01, -4.56191540e-01,
        -3.24591011e-01, -8.89236867e-01,  7.72850886e-02, -3.98224384e-01,
         1.42099917e-01,  1.05679929e-01, -2.41677299e-01,  1.50840163e-01,
         2.03998566e+00,  8.88291121e-01, -5.64850390e-01,  8.77956674e-02,
         1.24411201e+00, -3.98585945e-01,  9.55495760e-02, -6.59634113e-01,
        -1.72336504e-01,  1.81928217e+00,  1.52176476e+00,  6.29707217e-01,
        -5.61922133e-01,  5.77049077e-01,  6.01544082e-01, -3.91049832e-01,
        -1.23824143e+00,  3.85317445e-01,  2.12756768e-01, -2.03903899e-01,
        -4.87321377e-01, -3.18626970e-01, -1.03982024e-01,  2.58369833e-01,
        -3.66104953e-02, -1.77985430e-02, -4.55405176e-01, -2.59956509e-01,
        -5.14823616e-01, -8.95873070e-01,  7.95724869e-01, -5.47016323e-01,
         3.07352972e+00,  1.71522871e-01,  5.12809634e-01, -2.91853279e-01,
         1.52609944e-01, -2.45820060e-01,  2.68639892e-01, -5.27580738e-01,
         1.55987632e+00,  6.79236829e-01, -2.24442214e-01,  2.54002064e-01,
        -1.93356290e-01,  1.40254426e+00, -2.33932242e-01,  8.49386632e-01,
        -1.18528958e-02,  4.00229305e-01, -3.33093524e-01,  6.13132305e-03,
         3.11727494e-01, -3.48520614e-02,  2.65405178e-01,  3.38042259e-01,
         5.70438683e-01, -9.15606439e-01, -9.28561985e-01,  5.59336126e-01,
        -7.55285844e-02, -1.31314978e-01,  7.33380914e-01, -5.11452556e-01,
         1.82452214e+00,  3.16916794e-01,  4.43177670e-01,  1.14170623e+00,
        -7.82480121e-01,  3.58512253e-01,  6.22398734e-01,  1.19109297e+00,
         5.48851788e-01, -7.73045301e-01,  5.18816337e-02, -3.04756850e-01,
        -1.73587456e-01,  1.69156164e-01,  1.37144491e-01,  7.25877583e-01,
        -1.09411351e-01,  3.83180112e-01,  1.06113815e+00,  8.84296075e-02,
        -1.09231211e-01,  3.44875157e-01,  5.22659242e-01, -5.06400228e-01,
        -5.39325476e-01, -6.25870004e-03, -5.42294160e-02, -1.10060237e-01,
         2.72753686e-01, -6.08180165e-01, -5.11655390e-01,  3.09187099e-02,
         1.81478846e+00, -3.47051710e-01, -3.65452230e-01,  1.47826165e-01,
         4.58611637e-01,  5.83332658e-01, -2.22239777e-01,  1.74866593e+00,
        -4.29845154e-01, -5.87234534e-02,  1.64078608e-01, -2.56165147e-01,
         6.78182364e-01,  1.49624670e+00,  1.74241856e-01,  5.95162034e-01,
         9.92442966e-01, -5.39313674e-01,  2.80808419e-01,  4.89233047e-01,
         4.83074129e-01,  1.18117273e-01, -5.22628665e-01, -3.82398576e-01,
         2.85965800e-01, -5.66529989e-01,  6.12452105e-02,  2.38560781e-01,
        -9.28068385e-02, -3.57449800e-01,  2.79445171e-01, -8.74210801e-03,
        -4.69140577e+00,  6.27961278e-01,  8.62550497e-01, -2.71632046e-01,
        -7.59497210e-02,  2.51184553e-01,  1.02832472e+00, -3.82637203e-01,
         5.39280400e-02,  5.22419453e-01,  1.25975621e+00, -1.24710643e+00,
        -1.74684548e+00,  2.63144702e-01, -1.03101051e+00, -9.84400511e-01,
        -3.73025894e+00,  1.89660862e-01,  6.03703141e-01, -2.12101102e-01,
        -1.29109752e+00,  1.06802893e+00, -5.72394729e-01, -5.26743114e-01,
        -2.85944402e-01, -1.89586237e-01, -1.76410060e-02,  9.53579605e-01,
        -2.95196444e-01, -7.81087697e-01,  1.20133209e+00, -2.15769506e+00,
        -2.14351267e-01, -1.82165772e-01, -9.80141759e-02,  5.40955774e-02,
        -2.74507642e-01,  3.23228121e-01, -9.21283841e-01, -1.03137529e+00,
        -4.48028892e-01,  2.78927863e-01,  3.18396598e-01,  2.42532164e-01,
        -2.74067521e-01,  1.04684043e+00,  3.93352538e-01, -5.84380388e-01};
  const vector<float> combined_conv_3_bias = {6.20002699, -0.42482451,  0.16643116, -2.02611184,  5.62906122,
         7.99757957,  0.86516970,  1.10006893,  6.85921907,  5.93191147,
         6.96220589,  1.53250098,  1.70456004, -2.14940023,  0.32597804,
         0.12151895};
  const vector<float> combined_conv_4_weight = {1.54919767e+00, -8.80570531e-01,  3.06944519e-01, -9.08157349e+00,
         8.06221604e-01,  1.33562708e+00, -3.28628808e-01,  6.58419132e-02,
        -5.35079777e-01,  5.59188247e-01, -8.14534485e-01, -5.28330088e-01,
        -1.32754490e-01,  4.82079595e-01, -5.05115926e-01, -3.53721172e-01,
        -7.06384039e+00, -8.87627184e-01,  1.70272923e+00, -1.43543458e+00,
         1.66676390e+00,  2.63172364e+00,  6.76891878e-02, -6.56958759e-01,
        -5.64980507e-01,  5.40761709e-01, -1.07847798e+00, -1.36270368e+00,
         8.29553366e-01,  1.17381406e+00, -7.85380363e-01, -9.38183069e-01,
         6.34323835e-01, -3.86464238e-01, -8.53485346e-01,  9.41420972e-01,
        -1.62772223e-01, -1.67413235e+00,  1.07513976e+00, -1.66262284e-01,
         5.68826437e-01, -6.49530530e-01, -5.24614871e-01,  4.29379135e-01,
        -5.64440489e-01,  4.07531768e-01,  8.94145548e-01,  6.29018843e-01,
         2.31979117e-01,  8.10828686e-01, -1.13609719e+00, -3.76489282e+00,
        -7.40931213e-01, -1.49441838e+00,  6.66959703e-01,  3.43385965e-01,
         3.60839546e-01, -6.30538106e-01,  6.69264197e-01,  3.11449349e-01,
        -8.43709826e-01, -6.29809022e-01,  5.55612504e-01,  4.82549071e-01,
        -4.87714911e+00, -6.02096498e-01, -1.11375320e+00,  1.40845168e+00,
        -2.57172084e+00, -2.11401629e+00, -3.67098618e+00, -5.22124863e+00,
         1.38258791e+00, -1.97137821e+00, -7.37471044e-01,  1.38983500e+00,
         2.78015405e-01, -5.97898245e+00, -1.99394703e+00,  7.62558556e+00,
         2.24464247e-03,  9.38632488e-01, -1.08121979e+00,  8.65126491e-01,
        -7.36599624e-01, -1.71423006e+00,  5.07827044e-01,  3.08838457e-01,
         2.59322733e-01, -6.34649694e-01,  7.86757767e-01,  5.44129908e-01,
        -4.90479857e-01, -8.04648638e-01,  4.54021603e-01,  5.63731372e-01,
        -4.31699306e-01, -9.65369880e-01,  1.21141613e+00,  1.47322118e-01,
         1.08244109e+00,  1.66446972e+00, -5.87256193e-01, -3.08416724e-01,
        -5.12849510e-01,  7.80855536e-01, -7.56065965e-01, -4.51954156e-01,
         7.02111363e-01,  6.89919889e-01, -7.11791098e-01, -6.04650974e-01,
        -3.58963513e+00,  6.13596976e-01, -5.61840236e-01, -1.73875594e+00,
        -5.80293298e-01,  2.32237744e+00, -2.22267890e+00,  2.15638787e-01,
        -7.76987731e-01, -1.65146303e+00,  3.83469492e-01,  5.88146806e-01,
         7.60783076e-01, -5.37075758e-01, -1.10473597e+00, -2.66095400e+00,
         6.92994654e-01, -6.52644038e-01,  2.51308411e-01,  6.81792498e-02,
         8.85898590e-01,  4.43840995e-02,  2.04911362e-02,  2.43829980e-01,
        -5.76440454e-01,  3.15929532e-01,  7.30818063e-02,  3.86876434e-01,
         4.45246190e-01,  2.78747757e-03, -5.55878937e-01, -3.08879130e-02,
        -6.04599667e+00,  1.22958279e+00, -8.98070157e-01, -1.05139434e+00,
        -7.17560872e-02, -3.27455068e+00,  1.04195929e+00,  3.51443321e-01,
         1.02609444e+00, -4.06324178e-01, -3.25011373e-01, -1.02801964e-01,
         1.36844468e+00, -1.41138598e-01,  6.11227393e-01, -1.57834336e-01,
         2.44928956e-01, -1.65319499e-02, -2.47217655e+00, -3.06588411e+00,
         3.00236285e-01, -1.84030747e+00,  4.07243252e-01,  1.29298532e+00,
        -3.48906964e-01, -2.28516817e-01, -2.71492992e-02, -3.13179135e-01,
        -2.17958021e+00, -1.92188025e-01,  2.30849907e-01,  8.28304887e-01,
         1.33951330e+00, -9.62436914e-01,  1.01813543e+00, -2.61930680e+00,
         6.66934013e-01,  3.99069786e-01,  1.69702441e-01, -7.56678104e-01,
        -1.45269454e-01,  5.52548468e-01, -7.58486748e-01, -7.24743754e-02,
         5.36467791e-01,  4.41602498e-01, -4.04773831e-01, -1.00456439e-01,
        -2.86027551e-01,  1.86560667e+00, -6.14773393e-01,  1.35941017e+00,
        -9.26805079e-01,  4.68168259e-01,  8.75545442e-01,  6.76296055e-02,
         2.87370354e-01, -1.09171963e+00,  1.26296985e+00,  6.51977479e-01,
        -4.34217662e-01, -9.59175885e-01,  2.84161568e-01, -1.24433286e-01,
         8.68541896e-01, -8.28209519e-01,  7.68427014e-01, -2.35256642e-01,
         1.08118796e+00,  8.62993479e-01, -5.43016255e-01,  8.46228227e-02,
        -4.44212437e-01,  4.81231958e-01, -6.58661485e-01, -4.55552816e-01,
         3.14212978e-01,  5.14569879e-01, -6.29194856e-01, -7.06764400e-01,
        -1.01532574e+01, -6.57325923e-01,  1.83060646e+00, -1.49251878e+00,
         5.10502398e-01,  1.81811893e+00, -2.60058451e+00,  8.50615621e-01,
        -4.78943408e-01,  1.46189260e+00, -9.61822152e-01, -5.06290615e-01,
         1.13526392e+00,  7.77900636e-01, -1.84588894e-01, -7.80748844e-01,
         1.85992742e+00,  5.36910951e-01, -1.56649590e-01,  2.04731897e-01,
         1.30485564e-01,  1.19510520e+00,  6.72815859e-01, -2.15006635e-01,
         1.64256673e-02, -4.46245849e-01, -4.81229544e-01,  4.76782545e-02,
        -4.02884841e-01,  1.40379980e-01, -5.16901575e-02, -9.54654217e-01};
  const vector<float> combined_conv_4_bias = {-6.67553902, -4.44465780,  0.36563769, -0.71549964,  0.39668179,
         1.31855237, -1.70721030,  5.78706312,  0.60136384,  0.21311936,
        -4.82837963,  3.09913087, -6.74126291,  0.51895195, -0.37033933,
         0.53897923};
  const vector<float> combined_conv_5_weight = {0.39312911,  0.41479024, -0.15141176,  0.05444891, -0.19550063,
        -0.22838263, -0.13497184,  0.13642092,  0.12690698,  0.28863284,
        -0.31490925, -0.16603415,  0.05818162,  0.13098100, -0.06250539,
         0.04081396};
  const vector<float> combined_conv_5_bias = {4.56770229};
  const vector<float> dense_weight = {0.75491345, -0.50611895, -0.10014740, -2.09038973,  0.49654096,
        -0.38442111,  0.47626203, -0.94809312,  0.03830245,  0.44187406,
        -0.09191249,  0.19179116, -0.04629906,  0.37095866, -0.52734542,
        -0.09499323, -1.46676207, -0.52827072, -0.20669046,  2.03386521,
         0.10252255, -0.17453596,  0.13883112, -0.10145181, -0.14673688,
         0.08610714,  0.13872261,  0.10540278, -0.25135991,  0.18568563,
        -0.31330889,  0.12620778,  0.26234260,  0.44903442, -0.02629553,
         0.02328466, -1.50544822, -0.66097152, -2.18779325,  1.16967845,
         0.17206933, -0.05475712, -0.00784741, -0.09526956,  0.08766762,
        -0.37911332,  0.93057543,  0.02968620,  0.64146549, -0.97302943,
         0.37678975, -0.42676073,  0.14498398,  1.00472224, -0.16303711,
        -0.45413598, -0.44689196,  0.49381575, -0.28686038, -0.34049711,
         0.38352245,  0.23517075, -0.70686740, -0.39547235, -0.59020758,
         1.05296552,  0.19680373, -1.68084550, -0.19525781,  0.41573673,
         0.98226631, -0.46871617,  0.03334028, -1.03646946,  0.40860766,
        -0.41696927,  0.09385660, -0.19836368,  0.10368052, -0.02679183};
  const vector<float> dense_bias = {2.09609604, -1.12334347,  0.33549047,  0.58848423, -2.26074862};

  const float money_mean = 346.74870064633706;
  const float money_stdev = 431.0048363794357;
  const float turn_mean = 499.4560070279324;
  const float turn_stdev = 288.86107030148634;
  const float hand_card_work_power_mean = 4.528233355930362;
  const float hand_card_work_power_stdev = 10.609973104069168;
  const float project_work_remaining_mean = 62.74699725356077;
  const float project_work_remaining_stdev = 64.58732208404447;
  const float project_reward_mean = 88.46427135316625;
  const float project_reward_stdev = 88.72259046000099;
  const float deck_card_work_power_mean = 11.187048003909721;
  const float deck_card_work_power_stdev = 15.560878129062734;
  const float deck_card_price_mean = 61.91726797219263;
  const float deck_card_price_stdev = 156.60624113045614;

  shared_ptr<Lib::NeuralNet::Input> input_money, input_turn, input_hand_card, input_project, input_deck_card;
  shared_ptr<Lib::NeuralNet::Layer> output_best_card_id;

  GetCardOutputNN() {
    input_money = make_shared<Lib::NeuralNet::Input>(vector<float>(16));
    input_turn = make_shared<Lib::NeuralNet::Input>(vector<float>(16));
    input_hand_card = make_shared<Lib::NeuralNet::Input>(vector<float>(7 * Consts::max_hand_card_num));
    input_project = make_shared<Lib::NeuralNet::Input>(vector<float>(3 * Consts::max_project_num));
    input_deck_card = make_shared<Lib::NeuralNet::Input>(vector<float>(9 * Consts::max_deck_card_num));

    auto hand_card_conv_1 = make_shared<Lib::NeuralNet::Conv1d>(7, 16, hand_card_conv_1_weight, hand_card_conv_1_bias);
    auto hand_card_conv_1_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto hand_card_conv_2 = make_shared<Lib::NeuralNet::Conv1d>(16, 16, hand_card_conv_2_weight, hand_card_conv_2_bias);
    auto hand_card_conv_2_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto project_conv_1 = make_shared<Lib::NeuralNet::Conv1d>(3, 16, project_conv_1_weight, project_conv_1_bias);
    auto project_conv_1_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto project_conv_2 = make_shared<Lib::NeuralNet::Conv1d>(16, 16, project_conv_2_weight, project_conv_2_bias);
    auto project_conv_2_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto deck_card_conv_1 = make_shared<Lib::NeuralNet::Conv1d>(9, 16, deck_card_conv_1_weight, deck_card_conv_1_bias);
    auto deck_card_conv_1_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto deck_card_conv_2 = make_shared<Lib::NeuralNet::Conv1d>(16, 16, deck_card_conv_2_weight, deck_card_conv_2_bias);
    auto deck_card_conv_2_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto concat = make_shared<Lib::NeuralNet::Concat>(16);
    auto transpose_1 = make_shared<Lib::NeuralNet::Transpose>(16);
    auto combined_conv_1 = make_shared<Lib::NeuralNet::Conv1d>(22, 16, combined_conv_1_weight, combined_conv_1_bias);
    auto combined_conv_1_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto transpose_2 = make_shared<Lib::NeuralNet::Transpose>(16);
    auto combined_conv_2 = make_shared<Lib::NeuralNet::Conv1d>(16, 16, combined_conv_2_weight, combined_conv_2_bias);
    auto combined_conv_2_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto transpose_3 = make_shared<Lib::NeuralNet::Transpose>(16);
    auto combined_conv_3 = make_shared<Lib::NeuralNet::Conv1d>(16, 16, combined_conv_3_weight, combined_conv_3_bias);
    auto combined_conv_3_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto transpose_4 = make_shared<Lib::NeuralNet::Transpose>(16);
    auto combined_conv_4 = make_shared<Lib::NeuralNet::Conv1d>(16, 16, combined_conv_4_weight, combined_conv_4_bias);
    auto combined_conv_4_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto add = make_shared<Lib::NeuralNet::Add>();
    auto transpose_5 = make_shared<Lib::NeuralNet::Transpose>(16);
    auto combined_conv_5 = make_shared<Lib::NeuralNet::Conv1d>(16, 1, combined_conv_5_weight, combined_conv_5_bias);
    auto combined_conv_5_relu = make_shared<Lib::NeuralNet::ReLU>();
    auto dense = make_shared<Lib::NeuralNet::Linear>(16, Consts::max_deck_card_num, dense_weight, dense_bias);

    output_best_card_id = make_shared<Lib::NeuralNet::Softmax>();

    hand_card_conv_1->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({input_hand_card});
    hand_card_conv_1_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({hand_card_conv_1});
    hand_card_conv_2->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({hand_card_conv_1_relu});
    hand_card_conv_2_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({hand_card_conv_2});
    project_conv_1->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({input_project});
    project_conv_1_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({project_conv_1});
    project_conv_2->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({project_conv_1_relu});
    project_conv_2_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({project_conv_2});
    deck_card_conv_1->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({input_deck_card});
    deck_card_conv_1_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({deck_card_conv_1});
    deck_card_conv_2->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({deck_card_conv_1_relu});
    deck_card_conv_2_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({deck_card_conv_2});
    concat->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({input_money, input_turn, hand_card_conv_2_relu, project_conv_2_relu, deck_card_conv_2_relu});
    transpose_1->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({concat});
    combined_conv_1->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({transpose_1});
    combined_conv_1_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_1});
    transpose_2->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_1_relu});
    combined_conv_2->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({transpose_2});
    combined_conv_2_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_2});
    transpose_3->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_2_relu});
    combined_conv_3->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({transpose_3});
    combined_conv_3_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_3});
    transpose_4->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_3_relu});
    combined_conv_4->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({transpose_4});
    combined_conv_4_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_4});
    add->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_2_relu, combined_conv_4_relu});
    transpose_5->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({add});
    combined_conv_5->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({transpose_5});
    combined_conv_5_relu->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_5});
    dense->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({combined_conv_5_relu});
    output_best_card_id->input_layers = vector<shared_ptr<Lib::NeuralNet::Layer>>({dense});
  }

  float z_score(const float x, const float mean, const float stdev) const {
    return (x - mean) / stdev;
  }

  vector<int> get_best_card_id(const GameEnvironment& environment, const GameState& state, const UseCardOutput& use_card_output, const TurnInput& turn_input) {
    input_money->input.resize(16);
    for (int i = 0; i < 16; i++) {
      input_money->input[i] = z_score((float) turn_input.current_money / (1LL << state.invest_count), money_mean, money_stdev);
    }

    input_turn->input.resize(16);
    for (int i = 0; i < 16; i++) {
      input_turn->input[i] = z_score((float) state.current_turn, turn_mean, turn_stdev);
    }

    input_hand_card->input.resize(7 * Consts::max_hand_card_num);
    for (int j = 0; j < Consts::max_hand_card_num; j++) {
      if (j < environment.hand_card_num) {
        for (int i = 0; i < 5; i++) {
          input_hand_card->input[Lib::NeuralNet::idx_2d(i, j, Consts::max_hand_card_num)] = (int) state.hand_cards[j].type == i ? 1.0 : 0.0;
        }
        input_hand_card->input[Lib::NeuralNet::idx_2d(5, j, Consts::max_hand_card_num)] = z_score((float) state.hand_cards[j].work_power, hand_card_work_power_mean, hand_card_work_power_stdev);
        input_hand_card->input[Lib::NeuralNet::idx_2d(6, j, Consts::max_hand_card_num)] = 1.0;
      }
      else {
        for (int i = 0; i < 7; i++) {
          input_hand_card->input[Lib::NeuralNet::idx_2d(i, j, Consts::max_hand_card_num)] = 0.0;
        }
      }
    }

    input_project->input.resize(3 * Consts::max_project_num);
    for (int j = 0; j < Consts::max_project_num; j++) {
      if (j < environment.project_num) {
        input_project->input[Lib::NeuralNet::idx_2d(0, j, Consts::max_project_num)] = z_score((float) turn_input.project_status[j].work_remaining, project_work_remaining_mean, project_work_remaining_stdev);
        input_project->input[Lib::NeuralNet::idx_2d(1, j, Consts::max_project_num)] = z_score((float) turn_input.project_status[j].reward, project_reward_mean, project_reward_stdev);
        input_project->input[Lib::NeuralNet::idx_2d(2, j, Consts::max_project_num)] = 1.0;
      }
      else {
        for (int i = 0; i < 3; i++) {
          input_project->input[Lib::NeuralNet::idx_2d(i, j, Consts::max_project_num)] = 0.0;
        }
      }
    }

    input_deck_card->input.resize(9 * Consts::max_deck_card_num);
    for (int j = 0; j < Consts::max_deck_card_num; j++) {
      if (j < environment.deck_card_num) {
        for (int i = 0; i < 5; i++) {
          input_deck_card->input[Lib::NeuralNet::idx_2d(i, j, Consts::max_deck_card_num)] = (int) turn_input.deck_cards[j].type == i ? 1.0 : 0.0;
        }
        input_deck_card->input[Lib::NeuralNet::idx_2d(5, j, Consts::max_deck_card_num)] = z_score((float) turn_input.deck_cards[j].work_power, deck_card_work_power_mean, deck_card_work_power_stdev);
        input_deck_card->input[Lib::NeuralNet::idx_2d(6, j, Consts::max_deck_card_num)] = z_score((float) turn_input.deck_cards[j].price, deck_card_price_mean, deck_card_price_stdev);
        input_deck_card->input[Lib::NeuralNet::idx_2d(7, j, Consts::max_deck_card_num)] = turn_input.current_money >= turn_input.deck_cards[j].price ? 1.0 : 0.0;
        input_deck_card->input[Lib::NeuralNet::idx_2d(8, j, Consts::max_deck_card_num)] = 1.0;
      }
      else {
        for (int i = 0; i < 9; i++) {
          input_deck_card->input[Lib::NeuralNet::idx_2d(i, j, Consts::max_deck_card_num)] = 0.0;
        }
      }
    }

    output_best_card_id->reset_output();
    output_best_card_id->update_output();
    const auto &output = output_best_card_id->output;
    vector<int> best_id_sorted(environment.deck_card_num);
    for (int i = 0; i < environment.deck_card_num; i++) best_id_sorted[i] = i;
    sort(best_id_sorted.rbegin(), best_id_sorted.rend(), [&](auto &a, auto &b){return output[a] < output[b];});
    #ifdef DEBUG
    cerr << state.current_turn << " ";
    for (int i = 0; i < environment.deck_card_num; i++) {
      cerr << output[i] << " ";
    }
    cerr << endl;
    #endif
    return best_id_sorted;
  }
};

InitialInput get_initial_input() {
  InitialInput input;
  cin >> input.hand_card_num >> input.project_num >> input.deck_card_num >> input.turn_max;
  for (int i = 0; i < input.hand_card_num; i++) {
    CardType type = WorkSingle;
    ll power;
    cin >> type >> power;
    input.initial_hand_cards.push_back(Card(type, power, 0));
  }
  for (int i = 0; i < input.project_num; i++) {
    ll work_remaining, reward;
    cin >> work_remaining >> reward;
    input.initial_projects.push_back(Project(work_remaining, reward));
  }
  return input;
}

GameEnvironment create_environment(const InitialInput& initial_input) {
  return GameEnvironment(initial_input.hand_card_num, initial_input.project_num, initial_input.deck_card_num, initial_input.turn_max);
}

GameState create_state(const InitialInput& initial_input) {
  return GameState(0, initial_input.initial_hand_cards, initial_input.initial_projects);
}

void output_comment(const string& s) {
  // cout << "#" << s << endl;
}

void output_use_card(const GameEnvironment& environment, const GameState& state, const UseCardOutput& output) {
  assert(0 <= output.card_id && output.card_id <= environment.hand_card_num);
  assert(0 <= output.target_project_id && output.target_project_id <= environment.project_num);

  const auto card_type = state.hand_cards[output.card_id].type;
  assert(card_type == WorkSingle || card_type == CancelSingle || output.target_project_id == 0);
  
  cout << output.card_id << " " << output.target_project_id << endl;
}

void update_use_card(const GameEnvironment& environment, GameState& state, const UseCardOutput& output, const bool no_output = false) {
  if (!no_output) output_use_card(environment, state, output);
  
  auto card_used = state.hand_cards[output.card_id];
  if (card_used.type == Invest) state.invest_count++;
}

TurnInput get_turn_input(const GameEnvironment& environment) {
  TurnInput input;
  for (int i = 0; i < environment.project_num; i++) {
    ll work_remaining, reward;
    cin >> work_remaining >> reward;
    input.project_status.push_back(Project(work_remaining, reward));
  }
  cin >> input.current_money;
  for (int i = 0; i < environment.deck_card_num; i++) {
    CardType type = WorkSingle;
    ll work_power, price;
    cin >> type >> work_power >> price;
    input.deck_cards.push_back(Card(type, work_power, price));
  }
  return input;
}

void output_get_card(const GameEnvironment& environment, const TurnInput& turn_input, const GetCardOutput& output) {
  assert(0 <= output.card_id && output.card_id < environment.deck_card_num);
  
  const auto& card = turn_input.deck_cards[output.card_id];
  assert(card.price <= turn_input.current_money);

  cout << output.card_id << endl;
}

void update_get_card(const GameEnvironment& environment, GameState& state, const UseCardOutput& use_card_output, const TurnInput& turn_input, const GetCardOutput& get_card_output, const bool no_output = false) {
  if (!no_output) output_get_card(environment, turn_input, get_card_output);

  const auto& card_to_get = turn_input.deck_cards[get_card_output.card_id];
  state.hand_cards[use_card_output.card_id] = card_to_get;
  state.money = turn_input.current_money - card_to_get.price;
  state.projects = turn_input.project_status;
}

UseCardOutput decide_use_card_output(const GameEnvironment& environment, const GameState& state) {
  const double money_efficiency_factor = HyperParameter::money_efficiency_factor;
  const double worth_finish_threshold_factor = HyperParameter::worth_finish_threshold_factor;
  const double wasted_power_penalty_factor = HyperParameter::wasted_power_penalty_factor;
  const double insufficient_power_penalty_factor = HyperParameter::insufficient_power_penalty_factor;
  const double work_all_utilization_rate_threshold = HyperParameter::work_all_utilization_rate_threshold;

  // 増資カードを使う場合
  // 持っている場合は即時使用する
  {
    bool has_invest_card = false;
    int card_id = 0;
    for (int c = 0; c < environment.hand_card_num; c++) {
      const auto& card = state.hand_cards[c];
      if (card.type == Invest) {
        has_invest_card = true;
        card_id = c;
      }
    }
    if (has_invest_card && state.invest_count < Consts::max_invest_num) {
      return UseCardOutput(card_id, 0);
    }
  }

  bool has_work_card = false;
  for (int c = 0; c < environment.hand_card_num; c++) {
    const auto& card = state.hand_cards[c];
    if (card.type == WorkSingle || card.type == WorkAll) has_work_card = true;
  }

  if (!has_work_card) {
    // 労働カードがなければ手持ちのカードを適当に使う
    // TODO: 労働カードを持っていない場合の処理
    return UseCardOutput(0, 0);
  }

  ll minimum_work_power = (ll) 9e18;
  for (int c = 0; c < environment.hand_card_num; c++) {
    const auto& card = state.hand_cards[c];
    if (card.type == WorkSingle || card.type == WorkAll) {
      minimum_work_power = min(minimum_work_power, card.work_power);
    }
  }

  double available_single_work_power = (double) state.money * money_efficiency_factor;
  for (int c = 0; c < environment.hand_card_num; c++) {
    const auto& card = state.hand_cards[c];
    if (card.type == WorkSingle || card.type == WorkAll) available_single_work_power += card.work_power;
  }

  vector<double> work_efficiency(environment.project_num);
  vector<bool> is_worth_finishing(environment.project_num);
  for (int p = 0; p < environment.project_num; p++) {
    const auto& project = state.projects[p];
    work_efficiency[p] = (double) project.reward / (double) project.work_remaining;
    is_worth_finishing[p] = work_efficiency[p] > worth_finish_threshold_factor;
  }

  // プロジェクト全てをキャンセルする場合
  {
    bool use_cancel_all_card = false;
    int card_id = 0;
    for (int c = 0; c < environment.hand_card_num; c++) {
      const auto& card = state.hand_cards[c];
      if (card.type != CancelAll) continue;
      bool good = true;
      for (int p = 0; p < environment.project_num; p++) {
        if (is_worth_finishing[p]) {
          good = false;
          break;
        }
      }
      if (good) {
        use_cancel_all_card = true;
        card_id = c;
      }
    }
    if (use_cancel_all_card) {
      return UseCardOutput(card_id, 0);
    }
  }

  // プロジェクト1つをキャンセルする場合
  {
    bool use_cancel_single_card = false;
    int card_id = 0;
    int target_id = 0;
    ll smallest_work_remaining = (ll) 9e18;
    for (int c = 0; c < environment.hand_card_num; c++) {
      const auto& card = state.hand_cards[c];
      if (card.type != CancelSingle) continue;
      for (int p = 0; p < environment.project_num; p++) {
        if (is_worth_finishing[p]) continue;
        const auto& project = state.projects[p];
        if (project.work_remaining < smallest_work_remaining) {
          use_cancel_single_card = true;
          card_id = c;
          target_id = p;
          smallest_work_remaining = project.work_remaining;
        }
      }
    }
    if (use_cancel_single_card) {
      return UseCardOutput(card_id, target_id);
    }
  }

  // 全力労働カードを使う場合
  {
    bool use_work_card = false;
    int card_id = 0;
    double best_utilization_rate = 0.0;
    for (int c = 0; c < environment.hand_card_num; c++) {
      const auto& card = state.hand_cards[c];
      if (card.type != WorkAll) continue;
      ll work_utilization = 0;
      for (int p = 0; p < environment.project_num; p++) {
        const auto& project = state.projects[p];
        work_utilization += min(card.work_power, project.work_remaining);
      }
      double utilization_rate = (double) work_utilization / (double) (card.work_power * environment.project_num);
      if (utilization_rate > work_all_utilization_rate_threshold && utilization_rate > best_utilization_rate) {
        use_work_card = true;
        card_id = c;
        best_utilization_rate = utilization_rate;
      }
    }
    if (use_work_card) {
      return UseCardOutput(card_id, 0);
    }
  }

  // 完了する価値のあるプロジェクトに労働カードを使う場合
  {
    bool use_work_card = false;
    int card_id = 0;
    int target_id = 0;
    double best_efficiency = 0.0;
    for (int c = 0; c < environment.hand_card_num; c++) {
      const auto& card = state.hand_cards[c];
      if (card.type != WorkSingle) continue;
      for (int p = 0; p < environment.project_num; p++) {
        if (!is_worth_finishing[p]) continue;
        const auto& project = state.projects[p];
        auto efficiency = (double) project.reward * min((double) card.work_power / (double) project.work_remaining, (double) project.work_remaining / (double) card.work_power);
        if (card.work_power > project.work_remaining) efficiency -= (double) (card.work_power - project.work_remaining) * wasted_power_penalty_factor;
        if (available_single_work_power < (double) project.work_remaining) efficiency -= ((double) project.work_remaining - available_single_work_power) * insufficient_power_penalty_factor;
        if (efficiency > best_efficiency) {
          use_work_card = true;
          card_id = c;
          target_id = p;
          best_efficiency = efficiency;
        }
      }
    }
    if (use_work_card) {
      output_comment("Efficiency = " + to_string(best_efficiency));
      return UseCardOutput(card_id, target_id);
    }
  }

  // 完了する価値のないプロジェクトに労働カードを使う場合
  {
    bool use_work_card = false;
    int card_id = 0;
    int target_id = 0;
    double best_efficiency = -1e300;
    for (int c = 0; c < environment.hand_card_num; c++) {
      const auto& card = state.hand_cards[c];
      if (!(card.type == WorkSingle || card.type == WorkAll)) continue;
      for (int p = 0; p < environment.project_num; p++) {
        // TODO: 全力労働カードの使い方を考える
        if (card.type == WorkAll && p > 0) continue;
        const auto& project = state.projects[p];
        auto efficiency = (double) project.reward * min((double) card.work_power / (double) project.work_remaining, (double) project.work_remaining / (double) card.work_power);
        if (card.work_power > project.work_remaining) efficiency -= (double) (card.work_power - project.work_remaining) * wasted_power_penalty_factor;
        if (available_single_work_power < (double) project.work_remaining) efficiency -= ((double) project.work_remaining - available_single_work_power) * insufficient_power_penalty_factor;
        if (efficiency > best_efficiency) {
          use_work_card = true;
          card_id = c;
          target_id = p;
          best_efficiency = efficiency;
        }
      }
    }
    if (use_work_card) {
      output_comment("Efficiency = " + to_string(best_efficiency));
      return UseCardOutput(card_id, target_id);
    }
  }

  // 決まらない場合は手持ちのカードを適当に使う
  return UseCardOutput(0, 0);
}

GetCardOutput decide_get_card_output(const GameEnvironment& environment, const GameState& state, const UseCardOutput& use_card_output, const TurnInput& turn_input) {
  const double worth_buy_work_single_efficiency_threshold_factor = HyperParameter::worth_buy_work_single_efficiency_threshold_factor;
  const double worth_buy_work_all_efficiency_threshold_factor = HyperParameter::worth_buy_work_all_efficiency_threshold_factor;
  const double worth_buy_cancel_hand_num_threshold_factor = HyperParameter::worth_buy_cancel_hand_num_threshold_factor;
  const ll worth_buy_cancel_price_threshold_factor = HyperParameter::worth_buy_cancel_price_threshold_factor;
  const ll worth_buy_invest_extra_money_threshold_factor = HyperParameter::worth_buy_invest_extra_money_threshold_factor;
  const ll worth_buy_invest_price_threshold_factor = HyperParameter::worth_buy_invest_price_threshold_factor;

  const int invest_stop_turn = HyperParameter::invest_stop_turn;
  const int buy_stop_turn = HyperParameter::buy_stop_turn;

  if (state.current_turn >= buy_stop_turn) {
    // 終了間際になったら無料労働カードを選ぶ
    return GetCardOutput(0);
  }

  int cancel_in_hand = 0;
  for (int c = 0; c < environment.hand_card_num; c++) {
    if (use_card_output.card_id == c) continue; // 使用済みカードは無視
    const auto& card = state.hand_cards[c];
    if (card.type == CancelSingle || card.type == CancelAll) cancel_in_hand++;
  }

  vector<bool> can_afford(environment.deck_card_num);
  vector<bool> worth_buying(environment.deck_card_num);
  for (int c = 0; c < environment.deck_card_num; c++) {
    const auto& card = turn_input.deck_cards[c];

    can_afford[c] = card.price <= turn_input.current_money;
    if (!can_afford[c]) {
      worth_buying[c] = false;
      continue;
    }

    if (card.type == WorkSingle) {
      const auto efficiency = (double) card.work_power / (double) max(1LL, card.price);
      worth_buying[c] = card.price == 0 || efficiency > worth_buy_work_single_efficiency_threshold_factor;
    }
    else if (card.type == WorkAll) {
      const auto efficiency = (double) (card.work_power * environment.project_num) / (double) max(1LL, card.price);
      worth_buying[c] = card.price == 0 || efficiency > worth_buy_work_all_efficiency_threshold_factor;
    }
    else if (card.type == CancelSingle) {
      double hand_cancel_rate = (double) cancel_in_hand / (double) environment.hand_card_num;
      worth_buying[c] = card.price <= (worth_buy_cancel_price_threshold_factor << state.invest_count) && hand_cancel_rate <= worth_buy_cancel_hand_num_threshold_factor;
    }
    else if (card.type == Invest) {
      worth_buying[c] = (turn_input.current_money - card.price) >= (worth_buy_invest_extra_money_threshold_factor << state.invest_count) && card.price <= (worth_buy_invest_price_threshold_factor << state.invest_count) && state.current_turn < invest_stop_turn && state.invest_count < Consts::max_invest_num;
    }
  }

  // 増資カードを買う場合
  {
    // 購入する価値のあるカードの中から最も安いものを選ぶ
    bool buy_invest_card = false;
    int card_id = 0;
    ll best_price = (ll) 9e18;
    for (int c = 1; c < environment.deck_card_num; c++) {
      const auto& card = turn_input.deck_cards[c];
      if (card.type != Invest) continue;
      if (can_afford[c] && worth_buying[c] && card.price < best_price) {
        buy_invest_card = true;
        card_id = c;
        best_price = card.price;
      }
    }
    if (buy_invest_card) {
      return GetCardOutput(card_id);
    }
  }

  // キャンセル系カードを買う場合
  {
    bool buy_cancel_card = false;
    int card_id = 0;
    ll best_price = (ll) 9e18;
    for (int c = 1; c < environment.deck_card_num; c++) {
      const auto& card = turn_input.deck_cards[c];
      if (!(card.type == CancelSingle || card.type == CancelAll)) continue;
      if (can_afford[c] && worth_buying[c] && card.price < best_price) {
        buy_cancel_card = true;
        card_id = c;
        best_price = card.price;
      }
    }
    if (buy_cancel_card) {
      return GetCardOutput(card_id);
    }
  }

  // 0番目以外の労働カードを買う場合
  {
    // 購入する価値のあるカードの中から最も労働力の高いものを選ぶ
    bool buy_work_card = false;
    int card_id = 0;
    ll best_work_power = 0;
    for (int c = 1; c < environment.deck_card_num; c++) {
      const auto& card = turn_input.deck_cards[c];
      if (!(card.type == WorkSingle || card.type == WorkAll)) continue;
      if (can_afford[c] && worth_buying[c] && card.work_power > best_work_power) {
        buy_work_card = true;
        card_id = c;
        best_work_power = card.work_power;
      }
    }
    if (buy_work_card) {
      return GetCardOutput(card_id);
    }
  }

  // 決まらない場合は無料労働カードを選ぶ
  return GetCardOutput(0);
}

TurnInput monte_carlo_create_turn_input(const GameEnvironment& environment, const GameState& state, const UseCardOutput& use_card_output, const MonteCarloEnvironment& monte_carlo_environment, const MonteCarloState& monte_carlo_state) {
  TurnInput turn_input;
  turn_input.project_status = state.projects;
  turn_input.current_money = state.money;
  turn_input.deck_cards.reserve(environment.deck_card_num);

  // カード生成
  for (int c = 0; c < environment.deck_card_num; c++) {
    if (c == 0) {
      // 無料労働カード
      turn_input.deck_cards.emplace_back(WorkSingle, 1LL << state.invest_count, 0LL);
    }
    else {
      turn_input.deck_cards.push_back(monte_carlo_state.create_random_card(state, monte_carlo_environment));
    }
  }

  // プロジェクトの状態の更新
  const auto& card_used = state.hand_cards[use_card_output.card_id];
  switch(card_used.type) {
    case WorkSingle:
      {
        auto& project = turn_input.project_status[use_card_output.target_project_id];
        project.work_remaining -= card_used.work_power;
        if (project.work_remaining <= 0LL) {
          turn_input.current_money += project.reward;
          project = monte_carlo_state.create_random_project(state, monte_carlo_environment);
        }
      }
      break;
    case WorkAll:
      {
        for (int p = 0; p < environment.project_num; p++) {
          auto& project = turn_input.project_status[p];
          project.work_remaining -= card_used.work_power;
          if (project.work_remaining <= 0LL) {
            turn_input.current_money += project.reward;
            project = monte_carlo_state.create_random_project(state, monte_carlo_environment);
          }
        }
      }
      break;
    case CancelSingle:
      {
        auto& project = turn_input.project_status[use_card_output.target_project_id];
        project = monte_carlo_state.create_random_project(state, monte_carlo_environment);
      }
      break;
    case CancelAll:
      {
        for (int p = 0; p < environment.project_num; p++) {
          auto& project = turn_input.project_status[p];
          project = monte_carlo_state.create_random_project(state, monte_carlo_environment);
        }
      }
      break;
    case Invest:
      // 増資回数追加はupdate_use_card()内で行うので、ここでは何もしない
      break;
    default:
      assert(false);
      break;
  }

  return turn_input;
}

GetCardOutput monte_carlo_decide_get_card_output(const GameEnvironment& environment, const GameState& state, const UseCardOutput& use_card_output, const TurnInput& turn_input, const MonteCarloEnvironment& monte_carlo_environment, const MonteCarloState& monte_carlo_state) {
  const int simulation_turn = HyperParameter::monte_carlo_simulation_turn;
  const int score_calculation_turn = HyperParameter::monte_carlo_score_calculation_turn;
  const int money_only_score_turn = HyperParameter::monte_carlo_money_only_score_turn;
  const double work_single_worth = HyperParameter::monte_carlo_work_single_worth;
  const double work_all_worth = HyperParameter::monte_carlo_work_all_worth;
  
  const double start_time = theTimer.time();
  const double time_limit = start_time + (Consts::global_time_limit - start_time) * 1.0 / ((double) (max(1, environment.turn_max - (state.current_turn + 1))) / (double)(min(HyperParameter::monte_carlo_interval_turn, environment.turn_max - (state.current_turn + 1))));

  // ルールベースで決めたときに増資カードを取る場合は必ず増資カードを取る
  const auto rule_base_get_card_output = decide_get_card_output(environment, state, use_card_output, turn_input);
  if (turn_input.deck_cards[rule_base_get_card_output.card_id].type == Invest) {
    return rule_base_get_card_output;
  }
  
  vector<double> score_sum(environment.deck_card_num);
  int iter_count = 0;
  while (theTimer.time() < time_limit) {
    for (int c = 0; c < environment.deck_card_num; c++) {
      if (turn_input.current_money < turn_input.deck_cards[c].price) continue;
      if (turn_input.deck_cards[c].type == CancelAll) continue; // 業務転換カードは無視しておく
      if (turn_input.deck_cards[c].type == Invest && state.invest_count == Consts::max_invest_num) continue; // 増資回数上限に到達している場合は増資カードは無視
      auto sim_state = state;
      const auto first_get_card_output = GetCardOutput(c);
      update_get_card(environment, sim_state, use_card_output, turn_input, first_get_card_output, true);
      const auto sim_turn_max = min(environment.turn_max, state.current_turn + simulation_turn + 1);
      double score = 0.0;
      for (int turn = state.current_turn + 1; turn < sim_turn_max; turn++) {
        // ルールベースで行動を行った場合のシミュレーション
        sim_state.current_turn = turn;
        const auto sim_use_card_output = decide_use_card_output(environment, sim_state);
        update_use_card(environment, sim_state, sim_use_card_output, true);
        const auto sim_turn_input = monte_carlo_create_turn_input(environment, sim_state, sim_use_card_output, monte_carlo_environment, monte_carlo_state);
        const auto sim_get_card_output = decide_get_card_output(environment, sim_state, sim_use_card_output, sim_turn_input);
        update_get_card(environment, sim_state, sim_use_card_output, sim_turn_input, sim_get_card_output, true);
        if (sim_turn_max - turn <= score_calculation_turn) {
          // 残りターン数が一定以下のときの最大スコアを、そのシミュレーションのスコアとする
          double worth = sim_state.money;
          if (state.current_turn < money_only_score_turn)
          {
            // 最終盤以外はカードも点数の考慮に入れる
            for (int hc = 0; hc < environment.hand_card_num; hc++) {
              const auto& card = sim_state.hand_cards[hc];
              switch (card.type) {
                case WorkSingle:
                  worth += (double) card.work_power * work_single_worth;
                  break;
                case WorkAll:
                  worth += (double) (card.work_power * environment.project_num) * work_all_worth;
                  break;
                default:
                  break;
              }
            }
          }
          score = max(score, worth);
        }
      }
      score_sum[c] += score;
    }
    iter_count++;
  }

  int best_card_id = 0;
  double best_score = 0.0;
  for (int c = 0; c < environment.deck_card_num; c++) {
    if (score_sum[c] > best_score) {
      best_card_id = c;
      best_score = score_sum[c];
    }
  }
  #ifdef DEBUG
  cerr << "turn = " << setw(3) << state.current_turn << ", iter_count = " << setw(4) << iter_count << ", score = " << setw(10) << (ll) (best_score / (double) max(1, iter_count)) << endl;
  #endif
  #ifdef NN_CREATE
  // NN用学習データを作成
  static bool is_first = true;
  if (is_first) {
    cerr << "{\"data\":[";
    is_first = false;
  }
  else {
    cerr << ",";
  }
  cerr << "{";
  cerr << "\"money\":" << ((double) turn_input.current_money / (double) (1LL << state.invest_count));
  cerr << ",";
  cerr << "\"turn\":" << state.current_turn;
  cerr << ",";
  cerr << "\"hand_card\":[";
  for (int c = 0; c < environment.hand_card_num; c++) {
    const auto& card = state.hand_cards[c];
    cerr << "{";
    cerr << "\"type\":" << (int) card.type << ",";
    cerr << "\"work_power\":" << ((double) card.work_power / (double) (1LL << state.invest_count)) << ",";
    cerr << "\"used\":" << (c == use_card_output.card_id ? "true" : "false");
    cerr << "}";
    if (c < environment.hand_card_num - 1) cerr << ",";
  }
  cerr << "]";
  cerr << ",";
  cerr << "\"project\":[";
  for (int p = 0; p < environment.project_num; p++) {
    const auto& project = turn_input.project_status[p];
    cerr << "{";
    cerr << "\"work_remaining\":" << ((double) project.work_remaining / (double) (1LL << state.invest_count)) << ",";
    cerr << "\"reward\":" << ((double) project.reward / (double) (1LL << state.invest_count));
    cerr << "}";
    if (p < environment.project_num - 1) cerr << ",";
  }
  cerr << "]";
  cerr << ",";
  cerr << "\"deck_card\":[";
  for (int c = 0; c < environment.deck_card_num; c++) {
    const auto& card = turn_input.deck_cards[c];
    cerr << "{";
    cerr << "\"type\":" << (int) card.type << ",";
    cerr << "\"work_power\":" << ((double) card.work_power / (double) (1LL << state.invest_count)) << ",";
    cerr << "\"price\":" << ((double) card.price / (double) (1LL << state.invest_count)) << ",";
    cerr << "\"can_afford\":" << (turn_input.current_money >= card.price ? "true" : "false");
    cerr << "}";
    if (c < environment.deck_card_num - 1) cerr << ",";
  }
  cerr << "]";
  cerr << ",";
  cerr << "\"best_card_id\":" << best_card_id;
  cerr << "}";
  #endif
  return GetCardOutput(best_card_id);
}

GetCardOutput nn_decide_get_card_output(const GameEnvironment& environment, const GameState& state, const UseCardOutput& use_card_output, const TurnInput& turn_input) {
  static auto nn = GetCardOutputNN();

  const double worth_buy_work_single_efficiency_threshold_factor = HyperParameter::worth_buy_work_single_efficiency_threshold_factor;
  const double worth_buy_work_all_efficiency_threshold_factor = HyperParameter::worth_buy_work_all_efficiency_threshold_factor;
  const double worth_buy_cancel_hand_num_threshold_factor = HyperParameter::worth_buy_cancel_hand_num_threshold_factor;
  const ll worth_buy_cancel_price_threshold_factor = HyperParameter::worth_buy_cancel_price_threshold_factor;
  const ll worth_buy_invest_extra_money_threshold_factor = HyperParameter::worth_buy_invest_extra_money_threshold_factor;
  const ll worth_buy_invest_price_threshold_factor = HyperParameter::worth_buy_invest_price_threshold_factor;

  const int invest_stop_turn = HyperParameter::invest_stop_turn;
  const int buy_stop_turn = HyperParameter::buy_stop_turn;

  // ルールベースで決めたときに増資カードを取る場合は必ず増資カードを取る
  const auto rule_base_get_card_output = decide_get_card_output(environment, state, use_card_output, turn_input);
  if (turn_input.deck_cards[rule_base_get_card_output.card_id].type == Invest) {
    return rule_base_get_card_output;
  }

  int cancel_in_hand = 0;
  for (int c = 0; c < environment.hand_card_num; c++) {
    if (use_card_output.card_id == c) continue; // 使用済みカードは無視
    const auto& card = state.hand_cards[c];
    if (card.type == CancelSingle || card.type == CancelAll) cancel_in_hand++;
  }

  vector<bool> can_afford(environment.deck_card_num);
  vector<bool> worth_buying(environment.deck_card_num);
  for (int c = 0; c < environment.deck_card_num; c++) {
    const auto& card = turn_input.deck_cards[c];

    can_afford[c] = card.price <= turn_input.current_money;
    if (!can_afford[c]) {
      worth_buying[c] = false;
      continue;
    }

    if (card.type == WorkSingle) {
      const auto efficiency = (double) card.work_power / (double) max(1LL, card.price);
      worth_buying[c] = card.price == 0 || efficiency > worth_buy_work_single_efficiency_threshold_factor;
    }
    else if (card.type == WorkAll) {
      const auto efficiency = (double) (card.work_power * environment.project_num) / (double) max(1LL, card.price);
      worth_buying[c] = card.price == 0 || efficiency > worth_buy_work_all_efficiency_threshold_factor;
    }
    else if (card.type == CancelSingle) {
      double hand_cancel_rate = (double) cancel_in_hand / (double) environment.hand_card_num;
      worth_buying[c] = card.price <= (worth_buy_cancel_price_threshold_factor << state.invest_count) && hand_cancel_rate <= worth_buy_cancel_hand_num_threshold_factor;
    }
    else if (card.type == Invest) {
      worth_buying[c] = (turn_input.current_money - card.price) >= (worth_buy_invest_extra_money_threshold_factor << state.invest_count) && card.price <= (worth_buy_invest_price_threshold_factor << state.invest_count) && state.current_turn < invest_stop_turn && state.invest_count < Consts::max_invest_num;
    }
  }

  const auto& best_card_id = nn.get_best_card_id(environment, state, use_card_output, turn_input);
  for (const auto& card_id: best_card_id) {
    const auto& card = turn_input.deck_cards[card_id];
    if (!can_afford[card_id]) continue;
    if (!worth_buying[card_id]) continue;
    if (card.type == CancelAll) continue; // 業務転換カードは無視しておく
    if (card.type == Invest && state.invest_count == Consts::max_invest_num) continue; // 増資回数上限に到達している場合は増資カードは無視
    return GetCardOutput(card_id);
  }

  return GetCardOutput(0);
}

void solve(const InitialInput& initial_input) {
  const auto environment = create_environment(initial_input);
  auto state = create_state(initial_input);

  const auto monte_carlo_environment = MonteCarloEnvironment(environment);
  auto monte_carlo_state = MonteCarloState();

  UseCardOutput prev_use_card_output;
  for (int turn = 0; turn < environment.turn_max; turn++) {
    if (turn == 0) {
      // 最初のターンのカード使用はルールベース
      state.current_turn = turn;
      const auto use_card_output = decide_use_card_output(environment, state);
      update_use_card(environment, state, use_card_output);
      prev_use_card_output = use_card_output;
      continue;
    }

    // if (turn % HyperParameter::monte_carlo_interval_turn == 0) {
    //   // 前のターンのカード取得～次のターンのカード使用をモンテカルロシミュレーションで決める
    //   const auto turn_input = get_turn_input(environment);
    //   monte_carlo_state.update_weight(environment, turn_input);
    //   const auto get_card_output = monte_carlo_decide_get_card_output(environment, state, prev_use_card_output, turn_input, monte_carlo_environment, monte_carlo_state);
    //   update_get_card(environment, state, prev_use_card_output, turn_input, get_card_output);
    //   state.current_turn = turn;
    //   const auto use_card_output = decide_use_card_output(environment, state);
    //   update_use_card(environment, state, use_card_output);
    //   prev_use_card_output = use_card_output;
    // }
    // else {
    //   const auto turn_input = get_turn_input(environment);
    //   monte_carlo_state.update_weight(environment, turn_input);
    //   const auto get_card_output = decide_get_card_output(environment, state, prev_use_card_output, turn_input);
    //   update_get_card(environment, state, prev_use_card_output, turn_input, get_card_output);
    //   state.current_turn = turn;
    //   const auto use_card_output = decide_use_card_output(environment, state);
    //   update_use_card(environment, state, use_card_output);
    //   prev_use_card_output = use_card_output;
    // }
    const auto turn_input = get_turn_input(environment);
    monte_carlo_state.update_weight(environment, turn_input);
    const auto get_card_output = nn_decide_get_card_output(environment, state, prev_use_card_output, turn_input);
    update_get_card(environment, state, prev_use_card_output, turn_input, get_card_output);
    state.current_turn = turn;
    const auto use_card_output = decide_use_card_output(environment, state);
    update_use_card(environment, state, use_card_output);
    prev_use_card_output = use_card_output;

    if (turn == environment.turn_max - 1) {
      // 最後のターンのカード取得はルールベース
      const auto turn_input = get_turn_input(environment);
      const auto get_card_output = decide_get_card_output(environment, state, prev_use_card_output, turn_input);
      update_get_card(environment, state, prev_use_card_output, turn_input, get_card_output);
    }
  }

  // cerr << "Score = " << state.money << endl;
  #ifdef NN_CREATE
  cerr << "]}" << flush;
  #endif
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  HyperParameter::load_hyper_parameter(argc, argv);

  const auto input = get_initial_input();

  solve(input);

  return 0;
}
