#include "Map.hpp"
#include <chrono>
#include <random>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include <assert.h>
#include <map>
#include <initializer_list>
#include <set>
#include <cassert>

//Enables iteration test on a map larger than the memory available to the remote cluster
//WARNING: This will be VERY slow.
#define DO_BIG_ITERATION_TEST 0

namespace myMap {
  template <typename K, typename V>
  class StdMapWrapper {
  private:
    using base_map = std::map<K, V>;
    
  public:
    typedef typename base_map::iterator Iterator;
    typedef typename base_map::const_iterator ConstIterator;
    typedef typename base_map::reverse_iterator ReverseIterator;
    typedef typename base_map::const_reverse_iterator ConstReverseIterator;
    typedef typename base_map::value_type value_type;
    typedef typename base_map::mapped_type mapped_type;
    typedef typename base_map::key_type key_type;
    
    StdMapWrapper() {}
    StdMapWrapper(std::initializer_list<std::pair<K,V>> il) {
      for(auto x : il) {
        m_map.insert(x);
      }
    }
    
    StdMapWrapper(StdMapWrapper &&other)
      : m_map(std::move(other.m_map))
    {}
    
    StdMapWrapper(const StdMapWrapper &other)
      : m_map(other.m_map)
    {}
    
    StdMapWrapper &operator=(const StdMapWrapper &other) {
      if(this != &other) {
        StdMapWrapper tmp(other);
        std::swap(m_map, tmp.m_map);
      }
      return *this;
    }
    
    StdMapWrapper &operator=(const StdMapWrapper &&other) {
      StdMapWrapper tmp(other);
      std::swap(tmp.m_map, m_map);
      return *this;
    }
    
    ///////// Iterators
    Iterator begin() {
      return m_map.begin();
    }
    
    ConstIterator begin() const {
      return m_map.begin();
    }
    
    ConstIterator cbegin() const {
      return m_map.begin();
    }
    
    ReverseIterator rbegin() {
      return m_map.rbegin();
    }
    
    /*
      ConstReverseIterator rbegin() const {
      return m_map.rbegin();
      }
      
      ConstReverseIterator crbegin() const {
      return m_map.crbegin();
      }
    */
    
    Iterator end() {
      return m_map.end();
    }
    
    ConstIterator end() const {
      return m_map.end();
    }
    
    ConstIterator cend() const {
      return m_map.cend();
    }
    
    ReverseIterator rend() {
      return m_map.rend();
    }
    
    /*
      ConstReverseIterator rend() const {
      return m_map.rend();
      }
    
      ConstReverseIterator crend() const {
      return m_map.crend();
      }
    */
    
    ///////// Capacity
    size_t size() const {
      return m_map.size();
    }
    
    size_t max_size() const {
      return m_map.max_size();
    }
    
    bool empty() const {
      return m_map.empty();
    }
    
    
    ///////// Modifiers
    Iterator insert(const value_type &value) {
      return m_map.insert(value).first;
    }
    
    Iterator insert(value_type &&value) {
      return m_map.insert(std::move(value)).first;
    }
    
    void erase(const K &k) {
      m_map.erase(k);
    }
    
    
    void erase(Iterator it) {
      m_map.erase(it);
    }
    
    ///////// Lookup
    V &at(const K &k) {
      return m_map.at(k);
    }
    
    const V &at(const K &k) const {
      return m_map.at(k);
    }
    
    Iterator find(const K &k) {
      return m_map.find(k);
    }
    
    ConstIterator find(const K &k) const {
      return m_map.find(k);
    }
    
    V &operator[](const K &k) {
      return m_map[k];
    }
    
    
  private:
    base_map m_map;
    
    template<typename A, typename B>
    friend
    bool operator==(const StdMapWrapper<A,B>&, const StdMapWrapper<A,B>&);
    
    template<typename A, typename B>
    friend bool operator!=(const StdMapWrapper<A,B>&, const StdMapWrapper<A,B>&);
    template<typename A, typename B>
    friend bool operator<=(const StdMapWrapper<A,B>&, const StdMapWrapper<A,B>&);
    template<typename A, typename B>
    friend bool operator<(const StdMapWrapper<A,B>&, const StdMapWrapper<A,B>&);
    template<typename A, typename B>
    friend bool operator>=(const StdMapWrapper<A,B>&, const StdMapWrapper<A,B>&);
    template<typename A, typename B>
    friend bool operator>(const StdMapWrapper<A,B>&, const StdMapWrapper<A,B>&);
  };
  
  template<typename K, typename T>
  bool operator==(const StdMapWrapper<K,T> &a, const StdMapWrapper<K,T> &b) {
    return a.m_map == b.m_map;
  }
  
  template<typename K, typename T>
  bool operator!=(const StdMapWrapper<K,T> &a, const StdMapWrapper<K,T> &b) {
    return a.m_map != b.m_map;
  }
  
