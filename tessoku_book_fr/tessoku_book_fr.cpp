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
#include <bitset>
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
    return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
    #else
    //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
    //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
    return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
    #endif
  }
};

using namespace std;

/*-----------------------------------------------*/

// dynamic connectivity
// source: https://kopricky.github.io/code/Academic/dynamic_connectivity.html

template<class _Key, class _Hash, bool DOWNSIZE> class UnorderedSetIterator;

template<class _Key, class _Hash = hash<_Key>, bool DOWNSIZE = false>
class UnorderedSet
{
private:
    using iterator = UnorderedSetIterator<_Key, _Hash, DOWNSIZE>;
    friend UnorderedSetIterator<_Key, _Hash, DOWNSIZE>;
    struct bucket {
        _Key _key;
        short int _dist;
        bool _last, _end;
        bucket() noexcept : _dist(-1), _last(false), _end(false){}
        bucket& operator=(const bucket& another) = default;
        ~bucket(){ if(!empty()) _key.~_Key(); }
        inline void clear() noexcept { _dist = -1; }
        inline void _delete(){ _dist = -1, _key.~_Key(); }
        inline bool empty() const noexcept { return (_dist == -1); }
    };
    inline static unsigned int ceilpow2(unsigned int u) noexcept {
        if(u == 0u) return 0u;
        --u, u |= u >> 1, u |= u >> 2, u |= u >> 4, u |= u >> 8;
        return (u | (u >> 16)) + 1;
    }
    inline static bucket *increment(bucket *cur) noexcept {
        for(++cur; !cur->_end; ++cur){
            if(!cur->empty()) break;
        }
        return cur;
    }
    inline bucket *next_bucket(bucket *cur) const noexcept {
        return cur->_last ? _buckets : cur + 1;
    }
    inline unsigned int make_hash(const _Key& key) const noexcept {
        return _Hash()(key);
    }
    inline float load_rate() const noexcept {
        return (float)_data_count / _bucket_count;
    }
    bucket *insert(bucket *cur, short int dist, _Key&& key){
        bucket *ret = cur;
        bool flag = false;
        while(true){
            if(cur->empty()){
                cur->_key = move(key), cur->_dist = dist;
                if(!flag) ret = cur, flag = true;
                break;
            }else if(dist > cur->_dist){
                swap(key, cur->_key), swap(dist, cur->_dist);
                if(!flag) ret = cur, flag = true;
            }
            ++dist;
            cur = next_bucket(cur);
        }
        return ret;
    }
    bucket *_find(const _Key& key) const {
        bucket *cur = _buckets + (make_hash(key) & _mask);
        int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        return _buckets + _bucket_count;
    }
    template<class Key>
    bucket *find_insert(Key&& key){
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (hash & _mask);
        int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        ++_data_count;
        if(rehash_check()){
            cur = _buckets + (hash & _mask), dist = 0;
        }
        _Key new_key = forward<Key>(key);
        return insert(cur, dist, move(new_key));
    }
    template<typename... Args>
    bucket *emplace(Args&&... args){
        return find_insert(_Key(forward<Args>(args)...));
    }
    bucket *backward_shift(bucket *cur, bool next_ret){
        bucket *next = next_bucket(cur), *ret = cur;
        if(next->_dist < 1) return next_ret ? increment(cur) : cur;
        do {
            cur->_key = next->_key, cur->_dist = next->_dist - 1;
            cur = next, next = next_bucket(cur);
        }while(next->_dist >= 1);
        cur->clear();
        return ret;
    }
    bucket *erase_impl(bucket *cur, bool next_ret){
        assert(static_cast<size_t>(cur - _buckets) != _bucket_count);
        cur->_delete();
        --_data_count;
        return backward_shift(cur, next_ret);
    }
    bucket *erase_itr(bucket *cur, bool next_ret = true){
        const _Key key = cur->_key;
        return erase_impl(rehash_check() ? _find(key) : cur, next_ret);
    }
    bucket *erase_key(const _Key& key, bool next_ret = true){
        rehash_check();
        return erase_impl(_find(key), next_ret);
    }
    bool rehash_check(){
        if(_bucket_count == 0){
            rehash(1u);
            return true;
        }else if(load_rate() >= MAX_LOAD_RATE){
            rehash(_bucket_count * 2u);
            return true;
        }else if(DOWNSIZE){
            if(load_rate() <= MIN_LOAD_RATE && _bucket_count >= DOWNSIZE_THRESHOLD){
                rehash(_bucket_count / 2u);
                return true;
            }
        }
        return false;
    }
    void move_data(bucket *cur){
        insert(_buckets + (make_hash(cur->_key) & _mask), 0, move(cur->_key));
    }
    void rehash(unsigned int new_bucket_count){
        UnorderedSet new_unordered_set(new_bucket_count);
        new_unordered_set._data_count = _data_count;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            if(!cur->empty()){
                new_unordered_set.move_data(cur);
            }
        }
        swap(*this, new_unordered_set);
    }
    friend void swap(UnorderedSet& ust1, UnorderedSet& ust2){
        swap(ust1._bucket_count, ust2._bucket_count);
        swap(ust1._mask, ust2._mask);
        swap(ust1._data_count, ust2._data_count);
        swap(ust1._buckets, ust2._buckets);
    }

private:
    unsigned int _bucket_count, _mask, _data_count;
    bucket *_buckets;
