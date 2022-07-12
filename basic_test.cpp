#include <iostream>
#include "Map.hpp"
#include <map>
#include <assert.h>

class A {
    public:
        A() = delete;
    private:
        double x;
        int array[100'000'000]; // Cache behavior?
};


class vehicle {
    public:
        int model_number;
        vehicle(int m_n) : model_number{m_n} {}
};

class bus : public vehicle {
    public:
        char type;
        bus(int m_n, char type) : vehicle{m_n}, type{type} {}
};

int main() {
    // Testing basic construction
    printf("----------------------------\n");
    printf("Testing basic construction\n");
    cs540::Map<int, int> m;
    printf("Size of m: %lu\n", m.size());
    assert(m.size() == 0);
    printf("Is m empty: %d\n", m.empty());
    assert(m.empty());
    printf("PASSED\n\n");

    // Test the insertion
    printf("----------------------------\n");
    printf("Basic Insertion\n");
    printf("Inserting {32, 123213}\n");
    std::pair<cs540::Map<int, int>::Iterator, bool> foo_insert_2 = m.insert(std::make_pair(32, 123213));
    printf("key: %d value: %d, and bool: %d \n", foo_insert_2.first->first, foo_insert_2.first->second, foo_insert_2.second);
    assert(foo_insert_2.second);
    assert(foo_insert_2.first->first == 32 && foo_insert_2.first->second == 123213);
    std::pair<cs540::Map<int, int>::Iterator, bool> foo_insert_3 = m.insert(std::make_pair(43, 143));
    printf("Inserting {43, 143}\n");
    printf("key: %d value: %d, and bool: %d \n", foo_insert_3.first->first, foo_insert_3.first->second, foo_insert_3.second);
    assert(foo_insert_3.second);
    assert(foo_insert_3.first->first == 43 && foo_insert_3.first->second == 143);
    printf("PASSED\n\n");

    // Testing the Skip List Output
    // m.print_skip_list();

    // Test the find function w/ more insertions
    printf("----------------------------\n");
    printf("Testing Large Insertion and Find (Iterator)\n");
    printf("Inserting 100000 elements and asserting the key and values\n");
    cs540::Map<int, int> large_m;
    const cs540::Map<int, int> c_large_m;
    for (int i = 0; i < 200000; i+=2) {
        large_m.insert(std::make_pair(i, i+1));

    }
    for (int i = 0; i < 200000; i+=2) {
        cs540::Map<int, int>::Iterator find_foo = large_m.find(i);
        assert(find_foo->first == i);
        assert(find_foo->second == i+1);
    }
    printf("PASSED\n\n");

    // Test the at function and [] operator
    printf("----------------------------\n");
    printf("Testing Large Insertion and At and []\n");
    for (int i = 0; i < 200000; i+=2) {
        assert(large_m.at(i) == i+1);
        assert(large_m[i] == i+1);
    }
    printf("PASSED\n\n");

    // Testing edge cases for find/at/[] operators
    printf("----------------------------\n");
    printf("Special Cases for find/at/[] operators\n");
    printf("Testing for key not found in find()\n");
    assert(large_m.find(-1) == large_m.end());
    assert(large_m.find(200001) == large_m.end());
    printf("PASSED\n\n");
    printf("Testing for keys not found in at()\n");
    try {
        large_m.at(-1);
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    try {
        large_m.at(200001);
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n\n");
    printf("Testing for [] that are not in the list\n");
    large_m[-1];
    large_m[200001];
    assert(large_m.at(-1) == 0);
    assert(large_m.at(200001) == 0);
    printf("PASSED\n\n");

    // Test the erase key_val operator
    printf("----------------------------\n");
    printf("Testing Erase() for Iterator and const arguments\n");
    size_t old_size = large_m.size();
    large_m.erase(-1);
    assert(old_size != large_m.size());
    old_size = large_m.size();
    cs540::Map<int, int>::Iterator foo_iter = large_m.find(200001);
    large_m.erase(foo_iter);
    assert(old_size != large_m.size());
    printf("Try erasing the same values just erased\n");
    try {
        large_m.erase(-1);
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
   try {
        large_m.erase(200001);
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n\n");


    // Testing clearing an empty map
    printf("----------------------------\n");
    cs540::Map<int, int> empty_map;
    // empty_map.print_skip_list();
    printf("Clearing empty Map\n");
    empty_map.clear();
    assert(empty_map.size() == 0);
    printf("PASSED\n\n");


    // Testing clearing a full map
    printf("----------------------------\n");
    printf("Clearing a full map\n");
    cs540::Map<int, int> full_map;
    full_map.insert(std::make_pair(1, 4));
    full_map.insert(std::make_pair(2, 5));
    full_map.insert(std::make_pair(3, 6));
    full_map.clear();
    large_m.clear();
    assert(full_map.size() == 0);
    assert(large_m.size() == 0);
    printf("PASSED\n\n");


    // Testing copy construction on 2 empty maps
    printf("----------------------------\n");
    printf("Testing the copy constructors \n");
    printf("Testing the copying of 2 empty maps\n");
    cs540::Map<int, int> empty_map2{empty_map};
    assert(empty_map2.size() == 0);
    assert(empty_map.size() == 0);
    printf("PASSED\n");
    printf("Testing copy construction a full map\n");
    for (int i = 0; i < 10000; i++) {
        large_m.insert(std::make_pair(i, i));
    }
    cs540::Map<int, int> copy_large_m{large_m};
    for (int i = 0; i < 10000; i++) {
        cs540::Map<int, int>::Iterator large_iter = large_m.find(i);
        cs540::Map<int, int>::Iterator c_large_iter = copy_large_m.find(i);
        assert(large_iter->first == c_large_iter->first);
        assert(large_iter->second == c_large_iter->second);
        assert(large_m[i] == copy_large_m[i]);
        assert(large_m.at(i) == copy_large_m.at(i));
    }
    printf("PASSED\n");
    printf("Out of bounds check\n");
    try{
        printf("Real Map [-1]: ");
        std::cout << large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    try{
        printf("Copy Real Map [-1]: ");
        std::cout << copy_large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n");
    printf("Insert into the copy {-1, -1}, but not in Real Map \n");
    copy_large_m.insert(std::make_pair(-1, -1));
    try{
        printf("Real Map [-1]: ");
        std::cout << large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    try{
        printf("Copy Real Map [-1]: ");
        std::cout << copy_large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n\n");


    // Testing the copy assignment operator
    printf("----------------------------\n");
    printf("Testing the copy assignment operator\n");
    printf("Testing the copying of 2 empty maps\n");
    cs540::Map<int, int> copy_assignment_empty_map = empty_map;
    assert(copy_assignment_empty_map.size() == 0);
    assert(empty_map.size() == 0);
    printf("PASSED\n");
    printf("Testing the copy assignment operator self assignment of empty maps\n");
    copy_assignment_empty_map = copy_assignment_empty_map;
    assert(copy_assignment_empty_map.size() == 0);
    printf("PASSED\n");
    large_m.clear();
    printf("Testing copy construction a full map\n");
    for (int i = 0; i < 10000; i++) {
        large_m.insert(std::make_pair(i, i));
    }
    cs540::Map<int, int> copy_assignment_large_m = large_m;
    for (int i = 0; i < 10000; i++) {
        cs540::Map<int, int>::Iterator large_iter = large_m.find(i);
        cs540::Map<int, int>::Iterator c_large_iter = copy_assignment_large_m.find(i);
        assert(large_iter->first == c_large_iter->first);
        assert(large_iter->second == c_large_iter->second);
        assert(large_m[i] == copy_assignment_large_m[i]);
        assert(large_m.at(i) == copy_assignment_large_m.at(i));
    }
    printf("PASSED\n");
    printf("Testing the copy assignment operator self assignment of empty maps\n");
    copy_assignment_large_m = copy_assignment_large_m;
    printf("PASSED\n");
    printf("Out of bounds check\n");
    try{
        printf("Real Map [-1]: ");
        std::cout << large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    try{
        printf("Copy Real Map [-1]: ");
        std::cout << copy_assignment_large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n");
    printf("Insert into the copy {-1, -1}, but not in Real Map \n");
    copy_assignment_large_m.insert(std::make_pair(-1, -1));
    try{
        printf("Real Map [-1]: ");
        std::cout << large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    try{
        printf("Copy Real Map [-1]: ");
        std::cout << copy_assignment_large_m.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n\n");


    // Testing the Initializer_list
    printf("----------------------------\n");
    printf("Testing the Initializer_list\n");
    printf("Empty Initializer_list\n");
    cs540::Map<int, int> empty_init_list{};
    assert(empty_init_list.size() == 0);
    printf("PASSED\n");
    printf("Testing Initializer_list with Map<int, int>: {{1, 1}, {2, 2}, {3, 3}, {4, 4}}\n");
    cs540::Map<int, int> init_list_map {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    for (int i = 1; i < 5; i++) {
        cs540::Map<int, int>::Iterator init_list_iter = init_list_map.find(i);
        assert(init_list_map[i] == i);
        assert(init_list_map.at(i) == i);
        assert(init_list_iter->first == i);
        assert(init_list_iter->second == i);
    }
    printf("PASSED\n");
    printf("Out of bounds check\n");
    try{
        printf("Initializer_list Map [-1]: ");
        std::cout << init_list_map.at(-1) << std::endl;
    } catch (std::out_of_range & e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n");
    printf("Insert {-1, -1} into Initializer_list Map, but not in Real Map \n");
    init_list_map.insert(std::make_pair(-1, -1));
    cs540::Map<int, int>::Iterator init_list_iter = init_list_map.find(-1);
    assert(init_list_map[-1] == -1);
    assert(init_list_map.at(-1) == -1);
    assert(init_list_iter->first == -1);
    assert(init_list_iter->second == -1);
    printf("PASSED\n\n");


    // Const iterator
    printf("----------------------------\n");
    printf("Const Iterator for Begin(), End(), find(), and at()\n");
    printf("Inserting {{1, 1.0}, {3, 5.67}, {13, 6.9}} into the map\n");
    const cs540::Map<int, double> int_double_map_const {{1, 1.0}, {3, 5.67}, {13, 6.9}};
    std::vector<int> temp_keys = {1, 3, 13};
    std::vector<double> temp_vals = {1.0, 5.67, 6.9};
    int counter = 0;
    for (auto i = int_double_map_const.begin(); i != int_double_map_const.end(); i++) {
        assert(i->first == temp_keys[counter]);
        assert(i->second == temp_vals[counter]);
        counter++;
    }
    printf("PASSED\n");

    printf("Testing Find() with ConstIterator\n");
    const cs540::Map<int, double>::ConstIterator find_const_iter = int_double_map_const.find(1);
    std::cout << "find_const_iter (1): key: " << find_const_iter->first << " value: " << find_const_iter->second << std::endl;
    const cs540::Map<int, double>::ConstIterator find_const_iter2 = int_double_map_const.find(3);
    std::cout << "find_const_iter (3): key: " << find_const_iter2->first << " value: " << find_const_iter2->second << std::endl;
    const cs540::Map<int, double>::ConstIterator find_const_iter3 = int_double_map_const.find(13);
    std::cout << "find_const_iter (13): key: " << find_const_iter3->first << " value: " << find_const_iter3->second << std::endl;
    printf("PASSED\n");

    printf("Testing At() with ConstIterator\n");
    std::cout << "find_const_iter.at(1): val: " << int_double_map_const.at(1) << std::endl;
    std::cout << "find_const_iter.at(3): val: " << int_double_map_const.at(3) << std::endl;
    std::cout << "find_const_iter.at(13): val: " << int_double_map_const.at(13) << std::endl;
    printf("PASSED\n");

    printf("Testing Out Of Range At(41) with ConstIterator\n");
    try {
        int_double_map_const.at(41);
    } catch (std::out_of_range &e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n\n");


    // Test Iterator Insertion
    printf("----------------------------\n");
    printf("Test Iterator Insertion\n");
    std::vector<std::pair<int, int>> int_int_map_pairs = {std::make_pair(1,2), std::make_pair(2,3), std::make_pair(4,5)};
    cs540::Map<int, int> int_int_map;
    int_int_map.insert(int_int_map_pairs.begin(), int_int_map_pairs.end());
    printf("Inserting a vector of pair ints: {{1,2}, {2, 3}, {4, 5}}\n");
    printf("Test .at(1): %d\n", int_int_map.at(1));
    printf("Test .at(2): %d\n", int_int_map.at(2));
    printf("Test .at(4): %d\n", int_int_map.at(4));
    printf("PASSED\n\n");

    // Comparision Operators
    printf("----------------------------\n");
    printf("Test the Comparision Operations For Maps\n");
    printf("Equality\n");
    cs540::Map<int, int> int_int_map1;
    cs540::Map<int, int> int_int_map2;
    printf("Comparing 2 empty maps (print out 1): %d\n", int_int_map1 == int_int_map2);
    assert(int_int_map1 == int_int_map2);
    printf("PASSED\n");
    
    printf("Inserting 100000 of the same keys and values to 2 different maps");
    // cs540::Map<int, int> int_int_map1;
    // cs540::Map<int, int> int_int_map2;
    for (int i = 0; i < 100000; i++) {
        int_int_map1.insert(std::make_pair(i, i));
        int_int_map2.insert(std::make_pair(i, i));
    }
    printf("Comparing 2 same value maps (print out 1): %d\n", int_int_map1 == int_int_map2);
    assert(int_int_map1 == int_int_map2);
    printf("PASSED\n");
    
    printf("Inserting {-1, -1} to only one map");
    int_int_map1.insert(std::make_pair(-1, -1));
    printf("Comparing 2 different sized maps (print out 0): %d\n", int_int_map1 == int_int_map2);
    assert(!(int_int_map2 == int_int_map1));
    printf("PASSED\n\n");

    printf("Not Equality Operator\n");
    printf("Comparing 2 maps of different size (print out 1): %d\n", int_int_map1 != int_int_map2);
    printf("PASSED\n");

    int_int_map2.insert(std::make_pair(2,2));
    printf("Comparing 2 maps of same size/value different key (print out 1): %d\n", int_int_map1 != int_int_map2);
    assert(int_int_map2 != int_int_map1);
    printf("PASSED\n");

    int_int_map2.clear();
    int_int_map2.insert(std::make_pair(1,3));
    printf("Comparing 2 maps of same size/key different value (print out 1): %d\n", int_int_map1 != int_int_map2);
    assert(int_int_map2 != int_int_map1);
    printf("PASSED\n\n");

    printf("Less than Operator\n");
    int_int_map1.clear();
    printf("Comparing 2 maps empty map < fill map (print out 1): %d\n", int_int_map1 < int_int_map2);
    assert(int_int_map1 < int_int_map2);
    printf("PASSED\n");

    int_int_map1.insert(std::make_pair(0, 3));
    printf("Comparing 2 maps map1.key < map2.key (same value) (print out 1): %d\n", int_int_map1 < int_int_map2);
    assert(int_int_map1 < int_int_map2);
    printf("PASSED\n");

    int_int_map1.clear();
    int_int_map1.insert(std::make_pair(0,-1));
    printf("Comparing 2 maps map1.value < map2.value (same key) (print out 1): %d\n", int_int_map1 < int_int_map2);
    assert(int_int_map1 < int_int_map2);
    printf("PASSED\n");

    int_int_map2.clear();
    int_int_map2.insert(std::make_pair(0,-1));
    int_int_map2.insert(std::make_pair(3,2));
    printf("Comparing 2 maps same content but map1.size < map2.size (print out 1): %d\n", int_int_map1 < int_int_map2);
    assert(int_int_map1 < int_int_map2);
    printf("PASSED\n\n");

    // Test Iterator Methods
    printf("----------------------------\n");
    cs540::Map<int, int>::Iterator begin_iter_int_int_map = int_int_map.begin();
    cs540::Map<int, int>::Iterator end_iter_int_int_map = int_int_map.end();
    end_iter_int_int_map--;
    printf("Test Iterator operations\n");
    printf("Inserting a of pair ints: {{1,2}, {2, 3}, {4, 5}}\n");
    printf("Test begin(): key: %d, value: %d \n", begin_iter_int_int_map->first, begin_iter_int_int_map->second);
    printf("Test element before end(): key: %d, value: %d \n", end_iter_int_int_map->first, end_iter_int_int_map->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    auto begin_preincrement = ++begin_iter_int_int_map;
    begin_iter_int_int_map--;
    auto begin_postincrement = begin_iter_int_int_map++;
    printf("Test begin() preincrement: key: %d, value: %d \n", begin_preincrement->first, begin_preincrement->second);
    printf("Test begin() postincrement: key: %d, value: %d \n", begin_postincrement->first, begin_postincrement->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    auto end_predecrement = --end_iter_int_int_map;
    end_iter_int_int_map++;
    auto end_postdecrement = end_iter_int_int_map--;
    printf("Test end() predecrement: key: %d, value: %d \n", end_predecrement->first, end_predecrement->second);
    printf("Test end() postdecrement: key: %d, value: %d \n", end_postdecrement->first, end_postdecrement->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the * operation\n");
    printf("Changing the begin() iter's Mapped_T value to 31\n");
    begin_iter_int_int_map--;
    (*begin_iter_int_int_map).second = 31;
    printf("Checking the begin() iter's Mapped_T value: key: %d, value: %d\n", begin_iter_int_int_map->first, begin_iter_int_int_map->second);
    auto another_begin_iter = int_int_map.begin();
    printf("Checking the begin() iter's Mapped_T value with another a new iter begin(): key: %d, value: %d\n", another_begin_iter->first, another_begin_iter->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the implicit assignment operator\n");
    auto test_implicit_assignment = another_begin_iter;
    printf("Iter should be at the beginning of Map: key: %d, val: %d\n", test_implicit_assignment->first, test_implicit_assignment->second);
    test_implicit_assignment = --int_int_map.end();
    printf("Iter should be at the end of Map: key: %d, val: %d\n", test_implicit_assignment->first, test_implicit_assignment->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the Copy Constructor assignment operator\n");
    test_implicit_assignment = another_begin_iter;
    another_begin_iter++;
    printf("Assigning an iter to the begin() of map and incrementing the assigning iter\n");
    printf("New iter should still be pointing at the begin() of Map: key: %d, val: %d\n", test_implicit_assignment->first, test_implicit_assignment->second);
    printf("PASSED\n\n");

    // Test ConstIterator Methods
    printf("----------------------------\n");
    const cs540::Map<int, int> const_int_int{{10, 11}, {12, 14}, {41, 51}};
    cs540::Map<int, int>::ConstIterator begin_iter_const_map = const_int_int.begin();
    cs540::Map<int, int>::ConstIterator end_iter_const_map = const_int_int.end();
    end_iter_const_map--;
    printf("Test ConstIterator operations\n");
    printf("Inserting a of pair ints: {{10, 11}, {12, 14}, {41, 51}}\n");
    printf("Test begin(): key: %d, value: %d \n", begin_iter_const_map->first, begin_iter_const_map->second);
    printf("Test element before end(): key: %d, value: %d \n", end_iter_const_map->first, end_iter_const_map->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    auto begin_iter_const_preincrement = ++begin_iter_const_map;
    begin_iter_const_map--;
    auto begin_iter_const_postincrement = begin_iter_const_map++;
    printf("Test begin() preincrement: key: %d, value: %d \n", begin_iter_const_preincrement->first, begin_iter_const_preincrement->second);
    printf("Test begin() postincrement: key: %d, value: %d \n", begin_iter_const_postincrement->first, begin_iter_const_postincrement->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    auto end_iter_const_predecrement = --end_iter_const_map;
    end_iter_const_map++;
    auto end_iter_const_postdecrement = end_iter_const_map--;
    printf("Test end() predecrement: key: %d, value: %d \n", end_iter_const_predecrement->first, end_iter_const_predecrement->second);
    printf("Test end() postdecrement: key: %d, value: %d \n", end_iter_const_postdecrement->first, end_iter_const_postdecrement->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the Iterator Conversion operation\n");
    printf("Inserting a of pair ints: {{1,2}}\n");
    printf("Assigning Iterator to begin(), and passing this iterator into ConstIterator\n");
    cs540::Map<int, int> const_conversion_op_map {{1,2}};
    const cs540::Map<int, int>::Iterator converision_iter = const_conversion_op_map.begin();
    cs540::Map<int, int>::ConstIterator const_convert_iter(converision_iter);
    printf("Testing accessing of data: key: %d, val: %d\n", const_convert_iter->first, const_convert_iter->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the * operation\n");
    printf("Taking the * of the begin() iter, and storing the reference\n");
    begin_iter_const_map--;
    auto ref_to_begin_data = (*begin_iter_const_map);
    printf("Checking the begin() iter's Mapped_T value: key: %d, value: %d\n", ref_to_begin_data.first, ref_to_begin_data.second);
    auto another_begin_const_iter = const_int_int.begin();
    printf("Checking the begin() iter's Mapped_T value with another a new iter begin(): key: %d, value: %d\n", another_begin_const_iter->first, another_begin_const_iter->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the implicit assignment operator\n");
    auto const_test_implicit_assignment = another_begin_const_iter;
    printf("Iter should be at the beginning of Map: key: %d, val: %d\n", const_test_implicit_assignment->first, const_test_implicit_assignment->second);
    const_test_implicit_assignment = --const_int_int.end();
    printf("Iter should be at the end of Map: key: %d, val: %d\n", const_test_implicit_assignment->first, const_test_implicit_assignment->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the Copy Constructor assignment operator\n");
    const_test_implicit_assignment = another_begin_const_iter;
    another_begin_const_iter++;
    printf("Assigning an iter to the begin() of map and incrementing the assigning iter\n");
    printf("New iter should still be pointing at the begin() of Map: key: %d, val: %d\n", const_test_implicit_assignment->first, const_test_implicit_assignment->second);
    printf("PASSED\n\n");

    // Test ReverseIterator Methods
    printf("----------------------------\n");
    cs540::Map<int, int> r_int_int_map {{1,2}, {2, 3}, {4, 5}};
    cs540::Map<int, int>::ReverseIterator r_begin_iter_int_int_map = r_int_int_map.rbegin();
    cs540::Map<int, int>::ReverseIterator r_end_iter_int_int_map = r_int_int_map.rend();
    r_begin_iter_int_int_map++;
    printf("Test ReverseIterator operations: \n");
    printf("Inserting a of pair ints: {{1,2}, {2, 3}, {4, 5}}\n");
    printf("Test rbegin() (should print the last element): key: %d, value: %d \n", r_begin_iter_int_int_map->first, r_begin_iter_int_int_map->second);
    printf("Test element before rend()(should print the first element): key: %d, value: %d \n", r_end_iter_int_int_map->first, r_end_iter_int_int_map->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    auto r_begin_preincrement = ++r_begin_iter_int_int_map;
    r_begin_iter_int_int_map--;
    auto r_begin_postincrement = r_begin_iter_int_int_map++;
    printf("Test rbegin() preincrement: key: %d, value: %d \n", r_begin_preincrement->first, r_begin_preincrement->second);
    printf("Test rbegin() postincrement: key: %d, value: %d \n", r_begin_postincrement->first, r_begin_postincrement->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    auto r_end_predecrement = --r_end_iter_int_int_map;
    r_end_iter_int_int_map++;
    auto r_end_postdecrement = r_end_iter_int_int_map--;
    printf("Test rend() predecrement: key: %d, value: %d \n", r_end_predecrement->first, r_end_predecrement->second);
    printf("Test rend() postdecrement: key: %d, value: %d \n", r_end_postdecrement->first, r_end_postdecrement->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the * operation\n");
    printf("Changing the rbegin() iter's Mapped_T value to 31\n");
    r_begin_iter_int_int_map--;
    (*r_begin_iter_int_int_map).second = 31;
    printf("Checking the rbegin() iter's Mapped_T value: key: %d, value: %d\n", r_begin_iter_int_int_map->first, r_begin_iter_int_int_map->second);
    auto r_another_begin_iter = ++r_int_int_map.rbegin();
    printf("Checking the rbegin() iter's Mapped_T value with another a new iter rbegin(): key: %d, value: %d\n", r_another_begin_iter->first, r_another_begin_iter->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the implicit assignment operator\n");
    auto r_test_implicit_assignment = r_another_begin_iter;
    printf("Iter should be at the beginning of Map: key: %d, val: %d\n", r_test_implicit_assignment->first, r_test_implicit_assignment->second);
    r_test_implicit_assignment = r_int_int_map.rend();
    printf("Iter should be at the end of Map: key: %d, val: %d\n", r_test_implicit_assignment->first, r_test_implicit_assignment->second);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Testing the Copy Constructor assignment operator\n");
    r_test_implicit_assignment = r_another_begin_iter;
    another_begin_iter++;
    printf("Assigning an iter to the rbegin() of map and incrementing the assigning iter\n");
    printf("New iter should still be pointing at the rbegin() of Map: key: %d, val: %d\n", r_test_implicit_assignment->first, r_test_implicit_assignment->second);
    printf("PASSED\n\n");

    // Test Comparision Operator for Iterators
    printf("----------------------------\n");
    cs540::Map<int, int> identical_map_1 {{1,2}, {4,1}, {241, 42}};
    cs540::Map<int, int> identical_map_2 {{1,2}, {4,1}, {241, 42}};
    printf("Inserting a of pair ints: {{1,2}, {4,1}, {241, 42}} to 2 different maps\n");
    cs540::Map<int, int>::Iterator identical_map_iter1 = identical_map_1.begin();
    cs540::Map<int, int>::Iterator identical_map_iter2 = identical_map_2.begin();
    cs540::Map<int, int>::ConstIterator c_identical_map_iter1 = identical_map_1.begin();
    cs540::Map<int, int>::ConstIterator c_identical_map_iter2 = identical_map_2.begin();
    cs540::Map<int, int>::ReverseIterator r_identical_map_iter1 = identical_map_1.rbegin();
    cs540::Map<int, int>::ReverseIterator r_identical_map_iter2 = identical_map_2.rbegin();
    printf("Test == between different maps of the same iterator types\n");
    printf("Iterators: the same data values: should return 0: %d\n", identical_map_iter1 == identical_map_iter2);
    printf("ConstIterator: the same data values: should return 0: %d\n", c_identical_map_iter2 == c_identical_map_iter1);
    printf("ReverseIterator: the same data values: should return 0: %d\n", r_identical_map_iter1 == r_identical_map_iter2);
    printf("Test == between same map pointing to the same object in the Map\n");
    printf("Iterators: the same data values: should return 1: %d\n", identical_map_iter1 == identical_map_1.begin());
    printf("ConstIterator: the same data values: should return 1: %d\n", c_identical_map_iter1 == identical_map_1.begin());
    printf("ReverseIterator: the same data values: should return 1: %d\n", r_identical_map_iter1 == identical_map_1.rbegin());
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Test != between different maps of the same iterator types\n");
    printf("Iterators: the same data values: should return 1: %d\n", identical_map_iter1 != identical_map_iter2);
    printf("ConstIterator: the same data values: should return 1: %d\n", c_identical_map_iter2 != c_identical_map_iter1);
    printf("ReverseIterator: the same data values: should return 1: %d\n", r_identical_map_iter1 != r_identical_map_iter2);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Test == between 2 different iterator (ConstIterator & Iterator) types pointing to the same object in the same map \n");
    printf("ConstIterator & Iterator: should return 1: %d\n", identical_map_iter1 == c_identical_map_iter1);
    printf("Iterator & ConstIterator: should return 1: %d\n", c_identical_map_iter1 == identical_map_iter1);
    printf("Test != between 2 different iterator types pointing to the same object in the same map \n");
    printf("ConstIterator & Iterator: should return 0: %d\n", identical_map_iter1 != c_identical_map_iter1);
    printf("Iterator & ConstIterator: should return 0: %d\n", c_identical_map_iter1 != identical_map_iter1);
    printf("PASSED\n");
    printf("----------------------------\n");
    printf("Test == between 2 different iterator (ConstIterator & Iterator) types pointing to different map \n");
    printf("ConstIterator & Iterator: should return 0: %d\n", identical_map_iter1 == c_identical_map_iter2);
    printf("Iterator & ConstIterator: should return 0: %d\n", c_identical_map_iter1 == identical_map_iter2);
    printf("Test != between 2 different iterator (ConstIterator & Iterator) types pointing to different map \n");    
    printf("ConstIterator & Iterator: should return 1: %d\n", identical_map_iter1 != c_identical_map_iter2);
    printf("Iterator & ConstIterator: should return 1: %d\n", c_identical_map_iter1 != identical_map_iter2);
    printf("PASSED\n\n");

    // Test Indexable Funciton (Extra Credit)
    printf("----------------------------\n");
    printf("Extra Credit Function: Testing Indexable function\n");
    printf("Inserting a of pair ints: {{1,2}, {4,1}, {241, 42}}\n");
    cs540::Map<int, int> indexable_map_1 {{1,2}, {4,1}, {241, 42}};
    auto indexable_map_1_index0 = indexable_map_1.indexable(0);
    auto indexable_map_1_index1 = indexable_map_1.indexable(1);
    auto indexable_map_1_index2 = indexable_map_1.indexable(2);
    printf("Testing index 0: key: %d, value: %d\n", indexable_map_1_index0.first, indexable_map_1_index0.second);
    printf("Testing index 1: key: %d, value: %d\n", indexable_map_1_index1.first, indexable_map_1_index1.second);
    printf("Testing index 2: key: %d, value: %d\n", indexable_map_1_index2.first, indexable_map_1_index2.second);
    try {
        printf("Testing index 3 (Out of Range)\n");
        indexable_map_1.indexable(3);
    } catch (std::out_of_range &e) {
        std::cout << e.what() << std::endl;
    }
    printf("PASSED\n\n");
}