  template<typename K, typename T>
  bool operator<=(const StdMapWrapper<K,T> &a, const StdMapWrapper<K,T> &b) {
    return a.m_map <= b.m_map;
  }
  
  template<typename K, typename T>
  bool operator<(const StdMapWrapper<K,T> &a, const StdMapWrapper<K,T> &b) {
    return a.m_map < b.m_map;
  }
  
  template<typename K, typename T>
  bool operator>=(const StdMapWrapper<K,T> &a, const StdMapWrapper<K,T> &b) {
    return a.m_map >= b.m_map;
  }
  
  template<typename K, typename T>
  bool operator>(const StdMapWrapper<K,T> &a, const StdMapWrapper<K,T> &b) {
    return a.m_map > b.m_map;
  }
  
}

using Milli = std::chrono::duration<double, std::ratio<1,1000>>;
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

void dispTestName(const char *testName, const char *typeName) {
  std::cout << std::endl << std::endl << "********************************************************************************************" << std::endl;
  std::cout << "\t" << testName << " for " << typeName << "\t" << std::endl;
  std::cout << "********************************************************************************************" << std::endl << std::endl;
}

template <typename T>
T ascendingInsert(int count, bool print = true) {
  using namespace std::chrono;
  TimePoint start, end;
  start = system_clock::now();
  T map; 
  for(int i = 0; i < count; i++) {
    map.insert(std::pair<int, int>(i,i));
  }
  end = system_clock::now();
  
  Milli elapsed = end - start;
  
  if(print)
    std::cout << "Inserting " << count << " elements in aescending order took " << elapsed.count() << " milliseconds" << std::endl;
  
  return map;
}

// ------- My findTest ------- //
template <typename T>
void findTest(int count, bool del_element = false) {
  using namespace std::chrono;
  T m = ascendingInsert<T>(count,false);

  std::vector<int> toFind;
  for(int i = 0; i < count; i++) {
    toFind.push_back(i);
  }
  
  TimePoint start, end;
  if (!del_element) {
    start = system_clock::now();
    for(const int e : toFind) {
        m.find(e);
    }
    end = system_clock::now();
  } else {
    unsigned del_rand_elem = rand() % count;
    m.erase(del_rand_elem);
    std::cout << "Deleting random key: " << del_rand_elem << std::endl;
    start = system_clock::now();
    for(const int e : toFind) {
        m.find(e);
    }
    end = system_clock::now();
  }
  
  Milli elapsed = end - start;
  
  std::cout << "'Indexing' across " << count << " elements in a map of size " << count << " took " << elapsed.count() << " milliseconds time per iteration was " << elapsed.count()/double(count)*1e6 << " nanoseconds" << std::endl;
}

// ------- My indexibilityTest ------- //
template <typename T>
void indexibilityTest(unsigned count, bool del_element = false) {
  using namespace std::chrono;
  T m = ascendingInsert<T>(count,false);

  TimePoint start, end;

  if (!del_element) {
    start = system_clock::now();
    for(unsigned j = 0; j < count-1; j++) {
      m.indexable(j);
      // Sanity Check: To make sure the indexing is extracting the correct information
      //   auto pair = m.indexable(j);
      //   assert(pair.first == (int) j);
      //   assert(pair.second == (int) j);
    }
    end = system_clock::now();
  } else {
    unsigned del_rand_elem = rand() % count;
    m.erase(del_rand_elem);
    std::cout << "Deleting random key: " << del_rand_elem << std::endl;
    start = system_clock::now();
    for(unsigned j = 0; j < count-1; j++) {
        auto pair = m.indexable(j);
        //printf("iter: %u; pair.first: %i; pair.second: %i\n", j, pair.first, pair.second);
        if (j >= del_rand_elem) {
            assert(pair.first == (int) j + 1);
            assert(pair.second == (int) j + 1);
        } else {
            assert(pair.first == (int) j);
            assert(pair.second == (int) j);
        }
    }
    end = system_clock::now();
    count--;
  }

  
  Milli elapsed = end - start;
  
  std::cout << "Indexing across " << count << " elements in a map of size " << count << " took " << elapsed.count() << " milliseconds time per iteration was " << elapsed.count()/double(count)*1e6 << " nanoseconds" << std::endl;
}
// ------- My indexibilityTest ------- //

/*
  #include <assert.h>

  using namespace std;

  ostream &
  operator<<(ostream &os, const type_info &ti) {
  int ec;
  const char *demangled_name = abi::__cxa_demangle(ti.name(), 0, 0, &ec);
  assert(ec == 0);
  os << demangled_name;
  free((void *) demangled_name);
  return os;
  }

  template <typename T>
  void foo(T &&o) {
  //o = 2;
  cout << typeid(const int &) << endl;
  }

  int main() {
  const int i = 1;
  foo(i);
  }
*/