public:
    const float MAX_LOAD_RATE = 0.5f;
    const float MIN_LOAD_RATE = 0.1f;
    const unsigned int DOWNSIZE_THRESHOLD = 16u;
    UnorderedSet(unsigned int bucket_size = 0u)
     : _bucket_count(ceilpow2(bucket_size)), _mask(_bucket_count - 1),
        _data_count(0u), _buckets(new bucket[_bucket_count + 1]){
        if(_bucket_count > 0) _buckets[_bucket_count - 1]._last = true;
        else _mask = 0;
        _buckets[_bucket_count]._end = true;
    }
    UnorderedSet(const UnorderedSet& another)
        : _bucket_count(another._bucket_count), _mask(another._mask), _data_count(another._data_count){
        _buckets = new bucket[_bucket_count + 1u];
        for(unsigned int i = 0u; i <= _bucket_count; ++i){
            _buckets[i] = another._buckets[i];
        }
    }
    UnorderedSet(UnorderedSet&& another)
        : _bucket_count(move(another._bucket_count)), _mask(move(another._mask)),
            _data_count(move(another._data_count)), _buckets(another._buckets){
        another._buckets = nullptr;
    }
    UnorderedSet& operator=(const UnorderedSet& another){
        delete[] _buckets;
        _bucket_count = another._bucket_count;
        _mask = another._mask;
        _data_count = another._data_count;
        _buckets = new bucket[_bucket_count + 1u];
        for(unsigned int i = 0u; i <= _bucket_count; ++i){
            _buckets[i] = another._buckets[i];
        }
        return *this;
    }
    UnorderedSet& operator=(UnorderedSet&& another){
        delete[] _buckets;
        _bucket_count = move(another._bucket_count);
        _mask = move(another._mask);
        _data_count = move(another._data_count);
        _buckets = another._buckets;
        another._buckets = nullptr;
        return *this;
    }
    void allocate(unsigned int element_size){
        rehash(ceilpow2(ceil(element_size / MAX_LOAD_RATE) + 1));
    }
    ~UnorderedSet(){ delete[] _buckets; }
    friend ostream& operator<< (ostream& os, UnorderedSet& ust) noexcept {
        for(_Key& val : ust) os << val << " ";
        return os;
    }
    void clear(){
        UnorderedSet new_unordered_set(0u);
        swap(*this, new_unordered_set);
    }
    size_t size() const noexcept { return _data_count; }
    size_t bucket_count() const noexcept { return _bucket_count; }
    bool empty() const noexcept { return (_data_count == 0); }
    iterator begin() noexcept {
        return (_buckets->empty() && _bucket_count > 0) ? iterator(increment(_buckets)) : iterator(_buckets);
    }
    iterator end() noexcept { return iterator(_buckets + _bucket_count); }
    iterator find(const _Key& key) const { return iterator(_find(key)); }
    size_t count(const _Key& key) const { return (_find(key) != _buckets + _bucket_count); }
    iterator insert(const _Key& key){ return iterator(find_insert(key)); }
    iterator insert(_Key&& key){ return iterator(find_insert(move(key))); }
    template<typename... Args>
    iterator emplace(Args&&... args){ return iterator(_emplace(forward<Args>(args)...)); }
    iterator erase(const _Key& key){ return iterator(erase_key(key)); }
    iterator erase(const iterator& itr){ return iterator(erase_itr(itr.bucket_ptr)); }
    void simple_erase(const _Key& key){ erase_key(key, false); }
    void simple_erase(const iterator& itr){ erase_itr(itr.bucket_ptr, false); }

    // DEBUG 用
    short int maximum_distance() const noexcept {
        short int ret = -1;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            ret = max(ret, cur->_dist);
        }
        return ret;
    }
};

template<class _Key, class _Hash, bool DOWNSIZE>
class UnorderedSetIterator {
private:
    friend UnorderedSet<_Key, _Hash, DOWNSIZE>;
    typename UnorderedSet<_Key, _Hash, DOWNSIZE>::bucket *bucket_ptr;
    using iterator_category = forward_iterator_tag;
    using value_type = _Key;
    using difference_type = ptrdiff_t;
    using pointer = _Key*;
    using reference = _Key&;

private:
    UnorderedSetIterator(typename UnorderedSet<_Key, _Hash, DOWNSIZE>::bucket *_bucket_ptr)
        noexcept : bucket_ptr(_bucket_ptr){}
public:
    UnorderedSetIterator() noexcept : bucket_ptr(){}
    UnorderedSetIterator(const UnorderedSetIterator& itr) noexcept : bucket_ptr(itr.bucket_ptr){}
    UnorderedSetIterator& operator=(const UnorderedSetIterator& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    UnorderedSetIterator& operator=(const UnorderedSetIterator&& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    reference operator*() const noexcept { return bucket_ptr->_key; }
    pointer operator->() const noexcept { return &(bucket_ptr->_key); }
    UnorderedSetIterator& operator++() noexcept {
        return bucket_ptr = UnorderedSet<_Key, _Hash, DOWNSIZE>::increment(bucket_ptr), *this;
    }
    UnorderedSetIterator operator++(int) const noexcept {
        return UnorderedSetIterator(UnorderedSet<_Key, _Hash, DOWNSIZE>::increment(this->bucket_ptr));
    }
    bool operator==(const UnorderedSetIterator& itr) const noexcept { return !(*this != itr); };
    bool operator!=(const UnorderedSetIterator& itr) const noexcept { return bucket_ptr != itr.bucket_ptr; }
};

template<class _Key, class _Tp, class _Hash, bool DOWNSIZE> class UnorderedMapIterator;

template<class _Key, class _Tp, class _Hash = hash<_Key>, bool DOWNSIZE = false>
class UnorderedMap
{
private:
    using iterator = UnorderedMapIterator<_Key, _Tp, _Hash, DOWNSIZE>;
    using value_type = _Tp;
    using data_type = pair<_Key, _Tp>;
    using aligned_pointer = typename aligned_storage<sizeof(value_type), alignof(value_type)>::type;
    friend UnorderedMapIterator<_Key, _Tp, _Hash, DOWNSIZE>;
    struct bucket {
        _Key _key;
        short int _dist;
        bool _last, _end;
        aligned_pointer _value_ptr;
        bucket() noexcept : _dist(-1), _last(false), _end(false){}
        bucket& operator=(const bucket& another) noexcept {
            _key = another._key, _dist = another._dist, _last = another._last, _end = another._end;
            if(!another.empty()){
                new(&_value_ptr) value_type(*reinterpret_cast<const value_type*>(&another._value_ptr));
            }
            return *this;
        }
        ~bucket(){ if(!empty()) _delete(); }
        inline void clear() noexcept { _dist = -1; }
        inline void _delete(){ _dist = -1, value_ptr()->~value_type(); }
        inline bool empty() const noexcept { return (_dist == -1); }
        inline value_type& value() noexcept {
            return *reinterpret_cast<value_type*>(&_value_ptr);
        }
        inline value_type* value_ptr() noexcept {
            return reinterpret_cast<value_type*>(&_value_ptr);
        }
        inline void new_value(value_type&& value){
            new(&_value_ptr) value_type(move(value));
        }
    };
    inline static unsigned int ceilpow2(unsigned int u) noexcept {
        if(u == 0u) return 0u;
        --u, u |= u >> 1, u |= u >> 2, u |= u >> 4, u |= u >> 8;
        return (u | (u >> 16)) + 1u;
    }
    inline static bucket *increment(bucket *cur) noexcept {
        for(++cur; !cur->_end; ++cur){
            if(!cur->empty()) break;
        }
        return cur;
    }
    inline bucket *next_bucket(bucket *cur) const noexcept {
        return cur->_last ? _buckets : cur + 1;
    }
    inline unsigned int make_hash(const _Key& key) const noexcept {
        return _Hash()(key);
    }
    inline float load_rate() const noexcept {
        return (float)_data_count / _bucket_count;
    }
    bucket *insert(bucket *cur, _Key&& key, short int dist, value_type&& value){
        bucket *ret = cur;
        bool flag = false;
        while(true){
            if(cur->empty()){
                cur->_key = move(key), cur->_dist = dist, cur->new_value(move(value));
                if(!flag) ret = cur, flag = true;
                break;
            }else if(dist > cur->_dist){
                swap(key, cur->_key), swap(dist, cur->_dist), swap(value, cur->value());
                if(!flag) ret = cur, flag = true;
            }
            ++dist;
            cur = next_bucket(cur);
        }
        return ret;
    }
    template<class Key>
    bucket *_find(Key&& key, bool push = false){
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (hash & _mask);
        short int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        if(!push) return _buckets + _bucket_count;
        ++_data_count;
        if(rehash_check()){
            cur = _buckets + (hash & _mask), dist = 0;
        }
        value_type new_value = value_type();
        _Key new_key = forward<Key>(key);
        return insert(cur, move(new_key), dist, move(new_value));
    }
    template<class Data>
    bucket *find_insert(Data&& data){
        const _Key& key = data.first;
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (hash & _mask);
        short int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        ++_data_count;
        if(rehash_check()){
            cur = _buckets + (hash & _mask), dist = 0;
        }
        data_type new_data = forward<Data>(data);
        return insert(cur, move(new_data.first), dist, move(new_data.second));
    }
    template<typename... Args>
    bucket *emplace(Args&&... args){
        return find_insert(data_type(forward<Args>(args)...));
    }
    bucket *backward_shift(bucket *cur, bool next_ret){
        bucket *next = next_bucket(cur), *ret = cur;
        if(next->_dist < 1) return next_ret ? increment(cur) : cur;
        do {
            cur->_key = next->_key, cur->_dist = next->_dist - 1;
            cur->new_value(move(next->value()));
            cur = next, next = next_bucket(cur);
        }while(next->_dist >= 1);
        cur->clear();
        return ret;
    }
    bucket *erase_impl(bucket *cur, bool next_ret){
        assert(static_cast<size_t>(cur - _buckets) != _bucket_count);
        cur->_delete();
        --_data_count;
        return backward_shift(cur, next_ret);
    }
    bucket *erase_itr(bucket *cur, bool next_ret = true){
        const _Key key = cur->_key;
        return erase_impl(rehash_check() ? _find(key) : cur, next_ret);
    }
    bucket *erase_key(const _Key& key, bool next_ret = true){
        rehash_check();
        return erase_impl(_find(key), next_ret);
    }
    bool rehash_check(){
        if(_bucket_count == 0){
            rehash(1u);
            return true;
        }else if(load_rate() >= MAX_LOAD_RATE){
            rehash(_bucket_count * 2u);
            return true;
        }else if(DOWNSIZE){
            if(load_rate() <= MIN_LOAD_RATE && _bucket_count >= DOWNSIZE_THRESHOLD){
                rehash(_bucket_count / 2u);
                return true;
            }
        }
        return false;
    }
    void move_data(bucket *cur){
        insert(_buckets + (make_hash(cur->_key) & _mask), move(cur->_key), 0, move(cur->value()));
    }
    void rehash(unsigned int new_bucket_count){
        UnorderedMap new_unordered_map(new_bucket_count);
        new_unordered_map._data_count = _data_count;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            if(!cur->empty()){
                new_unordered_map.move_data(cur);
            }
        }
        swap(*this, new_unordered_map);
    }
    friend void swap(UnorderedMap& ump1, UnorderedMap& ump2){
        swap(ump1._bucket_count, ump2._bucket_count);
        swap(ump1._mask, ump2._mask);
        swap(ump1._data_count, ump2._data_count);
        swap(ump1._buckets, ump2._buckets);
    }

private:
    unsigned int _bucket_count, _mask, _data_count;
    bucket *_buckets;
public:
    const float MAX_LOAD_RATE = 0.5f;
    const float MIN_LOAD_RATE = 0.1f;
    const unsigned int DOWNSIZE_THRESHOLD = 16u;
    UnorderedMap(unsigned int bucket_size = 0u)
     : _bucket_count(ceilpow2(bucket_size)), _mask(_bucket_count - 1),
        _data_count(0u), _buckets(new bucket[_bucket_count + 1]){
        if(_bucket_count > 0) _buckets[_bucket_count - 1]._last = true;
        else _mask = 0;
        _buckets[_bucket_count]._end = true;
    }
    UnorderedMap(const UnorderedMap& another)
        : _bucket_count(another._bucket_count), _mask(another._mask), _data_count(another._data_count){
        _buckets = new bucket[_bucket_count + 1u];
        for(unsigned int i = 0u; i <= _bucket_count; ++i){
            _buckets[i] = another._buckets[i];
        }
    }
    UnorderedMap(UnorderedMap&& another)
        : _bucket_count(move(another._bucket_count)), _mask(move(another._mask)),
            _data_count(move(another._data_count)), _buckets(another._buckets){
        another._buckets = nullptr;
    }
    UnorderedMap& operator=(const UnorderedMap& another){
        delete[] _buckets;
        _bucket_count = another._bucket_count;
        _mask = another._mask;
        _data_count = another._data_count;
        _buckets = new bucket[_bucket_count + 1u];
        for(unsigned int i = 0u; i <= _bucket_count; ++i){
            _buckets[i] = another._buckets[i];
        }
        return *this;
    }
    UnorderedMap& operator=(UnorderedMap&& another){
        delete[] _buckets;
        _bucket_count = move(another._bucket_count);
        _mask = move(another._mask);
        _data_count = move(another._data_count);
        _buckets = another._buckets;
        another._buckets = nullptr;
        return *this;
    }
    void allocate(unsigned int element_size){
        rehash(ceilpow2(ceil(element_size / MAX_LOAD_RATE) + 1));
    }
    ~UnorderedMap(){ delete[] _buckets; }
    friend ostream& operator<< (ostream& os, UnorderedMap& ump) noexcept {
        for(auto val : ump) os << '{' << val.first << ',' << val.second << "} ";
        return os;
    }
    _Tp& operator[](const _Key& key){ return _find(key, true)->value(); }
    _Tp& operator[](_Key&& key){ return _find(move(key), true)->value(); }
    const _Tp& at(const _Key& key){
        bucket *res = _find(key);
        if(res == _buckets + _bucket_count) __throw_out_of_range(__N("Unordered_Map::at"));
        return res->value();
    }
    void clear(){
        UnorderedMap new_unordered_map(0u);
        swap(*this, new_unordered_map);
    }
    size_t size() const noexcept { return _data_count; }
    size_t bucket_count() const noexcept { return _bucket_count; }
    bool empty() const noexcept { return (_data_count == 0); }
    iterator begin() noexcept {
        return (_buckets->empty() && _bucket_count > 0) ? iterator(increment(_buckets)) : iterator(_buckets);
    }
    iterator end() noexcept { return iterator(_buckets + _bucket_count); }
    iterator find(const _Key& key){ return iterator(_find(key)); }
    iterator insert(const data_type& data){ return iterator(find_insert(data)); }
    iterator insert(data_type&& data){ return iterator(find_insert(move(data))); }
    template<typename... Args>
    iterator emplace(Args&&... args){ return iterator(_emplace(forward<Args>(args)...)); }
    iterator erase(const _Key& key){ return iterator(erase_key(key)); }
    iterator erase(const iterator& itr){ return iterator(erase_itr(itr.bucket_ptr)); }
    void simple_erase(const _Key& key){ erase_key(key, false); }
    void simple_erase(const iterator& itr){ erase_itr(itr.bucket_ptr, false); }

    // DEBUG 用
    short int maximum_distance() const noexcept {
        short int ret = -1;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            ret = max(ret, cur->_dist);
        }
        return ret;
    }
};