class comma_numpunct : public std::numpunct<char> {
protected:
  virtual char do_thousands_sep() const { return ','; }
  virtual std::string do_grouping() const { return "\03"; }
};



int main() {
  //separate all printed numbers with commas
  std::locale comma_locale(std::locale(), new comma_numpunct());
  std::cout.imbue(comma_locale);
  
  auto demangle = [](const std::type_info &ti) {
    int ec;
    return abi::__cxa_demangle(ti.name(), 0, 0, &ec);
    assert(ec == 0);
  };
  
  const char *w = demangle(typeid(myMap::StdMapWrapper<int,int>));
  const char *m = demangle(typeid(myMap::Map<int,int>));
  
  // Add your own indexibility scaling test here
  {
    // Test Indexibility scaling
    // Note: I used the same size as what was given for the iteration test
    dispTestName("Indexibility scaling test", m);
    indexibilityTest<myMap::Map<int,int>>(10000);
    indexibilityTest<myMap::Map<int,int>>(20000);
    indexibilityTest<myMap::Map<int,int>>(40000);
    indexibilityTest<myMap::Map<int,int>>(80000);
    indexibilityTest<myMap::Map<int,int>>(160000);
    indexibilityTest<myMap::Map<int,int>>(320000);
    indexibilityTest<myMap::Map<int,int>>(640000);
    indexibilityTest<myMap::Map<int,int>>(1280000);
    indexibilityTest<myMap::Map<int,int>>(2560000);
    indexibilityTest<myMap::Map<int,int>>(5120000);
  }

  std::cout << "\n\nNote: " << std::endl;
  std::cout << "\tNot exactly sure how to compare this index function against the actual std::map since there isn't such functionality in it." << std::endl;
  std::cout << "\tSo I am just going to test it finding each element one by one (almost the same thing)." << std::endl;

  // NOTE:
  // Not exactly sure how to compare this index function against the actual std::map since there isn't such functionality in it.
  // So I am just going to test it finding each element one by one (almost the same thing)
  {
    dispTestName("Indexibility scaling test (Find function)", w);
    findTest<myMap::Map<int,int>>(10000);
    findTest<myMap::Map<int,int>>(20000);
    findTest<myMap::Map<int,int>>(40000);
    findTest<myMap::Map<int,int>>(80000);
    findTest<myMap::Map<int,int>>(160000);
    findTest<myMap::Map<int,int>>(320000);
    findTest<myMap::Map<int,int>>(640000);
    findTest<myMap::Map<int,int>>(1280000);
    findTest<myMap::Map<int,int>>(2560000);
    findTest<myMap::Map<int,int>>(5120000);
  }


  std::cout << "\n\nNote: " << std::endl;
  std::cout << "\tThe following test will remove one random object within the map and then it will index through the rest of the Map. I've also added an assert to check the results." << std::endl;

  // Indexability with deletion of some objects
  {
    dispTestName("Indexibility w/ deletion of a random object scaling test", m);
    indexibilityTest<myMap::Map<int,int>>(10001, true);
    indexibilityTest<myMap::Map<int,int>>(20001, true);
    indexibilityTest<myMap::Map<int,int>>(40001, true);
    indexibilityTest<myMap::Map<int,int>>(80001, true);
    indexibilityTest<myMap::Map<int,int>>(160001, true);
    indexibilityTest<myMap::Map<int,int>>(320001, true);
    indexibilityTest<myMap::Map<int,int>>(640001, true);
    indexibilityTest<myMap::Map<int,int>>(1280001, true);
    indexibilityTest<myMap::Map<int,int>>(2560001, true);
    indexibilityTest<myMap::Map<int,int>>(5120001, true);
  }

  std::cout << "\n\nNote: " << std::endl;
  std::cout << "\tThe following test does not have asserts or checks of correctness (b/c it is the actual std::Map)" << std::endl;
  
  {
    dispTestName("Indexibility w/ deletiong of a random object scaling test (Find function)", w);
    findTest<myMap::Map<int,int>>(10001, true);
    findTest<myMap::Map<int,int>>(20001, true);
    findTest<myMap::Map<int,int>>(40001, true);
    findTest<myMap::Map<int,int>>(80001, true);
    findTest<myMap::Map<int,int>>(160001, true);
    findTest<myMap::Map<int,int>>(320001, true);
    findTest<myMap::Map<int,int>>(640001, true);
    findTest<myMap::Map<int,int>>(1280001, true);
    findTest<myMap::Map<int,int>>(2560001, true);
    findTest<myMap::Map<int,int>>(5120001, true);
  }
 
  // Cast, due to const-ness.
  free((void *) w);
  free((void *) m);
}