template<class _Key, class _Tp, class _Hash, bool DOWNSIZE>
class UnorderedMapIterator {
private:
    friend UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>;
    typename UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::bucket *bucket_ptr;
    using iterator_category = forward_iterator_tag;
    using value_type = pair<const _Key, _Tp>;
    using difference_type = ptrdiff_t;
    using reference = pair<const _Key&, _Tp&>;

private:
    UnorderedMapIterator(typename UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::bucket *_bucket_ptr)
        noexcept : bucket_ptr(_bucket_ptr){}
public:
    UnorderedMapIterator() noexcept : bucket_ptr(){}
    UnorderedMapIterator(const UnorderedMapIterator& itr) noexcept : bucket_ptr(itr.bucket_ptr){}
    UnorderedMapIterator& operator=(const UnorderedMapIterator& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    UnorderedMapIterator& operator=(const UnorderedMapIterator&& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    reference operator*() const noexcept { return {bucket_ptr->_key, bucket_ptr->value()}; }
    UnorderedMapIterator& operator++() noexcept {
        return bucket_ptr = UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::increment(bucket_ptr), *this;
    }
    UnorderedMapIterator operator++(int) const noexcept {
        return UnorderedMapIterator(UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::increment(this->bucket_ptr));
    }
    bool operator==(const UnorderedMapIterator& itr) const noexcept { return !(*this != itr); };
    bool operator!=(const UnorderedMapIterator& itr) const noexcept { return bucket_ptr != itr.bucket_ptr; }
};

// 多重辺は無いと仮定する
class BSTNode {
public:
    struct murmur_hash32 {
        unsigned int operator()(int p) const {
            const unsigned int m = 0x5bd1e995; p *= m;
            unsigned int h = (p^(p>>24))*m;
            return h = (h^(h>>13))*m, (h^(h>>15));
        }
    };
    const int from, to;
    int sz;
    bool subtree_edge, subofftree_edge, exact_level;
    BSTNode *left, *right, *par;
    UnorderedSet<int, murmur_hash32, true> adjacent;
    BSTNode(const int _ver) noexcept :
        from(_ver), to(_ver), sz(1), subtree_edge(false), subofftree_edge(false),
            exact_level(false), left(nullptr), right(nullptr), par(nullptr){}
    BSTNode(const int _from, const int _to, const bool _flag) noexcept :
        from(_from), to(_to), sz(0), subtree_edge(false), subofftree_edge(false),
            exact_level((from < to) && _flag), left(nullptr), right(nullptr), par(nullptr){}
    inline bool IsRoot() const noexcept { return !par; }
    inline bool IsVertex() const noexcept { return (from == to); }
    inline void eval() noexcept {
        if(IsVertex()) sz = 1, subtree_edge = false, subofftree_edge = !adjacent.empty();
        else sz = 0, subtree_edge = exact_level, subofftree_edge = false;
        if(left){
            sz += left->sz, subtree_edge |= left->subtree_edge, subofftree_edge |= left->subofftree_edge;
        }
        if(right){
            sz += right->sz, subtree_edge |= right->subtree_edge, subofftree_edge |= right->subofftree_edge;
        }
    }
    inline void subtree_edge_eval(){
        subtree_edge = exact_level;
        if(left) subtree_edge |= left->subtree_edge;
        if(right) subtree_edge |= right->subtree_edge;
    }
    inline void subofftree_edge_eval(){
        subofftree_edge = !adjacent.empty();
        if(left) subofftree_edge |= left->subofftree_edge;
        if(right) subofftree_edge |= right->subofftree_edge;
    }
    inline bool subofftree_check(){
        return !adjacent.empty() ||
                (left ? left->subofftree_edge : false) || (right ? right->subofftree_edge : false);
    }
    inline bool offtree_check(){
        return adjacent.empty() ||
                (left ? left->subofftree_edge : false) || (right ? right->subofftree_edge : false);
    }
    void rotate(const bool right_) noexcept {
        BSTNode *p = par, *g = p->par;
        if(right_){
            if((p->left = right)) right->par = p;
            right = p, p->par = this;
        }else{
            if((p->right = left)) left->par = p;
            left = p, p->par = this;
        }
        p->eval(), eval();
        if(!(par = g)) return;
        if(g->left == p) g->left = this;
        if(g->right == p) g->right = this;
        g->eval();
    }
};

BSTNode *splay(BSTNode *u) noexcept {
    if(!u) return nullptr;
    while(!(u->IsRoot())){
        BSTNode *p = u->par, *gp = p->par;
        if(p->IsRoot()){
            u->rotate((u == p->left));
        }else{
            bool flag = (u == p->left);
            if((u == p->left) == (p == gp->left)){
                p->rotate(flag), u->rotate(flag);
            }else{
                u->rotate(flag), u->rotate(!flag);
            }
        }
    }
    return u;
}

BSTNode *join(BSTNode *root1, BSTNode *root2) noexcept {
    if(!root1 || !root2) return root1 ? root1 : root2;
    BSTNode *cur = nullptr, *nx = root1;
    do{ cur = nx, nx = cur->right; }while(nx);
    BSTNode *ver = splay(cur);
    ver->right = root2, ver->eval(), root2->par = ver;
    return ver;
}

class EulerTourTree {
public:
    struct murmur_hash64 {
        unsigned long long operator()(unsigned long long p) const {
            const unsigned long long m = 0xc6a4a7935bd1e995; p *= m;
            unsigned long long h = (p^(p>>47))*m;
            return h = (h^(h>>47))*m, (h^(h>>47));
        }
    };
    inline static unsigned long long pair_to_ll(const int u, const int v){
        return ((unsigned long long)(u) << 32) | v;
    }
    const int V;
    BSTNode** vertex_set;
    UnorderedMap<unsigned long long, pair<BSTNode*, BSTNode*>, murmur_hash64> edge_set;
private:
    void dfs(const int u, const int p, const BSTNode *cur,
        bool *visit, vector<BSTNode*>& nodes, const vector<vector<int> >& G) noexcept {
        visit[u] = true;
        nodes.push_back(vertex_set[u]);
        for(auto& v : G[u]){
            if(!visit[v]){
                BSTNode* e1 = new BSTNode(u, v, true);
                nodes.push_back(e1);
                dfs(v, u, cur, visit, nodes, G);
                BSTNode* e2 = new BSTNode(v, u, true);
                if(u < v) edge_set[pair_to_ll(u, v)] = {e1, e2};
                else edge_set[pair_to_ll(v, u)] = {e2, e1};
                nodes.push_back(e2);
            }else if(v != p){
                vertex_set[u]->adjacent.insert(v);
                vertex_set[u]->subofftree_edge = true;
            }
        }
    }
    void bst_build(vector<BSTNode*>& nodes) noexcept {
        int i, n = (int)nodes.size(), st = 2, isolate = ((n % 4 == 1) ? (n-1) : -1);
        while(st <= n){
            for(i = st-1; i < n; i += 2*st){
                nodes[i]->left = nodes[i-st/2], nodes[i-st/2]->par = nodes[i];
                if(i+st/2 < n) nodes[i]->right = nodes[i+st/2], nodes[i+st/2]->par = nodes[i];
                else if(isolate >= 0) nodes[i]->right = nodes[isolate], nodes[isolate]->par = nodes[i];
                nodes[i]->eval();
            }
            isolate = ((n % (4*st) >= st && (n % (4*st) < 2*st)) ? (i-2*st): isolate);
            st <<= 1;
        }
    }
    void build_forest(const vector<vector<int> >& G){
        bool *visit = new bool[V]();
        for(int i = 0; i < V; ++i){
            if(!visit[i]){
                vector<BSTNode*> nodes;
                BSTNode *cur = nullptr;
                dfs(i, -1, cur, visit, nodes, G);
                bst_build(nodes);
            }
        }
    }
    BSTNode *reroot(BSTNode *ver) noexcept {
        BSTNode *res = splay(ver)->left;
        if(!res) return ver;
        ver->left = nullptr, ver->eval();
        while(ver->right) ver = ver->right;
        splay(ver), ver->right = res, ver->eval(), res->par = ver;
        return ver;
    }
    void link(BSTNode *ver1, BSTNode *ver2, const bool flag) noexcept {
        BSTNode *e1 = new BSTNode(ver1->from, ver2->from, flag);
        BSTNode *e2 = new BSTNode(ver2->from, ver1->from, flag);
        edge_set[pair_to_ll(ver1->from, ver2->from)] = {e1, e2};
        join(join(reroot(ver1), e1), join(reroot(ver2), e2));
    }
    void cut(BSTNode *edge1, BSTNode *edge2) noexcept {
        splay(edge1), splay(edge2);
        BSTNode *p = edge1->par;
        bool _right = (edge1 == edge2->right);
        if(p != edge2){
            _right = (p == edge2->right);
            p->par = nullptr, edge1->rotate((edge1 == p->left));
        }
        if(edge1->left) edge1->left->par = nullptr;
        if(edge1->right) edge1->right->par = nullptr;
        if(_right){
            if(edge2->left) edge2->left->par = nullptr;
            join(edge2->left, edge1->right);
        }else{
            if(edge2->right) edge2->right->par = nullptr;
            join(edge1->left, edge2->right);
        }
        delete edge1; delete edge2;
    }
    bool connected(BSTNode *ver1, BSTNode *ver2) noexcept {
        splay(ver1), splay(ver2);
        return ver1->par;
    }
    int component_size(BSTNode *ver) noexcept { return splay(ver)->sz; }
public:
    EulerTourTree(const int node_size) : V(node_size), vertex_set(new BSTNode*[V]){
        for(int i = 0; i < V; i++) vertex_set[i] = new BSTNode(i);
    }
    EulerTourTree(const vector<vector<int> >& G) : V((int)G.size()), vertex_set(new BSTNode*[V]){
        for(int i = 0; i < V; i++) vertex_set[i] = new BSTNode(i);
        build_forest(G);
    }
    // ~EulerTourTree(){
    //     for(auto it : edge_set){
    //         delete (it.second).first;
    //         delete (it.second).second;
    //     }
    //     for(int i = 0; i < V; ++i) delete vertex_set[i];
    //     delete[] vertex_set;
    // }
    void reroot(const int node_id) noexcept { reroot(vertex_set[node_id]); }
    void link(int node1_id, int node2_id, bool flag=true) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        link(vertex_set[node1_id], vertex_set[node2_id], flag);
    }
    void cut(int node1_id, int node2_id){
        if(node1_id > node2_id) swap(node1_id, node2_id);
        auto it = edge_set.find(pair_to_ll(node1_id, node2_id));
        assert(it != edge_set.end());
        BSTNode *edge1 = ((*it).second).first, *edge2 = ((*it).second).second;
        edge_set.simple_erase(it);
        cut(edge1, edge2);
    }
    bool connected(const int node1_id, const int node2_id) noexcept {
        if(node1_id == node2_id) return true;
        return connected(vertex_set[node1_id], vertex_set[node2_id]);
    }
    int component_size(const int node_id) noexcept { return component_size(vertex_set[node_id]); }
    void check_dfs(const BSTNode* cur) const noexcept {
        if(cur->left) check_dfs(cur->left);
        cout << "{" << (cur->from) << "," << (cur->to) << "} ";
        if(cur->right) check_dfs(cur->right);
    }
};

class DynamicConnectivity {
private:
    BSTNode *level_up_dfs(BSTNode *cur, const int layer) noexcept {
        if(cur->exact_level){
            splay(cur)->exact_level = false, cur->subtree_edge_eval();
            detect_layer[EulerTourTree::pair_to_ll(cur->from, cur->to)]++;
            et[layer+1].link(cur->from, cur->to);
            return cur;
        }
        if(cur->left && cur->left->subtree_edge) return level_up_dfs(cur->left, layer);
        if(cur->right && cur->right->subtree_edge) return level_up_dfs(cur->right, layer);
        return nullptr;
    }
    BSTNode *search_edge_dfs
        (BSTNode *cur, const int layer, const int another, bool& flag, pair<int, int>& rep_edge) noexcept {
        if(!cur->adjacent.empty()){
            bool state = et[layer+1].vertex_set[cur->from]->adjacent.empty();
            for(auto it = cur->adjacent.begin(); it != cur->adjacent.end();){
                pair<int, int> e = {min(cur->from, *it), max(cur->from, *it)};
                BSTNode *correspond = et[layer].vertex_set[*it];
                if(et[layer].connected(another, *it)){
                    flag = true, rep_edge = e;
                    cur->adjacent.simple_erase(it), correspond->adjacent.simple_erase(cur->from);
                    if(!correspond->subofftree_check()){
                        splay(correspond)->subofftree_edge_eval();
                    }
                    break;
                }else{
                    if(!et[layer+1].vertex_set[*it]->subofftree_check()){
                        splay(et[layer+1].vertex_set[*it])->subofftree_edge = true;
                    }
                    et[layer+1].vertex_set[cur->from]->adjacent.insert(*it);
                    et[layer+1].vertex_set[*it]->adjacent.insert(cur->from);
                    detect_layer[EulerTourTree::pair_to_ll(e.first, e.second)]++;
                    it = cur->adjacent.erase(it);
                    correspond->adjacent.simple_erase(cur->from);
                    if(!correspond->subofftree_check()){
                        splay(correspond)->subofftree_edge_eval();
                    }
                }
            }
            if(state && !et[layer+1].vertex_set[cur->from]->offtree_check()){
                splay(et[layer+1].vertex_set[cur->from])->subofftree_edge = true;
            }
            splay(cur)->subofftree_edge_eval();
            return cur;
        }
        if(cur->left && cur->left->subofftree_edge){
            return search_edge_dfs(cur->left, layer, another, flag, rep_edge);
        }
        if(cur->right && cur->right->subofftree_edge){
            return search_edge_dfs(cur->right, layer, another, flag, rep_edge);
        }
        return nullptr;
    }
    bool replace(const int from, const int to, const int layer) noexcept {
        if(layer < 0) return true;
        int u, v;
        if(et[layer].component_size(from) <= et[layer].component_size(to)) u = from, v = to;
        else u = to, v = from;
        BSTNode *ver = splay(et[layer].vertex_set[u]);
        while(ver->subtree_edge) ver = level_up_dfs(ver, layer);
        pair<int, int> rep_edge = {-1, -1};
        bool flag = false;
        while(ver->subofftree_edge){
            ver = search_edge_dfs(ver, layer, v, flag, rep_edge);
            if(flag) break;
        }
        if(flag){
            et[layer].link(rep_edge.first, rep_edge.second);
            for(int i = 0; i < layer; i++) et[i].link(rep_edge.first, rep_edge.second, false);
            return false;
        }else return replace(from, to, layer-1);
    }
public:
    const int V;
    int depth;
    vector<EulerTourTree> et;
    UnorderedMap<unsigned long long, int, EulerTourTree::murmur_hash64> detect_layer;
    DynamicConnectivity(const int node_size) noexcept : V(node_size), depth(1){
        et.emplace_back(V);
    }
    DynamicConnectivity(const vector<vector<int> >& G) noexcept : V((int)G.size()), depth(1){
        for(int i = 0; i < V; ++i){
            for(const int j : G[i]){
                if(i < j) detect_layer[EulerTourTree::pair_to_ll(i, j)] = 0;
            }
        }
        et.emplace_back(G);
    }
    bool link(int node1_id, int node2_id) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        detect_layer[EulerTourTree::pair_to_ll(node1_id, node2_id)] = 0;
        if(et[0].connected(node1_id, node2_id)){
            BSTNode *ver1 = et[0].vertex_set[node1_id], *ver2 = et[0].vertex_set[node2_id];
            splay(ver1)->subofftree_edge = true, ver1->adjacent.insert(node2_id);
            splay(ver2)->subofftree_edge = true, ver2->adjacent.insert(node1_id);
            return false;
        }else{
            et[0].link(node1_id, node2_id);
            return true;
        }
    }
    bool cut(int node1_id, int node2_id) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        auto it = detect_layer.find(EulerTourTree::pair_to_ll(node1_id, node2_id));
        assert(it != detect_layer.end());
        int layer = (*it).second;
        detect_layer.simple_erase(it);
        auto& st = et[layer].vertex_set[node1_id]->adjacent;
        if(st.find(node2_id) == st.end()){
            for(int i = 0; i <= layer; i++) et[i].cut(node1_id, node2_id);
            if(layer + 1 == depth) ++depth, et.emplace_back(V);
            return replace(node1_id, node2_id, layer);
        }else{
            et[layer].vertex_set[node1_id]->adjacent.simple_erase(node2_id);
            if(!et[layer].vertex_set[node1_id]->subofftree_check()){
                splay(et[layer].vertex_set[node1_id])->subofftree_edge_eval();
            }
            et[layer].vertex_set[node2_id]->adjacent.simple_erase(node1_id);
            if(!et[layer].vertex_set[node2_id]->subofftree_check()){
                splay(et[layer].vertex_set[node2_id])->subofftree_edge_eval();
            }
            return false;
        }
    }
    bool connected(const int node1_id, const int node2_id) noexcept {
        return et[0].connected(node1_id, node2_id);
    }
};

/*-----------------------------------------------*/

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;
mt19937 theMersenne(1);

// hyper parameters

// structs
struct index_set {
  vector<int> included;
  vector<int> included_pos;
  vector<int> excluded;
  vector<int> excluded_pos;

  index_set() {};

  index_set(int n) {
    included.reserve(n);
    excluded.reserve(n);

    for (int i = 0; i < n; i++) excluded.push_back(i);

    included_pos = vector<int>(n, -1);
    excluded_pos = vector<int>(n);
    for (int i = 0; i < n; i++) excluded_pos[i] = i;
  }

  void include(int index) {
    assert(0 <= index && index < (int) excluded.size());
    included.push_back(excluded[index]);
    included_pos[excluded[index]] = included.size()-1;
    swap(excluded_pos[excluded[index]], excluded_pos[excluded[excluded.size()-1]]);
    excluded_pos[excluded[index]] = -1;
    swap(excluded[index], excluded[excluded.size()-1]);
    excluded.pop_back();
  }

  void exclude(int index) {
    assert(0 <= index && index < (int) included.size());
    excluded.push_back(included[index]);
    excluded_pos[included[index]] = excluded.size()-1;
    swap(included_pos[included[index]], included_pos[included[included.size()-1]]);
    included_pos[included[index]] = -1;
    swap(included[index], included[included.size()-1]);
    included.pop_back();
  }

  bool is_included(int element) {
    return included_pos[element] != -1;
  }

  bool is_excluded(int element) {
    return excluded_pos[element] != -1;
  }

  bool include_element(int element) {
    assert(0 <= element && element < (int) excluded_pos.size());
    if (is_excluded(element)) {
      include(excluded_pos[element]);
      return true;
    }
    else return false;
  }

  bool exclude_element(int element) {
    assert(0 <= element && element < (int) included_pos.size());
    if (is_included(element)) {
      exclude(included_pos[element]);
      return true;
    }
    else return false;
  }
};

// constants
constexpr int n = 50;
constexpr int k = 400;
constexpr int l = 20;
struct {
  const vector<int>  x = {  0,  -1,   0,   1};
  const vector<int>  y = { -1,   0,   1,   0};
  const vector<char> c = {'L', 'U', 'R', 'D'};
} delta;

// inputs
vector<int> a(k), b(k);
vector<vector<int>> c(n, vector<int>(n));

// outputs
vector<int> ans(k);

// internals
vector<vector<int>> city_graph;
unordered_set<int> articulation_points;
unordered_set<int> non_rootable_points;

vector<int> total_a;
vector<int> total_b;
vector<DynamicConnectivity> district_connection;
vector<index_set> sp_districts;

void get_input() {
  int _n, _k, _l;
  cin >> _n >> _k >> _l;
  for (int i = 0; i < k; i++) cin >> a[i] >> b[i];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) cin >> c[i][j];
  }
}

inline bool within_board(int x, int y) {
  return 0 <= x && x < n && 0 <= y && y < n;
}

void init() {
  city_graph = vector<vector<int>>(k);
  {
    vector<vector<bool>> visited(n, vector<bool>(n));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (c[i][j] != 0 && !visited[i][j]) {
          unordered_set<int> adjacent_district;
          queue<Pii> que;
          que.emplace(i, j);
          while (!que.empty()) {
            auto [px, py] = que.front();
            que.pop();
            if (visited[px][py]) continue;
            visited[px][py] = true;
            for (int d = 0; d < 4; d++) {
              int nx = px + delta.x[d];
              int ny = py + delta.y[d];
              if (within_board(nx, ny) && !visited[nx][ny] && c[nx][ny] != 0) {
                if (c[nx][ny] == c[i][j]) que.emplace(nx, ny);
                else adjacent_district.insert(c[nx][ny]);
              }
            }
          }
          for (auto &d: adjacent_district) {
            city_graph[c[i][j]-1].push_back(d-1);
            city_graph[d-1].push_back(c[i][j]-1);
          }
        }
      }
    }
  }

  {
    int rx, ry;
    do {
      rx = theRandom.nextUIntMod(n / 5 + 1) + n * 2 / 5;
      ry = theRandom.nextUIntMod(n / 5 + 1) + n * 2 / 5;
    } while (c[rx][ry] == 0);
    int root = c[rx][ry] - 1;

    unordered_set<int> all_articulation_points;
    vector<int> used(k), ord(k), low(k);
    auto dfs = [&](auto self, int now, int parent, int depth) {
      if (false) return -1;

      used[now] = true;
      ord[now] = depth++;
      low[now] = ord[now];
      bool is_aps = false;
      int children_count = 0;
      for (auto &child: city_graph[now]) {
        if (!used[child]) {
          children_count++;
          depth = self(self, child, now, depth);
          low[now] = min(low[now], low[child]);
          if (parent != -1 && ord[now] <= low[child]) is_aps = true;
        }
        else if (child != parent) {
          low[now] = min(low[now], ord[child]);
        }
      }
      if (parent == -1 && children_count >= 2) is_aps = true;
      if (is_aps) all_articulation_points.insert(now);
      return depth;
    };
    dfs(dfs, root, -1, 0);

    vector<bool> visited(k);
    queue<int> que;
    que.push(root);
    while (!que.empty()) {
      int now = que.front();
      que.pop();
      if (visited[now]) continue;
      visited[now] = true;
      for (auto &next: city_graph[now]) {
        if (all_articulation_points.find(next) != all_articulation_points.end()) articulation_points.insert(next);
        else if (!visited[next]) que.push(next);
      }
    }
    for (auto &p: articulation_points) que.push(p);
    while (!que.empty()) {
      int now = que.front();
      que.pop();
      if (visited[now]) continue;
      visited[now] = true;
      if (articulation_points.find(now) == articulation_points.end()) non_rootable_points.insert(now);
      for (auto &next: city_graph[now]) {
        if (!visited[next]) que.push(next);
      }
    }
  }

  total_a = vector<int>(l);
  total_b = vector<int>(l);
  district_connection = vector<DynamicConnectivity>(l, DynamicConnectivity(k));
  sp_districts = vector<index_set>(l, index_set(k));
}

void add_district(int sp, int dist) {
  assert(sp_districts[sp].is_excluded(dist));
  total_a[sp] += a[dist];
  total_b[sp] += b[dist];
  sp_districts[sp].include_element(dist);
  for (auto &adj: city_graph[dist]) {
    if (sp_districts[sp].is_included(adj)) district_connection[sp].link(dist, adj);
  }
}

bool remove_district(int sp, int dist) {
  assert(sp_districts[sp].is_included(dist));
  int disconnect_count = 0;
  for (auto &adj: city_graph[dist]) {
    if (sp_districts[sp].is_included(adj)) {
      bool is_disconnected = district_connection[sp].cut(dist, adj);
      if (is_disconnected) disconnect_count++;
    }
  }
  if (disconnect_count >= 2) {
    // revert
    for (auto &adj: city_graph[dist]) {
      if (sp_districts[sp].is_included(adj)) district_connection[sp].link(dist, adj);
    }
    return false;
  }
  
  total_a[sp] -= a[dist];
  total_b[sp] -= b[dist];
  sp_districts[sp].exclude_element(dist);
  return true;
}

bool give_district(int from_sp, int to_sp, int dist) {
  assert(ans[dist] == from_sp);
  if (!remove_district(from_sp, dist)) return false;
  add_district(to_sp, dist);
  ans[dist] = to_sp;
  return true;
}

double calc_score() {
  auto [a_min, a_max] = minmax_element(total_a.begin(), total_a.end());
  auto [b_min, b_max] = minmax_element(total_b.begin(), total_b.end());
  return min((double) *(a_min) / (double) *(a_max), (double) *(b_min) / (double) *(b_max));
}

void solve() {
  {
    vector<int> remaining_articulation_points;
    for (auto &p: articulation_points) remaining_articulation_points.push_back(p);
    vector<bool> visited(k);
    vector<index_set> available_adjacent(l, index_set(k));
    for (int i = 0; i < l; i++) {
      int first_district;
      do {
        int rx = theRandom.nextUIntMod(n / 2 + 1) + n / 4;
        int ry = theRandom.nextUIntMod(n / 2 + 1) + n / 4;
        first_district = c[rx][ry] - 1;
      } while (first_district == -1 || visited[first_district] || non_rootable_points.find(first_district) != non_rootable_points.end());
      visited[first_district] = true;
      add_district(i, first_district);
      for (auto &adj: city_graph[first_district]) {
        if (!visited[adj]) available_adjacent[i].include_element(adj);
      }
    }

    unordered_set<int> addable_sp_district;
    for (int i = 0; i < l; i++) addable_sp_district.insert(i);
    while (!addable_sp_district.empty()) {
      int next_sp_district;
      double roll = theRandom.nextDouble();
      if (roll < 0.5) next_sp_district = *min_element(addable_sp_district.begin(), addable_sp_district.end(), [&](auto &x, auto &y){return total_a[x] < total_a[y];});
      else            next_sp_district = *min_element(addable_sp_district.begin(), addable_sp_district.end(), [&](auto &x, auto &y){return total_b[x] < total_b[y];});

      while (!available_adjacent[next_sp_district].included.empty()) {
        int idx = theRandom.nextUIntMod(available_adjacent[next_sp_district].included.size());
        int adj = available_adjacent[next_sp_district].included[idx];
        available_adjacent[next_sp_district].exclude(idx);
        if (visited[adj]) continue;
        visited[adj] = true;
        add_district(next_sp_district, adj);
        for (auto &adjadj: city_graph[adj]) {
          if (!visited[adjadj]) available_adjacent[next_sp_district].include_element(adjadj);
        }
        break;
      }

      if (available_adjacent[next_sp_district].included.empty()) addable_sp_district.erase(next_sp_district);
    }

    for (int i = 0; i < l; i++) {
      for (auto &district: sp_districts[i].included) {
        ans[district] = i;
      }
    }
  }

  double score = calc_score();
  double last_score = score;
  double best_score = score;

  const double base_temperature = 5e-3;
  const double target_temperature = 1e-4;
  // const double decay_rate = 4e-5;
  double temperature = base_temperature;

  double time_start = theTimer.time();
  const double time_limit = 0.980;
  int iter_count = 0;

  while (theTimer.time() < time_limit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.20) {
      int city = theRandom.nextUIntMod(k);
      int adj = city_graph[city][theRandom.nextUIntMod(city_graph[city].size())];
      
      int from_sp = ans[city];
      int to_sp = ans[adj];
      if (sp_districts[from_sp].included.size() == 1) continue;
      if (from_sp == to_sp) continue;

      bool res = give_district(from_sp, to_sp, city);
      if (!res) continue;

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        if (score > best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        give_district(to_sp, from_sp, city);
        score = last_score;
      }
    }
    else if (roll < 1.00) {
      int city1 = theRandom.nextUIntMod(k);
      int adj1 = city_graph[city1][theRandom.nextUIntMod(city_graph[city1].size())];
      
      int from_sp = ans[city1];
      int to_sp = ans[adj1];
      if (sp_districts[from_sp].included.size() == 1) continue;
      if (from_sp == to_sp) continue;

      int city2 = sp_districts[to_sp].included[theRandom.nextUIntMod(sp_districts[to_sp].included.size())];
      bool is_swappable = false;
      for (auto &adj2: city_graph[city2]) {
        if (ans[adj2] == from_sp && adj2 != city1) {
          is_swappable = true;
          break;
        }
      }
      if (!is_swappable) continue;

      bool res1 = give_district(from_sp, to_sp, city1);
      if (!res1) continue;
      bool res2 = give_district(to_sp, from_sp, city2);
      if (!res2) {
        give_district(to_sp, from_sp, city1);
        continue;
      }

      score = calc_score();

      #ifdef DEBUG
      if (iter_count % 1000 == 0) cerr << iter_count << " " << score << " " << last_score << " " << best_score << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= last_score) {
        last_score = score;
        if (score > best_score) {
          best_score = score;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - last_score) / temperature)) { // accept
        last_score = score;
      }
      else { // rollback
        give_district(from_sp, to_sp, city2);
        give_district(to_sp, from_sp, city1);
        score = last_score;
      }
    }

    // temperature *= 1.0 - decay_rate;
    temperature = exp(log(base_temperature) - ((log(base_temperature) - log(target_temperature)) * ((theTimer.time() - time_start) * (1.0 / (time_limit - time_start)))));
    iter_count++;
  }

  for (int i = 0; i < l; i++) {
    cerr << "i = " << setw(2) << i << ", a = " << setw(8) << total_a[i] << ", b = " << setw(6) << total_b[i] << endl;
  }

  cerr << "iter_count   = " << iter_count << endl;
  cerr << "score        = " << score << endl;
  cerr << "best_score   = " << best_score << endl;
  cerr << "temperature  = " << temperature << endl;
}

void output_ans() {
  for (auto &x: ans) cout << x + 1 << endl;
}

int main(int argc, char *argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(false);

  get_input();

  init();
  solve();

  output_ans();

  return 0;
}
