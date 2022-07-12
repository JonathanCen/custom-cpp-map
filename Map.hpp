#ifndef MAP_HPP
#define MAP_HPP
#include <utility>
#include <iostream>
#include <vector>

namespace myMap {

    template <typename Key_T, typename Mapped_T> class Map;
    template <typename Key_T, typename Mapped_T> bool map_equality(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);
    template <typename Key_T, typename Mapped_T> bool less_than_helper(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);
    template <typename Key_T, typename Mapped_T> bool operator==(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);
    template <typename Key_T, typename Mapped_T> bool operator!=(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);
    template <typename Key_T, typename Mapped_T> bool operator<(const Map<Key_T, Mapped_T> &, const Map<Key_T, Mapped_T> &);

    // Type members: std::pair<const Key_T, Mapped_T>;
    template <typename Key_T, typename Mapped_T>
    class Map {
        public:
            class Iterator;
            class ConstIterator;
            class ReverseIterator;
            typedef std::pair<const Key_T, Mapped_T> ValueType;
                    friend bool map_equality<Key_T, Mapped_T>(const Map&, const Map &);
                    friend bool less_than_helper<Key_T, Mapped_T>(const Map&, const Map &);
        private:
            friend class Iterator;
            class Skip_List;
            /*
            Notes:
                - Sentinal Node or the head of the List (such that this head will never be removed)
                - Have L0 to be a doubly circular linked list
                - Use a base node class - for data class and sentienal class
                - Vector of possible transitions
            */
/*-------------------------- Nested Private Red-Black Tree & Node Classes --------------------------*/
            // Private parent node class
            class Skip_List_Node {
                private:

                    template <typename Key, typename Mapped> friend class Map;
                    friend class Map::Skip_List;
                    friend class Map::Iterator;
                    friend bool map_equality<Key_T, Mapped_T>(const Map&, const Map &);
                    friend bool less_than_helper<Key_T, Mapped_T>(const Map&, const Map &);

                    // Data members
                    Skip_List_Node *prev, *next;
                    // Clean the code up by merging both the vectors together into one
                    // List of nodes this current node can jump to
                    std::vector<Skip_List_Node *> vec_list_node;
                    // List of indices that this will jump if taken the corressponding express lanes
                    std::vector<unsigned> vect_list_node_jump_indices;

                    // helper functions
                    Mapped_T& get_mapped_val() {
                        return static_cast<Skip_List_Data_Node *> (this)->data.second;
                    }
                    const Key_T& get_key_val() {
                        return static_cast<Skip_List_Data_Node *> (this)->data.first;
                    }
                    // Constructor
                    Skip_List_Node(Skip_List_Node *p, Skip_List_Node *n) : prev{p}, next{n} {}
            };

            // Private child data node class
            class Skip_List_Data_Node : public Skip_List_Node {
                template <typename Key, typename Mapped> friend class Map;
                friend class Map::Skip_List;
                friend class Map::Iterator; // Doesn't work?
                friend bool map_equality<Key_T, Mapped_T>(const Map&, const Map &);
                friend bool less_than_helper<Key_T, Mapped_T>(const Map&, const Map &);

                std::pair<const Key_T, Mapped_T> data;
                Skip_List_Data_Node(Skip_List_Node *p, Skip_List_Node *n, std::pair<const Key_T, Mapped_T> val) : Skip_List_Node{p, n}, data{std::make_pair(val.first, val.second)} {}
            };

            class Skip_List {
                private:
                    template <typename Key, typename Mapped> friend class Map;
                    friend class Map::Iterator;  // Doesn't work?
                    friend bool map_equality<Key_T, Mapped_T>(const Map&, const Map &);
                    friend bool less_than_helper<Key_T, Mapped_T>(const Map&, const Map &);

                    // Data Members
                    Skip_List_Node sentinal_node;
                    size_t size;

                    // Skip List Methods (Creating, Updating, Reading, Deleting)
                    // O(lg(N)) for key lookup, insertion, and deletion
                    // O(N) for copy construction and assignment
                    // O(1) for all iterator increments and decrements
                    std::pair<Iterator, bool> insert(const ValueType &);
                    void erase_iter(Iterator);
                    void erase_val(const Key_T &);
                    void clear();

                    Iterator find(const Key_T &);
                    ConstIterator find_const_iter(const Key_T &) const;
                    Mapped_T &at(const Key_T &);
                    const Mapped_T &at_const(const Key_T &) const;
                    Mapped_T &operator[](const Key_T &);

                    // Constructor Only able to be called by Map Class
                    Skip_List() : sentinal_node{&sentinal_node, &sentinal_node}, size(0) {
                        sentinal_node.vec_list_node.push_back(&sentinal_node); // The top most level will point back to itself
                        sentinal_node.vect_list_node_jump_indices.push_back(1);
                    }

                    // Destructor not sure if it is enough
                    ~Skip_List() { clear(); }

                    // Debugging purposes only works for Basic Data Types
                    void print_skip_list() {
                        Map::Skip_List_Node * walking_ptr = &sentinal_node;
                        printf("Sentinal Node: height: %lu\n", sentinal_node.vec_list_node.size());
                        std::cout << " The addresss: " << static_cast<Skip_List_Data_Node *> (walking_ptr)->data.second << std::endl;
                        for (unsigned j = 0; j < walking_ptr->vec_list_node.size(); j++) {
                            std::cout << "Sentinal: " << " L" << j << ": keys:"<< walking_ptr->vec_list_node[j]->get_key_val() << " dist:" << walking_ptr->vect_list_node_jump_indices[j] << std::endl;
                        }
                        for (unsigned i = 0; i < size; i++) {
                            walking_ptr = walking_ptr->next;
                            printf("Node #%d: height: %lu\n", i, walking_ptr->vec_list_node.size());
                            for (unsigned j = 0; j < walking_ptr->vec_list_node.size(); j++) {
                                std::cout << "Node #" << i << ": L" << j << ": keys:"<< walking_ptr->vec_list_node[j]->get_key_val() << " dist:" << walking_ptr->vect_list_node_jump_indices[j] << std::endl;
                            }
                            std::cout << "Node #" << i << ": key: " << walking_ptr->get_key_val() << " ; value: "  << walking_ptr->get_mapped_val() << "\n";   
                        }

                    }

            };

            // "Map" object
            Map<Key_T, Mapped_T>::Skip_List skip_list_map;

            // Helper function for copy constructor/assignment
            void deep_copy(const Map &);

        public:
/*-------------------------- Nested Public? Iterator Classes --------------------------*/
            // Public Iterator Class
            class Iterator {
                public:
                    template <typename Key, typename Mapped> friend class Map;
                    friend class Map::Skip_List;
                    Iterator() = delete;
                    Iterator &operator++() {
                        if (iter == &list_obj->sentinal_node) {return *this;}
                        pos += 1;
                        iter = iter->next;
                        return *this;
                    }
                    Iterator operator++(int) { Iterator curr = *this; ++(*this); return curr; }
                    Iterator &operator--() {
                        if (pos == 0) {return *this;}
                        pos -= 1;
                        iter = iter->prev;
                        return *this;
                    }
                    Iterator operator--(int) { Iterator curr = *this; --(*this); return curr; }
                    ValueType &operator*() const { return get_reference_data(); };
                    ValueType *operator->() const { return get_data();};

                    bool operator== (const Iterator & iter2) {return this->iter == iter2.iter;};
                    bool operator!= (const Iterator & iter2) {return this->iter != iter2.iter;};
                    bool operator== (const ConstIterator & c_iter2) {return this->iter == c_iter2.iter;};
                    bool operator!= (const ConstIterator & c_iter2) {return this->iter != c_iter2.iter;};
                private:
                    // Constructors
                    // Called when inserting an object
                    Iterator(Skip_List * list, Skip_List_Node * iter, int pos) : list_obj{list}, iter{iter}, pos{pos} {}
                    Iterator(Skip_List * list, int pos) : list_obj{list}, iter{pos == 0 ? list->sentinal_node.next : &list->sentinal_node}, pos{pos} {} 
                    // Called by the iterator functions within the map API
                    Iterator(Map<Key_T, Mapped_T> & map, int pos) : list_obj{&map.skip_list_map}, iter{pos == 0 ? map.skip_list_map.sentinal_node.next : &map.skip_list_map.sentinal_node}, pos{pos} {} 
                    // Data Members
                    Skip_List * list_obj; 
                    Skip_List_Node * iter;  
                    int pos;
                    // Helper function to extract the data from the iter
                    ValueType & get_reference_data() const { return static_cast<Skip_List_Data_Node*>(iter)->data; }
                    ValueType * get_data() const { return &static_cast<Skip_List_Data_Node*>(iter)->data; }
            };

            // Public ConstIterator Class
            class ConstIterator {
                public:
                    template <typename Key, typename Mapped> friend class Map;
                    friend class Map::Skip_List;
                    ConstIterator() = delete;
                    // Test if these implicit ones work
                    ConstIterator(const Iterator & c_iter) {
                        list_obj = c_iter.list_obj;
                        iter = c_iter.iter;
                        pos = c_iter.pos;
                    };
                    ConstIterator &operator++() {
                        if (iter == &list_obj->sentinal_node) {return *this;}
                        pos += 1;
                        iter = iter->next;
                        return *this;
                    }
                    ConstIterator operator++(int) { ConstIterator curr = *this; ++(*this); return curr; }
                    ConstIterator &operator--() {
                        if (pos == 0) {return *this;}
                        pos -= 1;
                        iter = iter->prev;
                        return *this;
                    }
                    ConstIterator operator--(int)  { ConstIterator curr = *this; --(*this); return curr; }
                    const ValueType &operator*() const { return get_reference_data(); };
                    const ValueType *operator->() const { return get_data(); }

                    bool operator== (const ConstIterator & c_iter2) {return this->iter == c_iter2.iter;};
                    bool operator!= (const ConstIterator & c_iter2) {return this->iter != c_iter2.iter;};
                    bool operator== (const Iterator & iter2) {return this->iter == iter2.iter;};
                    bool operator!= (const Iterator & iter2) {return this->iter != iter2.iter;};
                private:
                    friend class Map::Skip_List;
                    // Constructors
                    // Called when inserting an object
                    ConstIterator(const Skip_List * list, Skip_List_Node * iter, int pos) : list_obj{list}, iter{iter}, pos{pos} {}
                    ConstIterator(const Skip_List * list, int pos) : list_obj{list}, iter{pos == 0 ? list->sentinal_node.next : &list->sentinal_node}, pos{pos} {} 
                    // Called by the iterator functions within the map API
                    ConstIterator(const Map<Key_T, Mapped_T> * map, int pos) : list_obj{&map->skip_list_map}, iter{pos == 0 ? map->skip_list_map.sentinal_node.next : &map->skip_list_map.sentinal_node}, pos{pos} {} 
                    // Data Members
                    const Skip_List * list_obj; 
                    const Skip_List_Node * iter;  
                    int pos;

                    // Helper function to extract the data from the iter
                    const ValueType & get_reference_data() const { 
                        Skip_List_Node * temp_iter = const_cast<Skip_List_Node *>(iter); 
                        return const_cast<ValueType &>(static_cast<Skip_List_Data_Node*>(temp_iter)->data);
                    }
                    const ValueType * get_data() const { 
                        Skip_List_Node * temp_iter = const_cast<Skip_List_Node *>(iter); 
                        return const_cast<ValueType *>(&static_cast<Skip_List_Data_Node*>(temp_iter)->data);
                    }
            };

            // Exactly the same as the Iterator class maybe use inheritence.
            // Public ReverseIterator Class
            class ReverseIterator {
                public:
                    template <typename Key, typename Mapped> friend class Map;
                    ReverseIterator() = delete;
                    // Check if these work
                    ReverseIterator &operator++() {
                        pos -= 1;
                        iter = iter->prev;
                        return *this;
                    };
                    ReverseIterator operator++(int) { ReverseIterator curr = *this; ++(*this); return curr; }
                    ReverseIterator &operator--() {
                        pos += 1;
                        iter = iter->next;
                        return *this;
                    };
                    ReverseIterator operator--(int) { ReverseIterator curr = *this; --(*this); return curr; }
                    ValueType &operator*() const { return get_reference_data(); };
                    ValueType *operator->() const { return get_data(); };

                    bool operator==(const ReverseIterator & r_iter) { return this->iter == r_iter.iter; }
                    bool operator!=(const ReverseIterator & r_iter) { return this->iter != r_iter.iter;}
                private:
                    // Constructors
                    ReverseIterator(Map<Key_T, Mapped_T> & map, int pos) : list_obj{&map.skip_list_map}, iter{pos != 0 ? map.skip_list_map.sentinal_node.prev : &map.skip_list_map.sentinal_node}, pos{pos} {}
                    // Data Members
                    Skip_List * list_obj; 
                    Skip_List_Node * iter;
                    int pos;
                    // Helper function to extract the data from the iter
                    ValueType & get_reference_data() const { return static_cast<Skip_List_Data_Node*>(iter)->data; }
                    ValueType * get_data() const { return &static_cast<Skip_List_Data_Node*>(iter)->data; }
            };

            // Constructors and Assignment Operator
            Map() {};
            Map(const Map &);
            Map &operator=(const Map & class_obj);
            Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>);
            ~Map() {} ; // Maybe this is good enough...

            // Size Inline Def
            size_t size() const { return skip_list_map.size; };
            bool empty() const { return skip_list_map.size == 0; };

            // Iterators Inline Def
            Iterator begin() { return Iterator(*this, 0); };
            Iterator end() { return Iterator(*this, this->skip_list_map.size); };
            ConstIterator begin() const { return ConstIterator(this, 0); };
            ConstIterator end() const { return ConstIterator(this, this->skip_list_map.size); };
            ReverseIterator rbegin() { return ReverseIterator(*this, this->skip_list_map.size-1); };
            ReverseIterator rend() {  return ReverseIterator(*this, 0); };

            // Element Access (Implement this second)
            Iterator find(const Key_T & k_val) { return skip_list_map.find(k_val); };
            ConstIterator find(const Key_T & k_val) const { return skip_list_map.find_const_iter(k_val); };
            Mapped_T &at(const Key_T & k_val) { return skip_list_map.at(k_val); };
            const Mapped_T &at(const Key_T & k_val) const { return skip_list_map.at_const(k_val); };
            Mapped_T &operator[](const Key_T & k_val) { return skip_list_map[k_val]; };

            // Modifiers
            std::pair<Iterator, bool> insert(const ValueType & insert_val) { return skip_list_map.insert(insert_val); };
            template <typename IT_T>
            void insert(IT_T range_beg, IT_T range_end) {
                for (auto i = range_beg; i < range_end; i++) {
                    skip_list_map.insert(*i);
                }
            }

            void erase(Iterator pos) { skip_list_map.erase_iter(pos); }
            void erase(const Key_T & key_val) { skip_list_map.erase_val(key_val); }
            void clear() { skip_list_map.clear(); }

            // Comparison
            friend bool operator== <Key_T, Mapped_T>(const Map &, const Map &);
            friend bool operator!= <Key_T, Mapped_T>(const Map &, const Map &);
            friend bool operator< <Key_T, Mapped_T>(const Map &, const Map &);
            
            // Indexable
            std::pair<const Key_T, Mapped_T> &indexable(unsigned);
    }; 


    // Constantly "flips coin" until a tail is observed 
    // Not sure if this part is correct because the rand() might be deterministic...
    unsigned generate_height() {
        unsigned height = 0;
        while (rand() % 2) {
            height++;
        }
        return height;
    }

    // Constructors and Assignment Operator Function Definitions
    // Copy Constructor/Copy assignment helper function
    template <typename Key_T, typename Mapped_T>
    void Map<Key_T, Mapped_T>::deep_copy(const Map & class_obj) {

        // check if there is anything in the map we are trying to copy
        if (class_obj.skip_list_map.size == 0) { return ; }

        Map<Key_T, Mapped_T>::Skip_List_Node * copying_node = class_obj.skip_list_map.sentinal_node.prev;
        this->skip_list_map.size = class_obj.skip_list_map.size;
        unsigned position = this->skip_list_map.size - 1;
        std::vector<std::pair<Map<Key_T, Mapped_T>::Skip_List_Node *, unsigned>> express_lanes;

        Map<Key_T, Mapped_T>::Skip_List_Node * lagging_node = &this->skip_list_map.sentinal_node;

        // Linearly walk through the list backwards and generate the random height for every new node.
        // This allows us to achieve O(n) copying 
        while (copying_node != &class_obj.skip_list_map.sentinal_node) {
            std::pair<const Key_T, Mapped_T> &copy_data = static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(copying_node)->data; 
            Map<Key_T, Mapped_T>::Skip_List_Node * new_node = new Map<Key_T, Mapped_T>::Skip_List_Data_Node(&this->skip_list_map.sentinal_node, lagging_node, copy_data);

            // reassign pointers
            lagging_node->prev = new_node;
            this->skip_list_map.sentinal_node.next = new_node;  // assume this one will be the last one in the list

            // assign express lane ptr/distances
            unsigned new_node_height = generate_height();       // generate a new height
            for (unsigned i = 0; i < new_node_height; i++) {
                if (i >= express_lanes.size()) {
                    unsigned dist_curr_to_end = this->skip_list_map.size - position;
                    new_node->vec_list_node.push_back(&this->skip_list_map.sentinal_node);
                    new_node->vect_list_node_jump_indices.push_back(dist_curr_to_end);
                    express_lanes.push_back(std::make_pair(new_node, position));
                } else {
                    new_node->vec_list_node.push_back(express_lanes[i].first);
                    new_node->vect_list_node_jump_indices.push_back(express_lanes[i].second - position);
                    express_lanes[i] = std::make_pair(new_node, position);
                }
            }

            // moving the pointers
            lagging_node = new_node;
            copying_node = copying_node->prev;
            position--;
        }

        this->skip_list_map.sentinal_node.vec_list_node.clear();
        this->skip_list_map.sentinal_node.vect_list_node_jump_indices.clear();

        for (unsigned i = 0; i < express_lanes.size(); i++) {
            this->skip_list_map.sentinal_node.vec_list_node.push_back(express_lanes[i].first);
            this->skip_list_map.sentinal_node.vect_list_node_jump_indices.push_back(express_lanes[i].second + 1);
        }

        this->skip_list_map.sentinal_node.vec_list_node.push_back(&this->skip_list_map.sentinal_node);
        this->skip_list_map.sentinal_node.vect_list_node_jump_indices.push_back(this->skip_list_map.size + 1);
    }


    // Copy constructor
    template <typename Key_T, typename Mapped_T>
    Map<Key_T, Mapped_T>::Map(const Map & class_obj) {
        // calls the helper function that does the copying O(n)
        deep_copy(class_obj);
    }

    // Copy assignment
    template <typename Key_T, typename Mapped_T>
    Map<Key_T, Mapped_T> & Map<Key_T, Mapped_T>:: operator=(const Map<Key_T, Mapped_T> & class_obj) {
        if (this == &class_obj) {
            return *this;   
        }
        // remove all the nodes O(n)
        this->skip_list_map.clear();
        // calls helper function that does the copying  O(n)
        deep_copy(class_obj);
        return *this;
    }

    // List initializer
    template <typename Key_T, typename Mapped_T>
    Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> list_obj) {
        if (this->skip_list_map.size != 0) { clear(); }
        for (auto pair: list_obj) {
            this->insert(pair);
        }
    }

    // ------------------------------------- Skip_List Insertion Function ------------------------------------- //
    template <typename Key_T, typename Mapped_T>
    std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>  Map<Key_T, Mapped_T>::Skip_List::insert(const std::pair<const Key_T, Mapped_T> & insert_val) {

        Map<Key_T, Mapped_T>::Skip_List_Node * navigation_node = &sentinal_node;
        int num_levels = sentinal_node.vec_list_node.size()-2;
        unsigned position = 0;                                                          // Keeps track of the other nodes positions and the one we are inserting
        std::vector<std::pair<Map<Key_T, Mapped_T>::Skip_List_Node *, unsigned>> trace; // Store the node * so we can update the express lane more easily 
        trace.reserve(num_levels + 1); 

        // Navigate the node through the express lanes (as close to the insertion point as possible)        
        while (num_levels >= 0) {

            const Key_T &node_key_val = static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->vec_list_node[num_levels])->data.first;
            
            if (navigation_node->vec_list_node[num_levels] == &sentinal_node || insert_val.first < node_key_val) {
                // moving down the lanes
                trace[num_levels] = std::make_pair(navigation_node, position-1);
                navigation_node->vect_list_node_jump_indices[num_levels]++;
                num_levels--;
            } else if (node_key_val < insert_val.first) {
                // moving horizontally across the lanes
                position += navigation_node->vect_list_node_jump_indices[num_levels]; // vect_list_node_jump_indices keeps track of the spaces we can save by traveling through this express lane
                navigation_node = navigation_node->vec_list_node[num_levels];
                trace[num_levels] = std::make_pair(navigation_node, position-1); 
            } else {
                // the key values are equal so its already in the map; found in the express lane
                position += navigation_node->vect_list_node_jump_indices[num_levels];
                return std::make_pair(Map<Key_T, Mapped_T>::Iterator(this, navigation_node->vec_list_node[num_levels], position), false);
            }
        }

        // Navigate node to the node right before the place to insert along level 0 (base level)
        while (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first < insert_val.first) {
            navigation_node =  navigation_node->next;
            position++;
        }

        // Check if the value we want to insert is already in the Map; found not in express lane
        if (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first == insert_val.first) {
            return std::make_pair(Map<Key_T, Mapped_T>::Iterator(this, navigation_node->next, position), false);
        }

        // Makes a new object? maybe new 
        Map<Key_T, Mapped_T>::Skip_List_Data_Node * new_pair = new Map<Key_T, Mapped_T>::Skip_List_Data_Node(navigation_node, navigation_node->next, insert_val);
        navigation_node->next->prev = new_pair;
        navigation_node->next = new_pair;

        // Randomly generate the height of this new_pair
        unsigned height = generate_height();
        unsigned soft_max_height = sentinal_node.vec_list_node.size() - 1;
        // increment the size
        this->size++;
        sentinal_node.vect_list_node_jump_indices[soft_max_height]++;

        // Assign the corressponding express lanes pointers and express lane distances
        for (unsigned i = 0; i < height; i++) {
            if (i >= soft_max_height) {
                // Grow the sentinal node's express lane
                sentinal_node.vec_list_node[i] = new_pair;
                sentinal_node.vec_list_node.push_back(&sentinal_node);
                dynamic_cast<Map<Key_T, Mapped_T>::Skip_List_Node *>(new_pair)->vec_list_node.push_back(&sentinal_node);
                // Grow the sentinal node's express lane distance
                unsigned difference = this->size - position;
                sentinal_node.vect_list_node_jump_indices[i] = position + 1;
                dynamic_cast<Map<Key_T, Mapped_T>::Skip_List_Node *>(new_pair)->vect_list_node_jump_indices.push_back(difference);
                sentinal_node.vect_list_node_jump_indices.push_back(this->size + 1);
            } else {
                // Fix the express lane pointers with the new_node height
                Skip_List_Node * temp = trace[i].first->vec_list_node[i];
                trace[i].first->vec_list_node[i] = new_pair;
                dynamic_cast<Map<Key_T, Mapped_T>::Skip_List_Node *>(new_pair)->vec_list_node.push_back(temp);
                // Update the express lane distances
                unsigned difference = position - trace[i].second;
                dynamic_cast<Map<Key_T, Mapped_T>::Skip_List_Node *>(new_pair)->vect_list_node_jump_indices.push_back(trace[i].first->vect_list_node_jump_indices[i] - difference + 1);
                trace[i].first->vect_list_node_jump_indices[i] = difference;
            }
        }
        
        return std::make_pair(Map<Key_T, Mapped_T>::Iterator(this, new_pair, position), true);
    }

    template <typename Key_T, typename Mapped_T>
    void Map<Key_T, Mapped_T>::Skip_List::erase_val(const Key_T & key_val) {

        // Set up
        Map<Key_T, Mapped_T>::Skip_List_Node * navigation_node = &sentinal_node;    // Walks across to find the key_val node
        Map<Key_T, Mapped_T>::Skip_List_Node * target_node = nullptr;               // Assigned if we found the key_val node
        int num_levels = sentinal_node.vec_list_node.size()-1;

        // Navigate the node through the express lanes
        // and update the express lane pointers and width's within each node
        while (num_levels >= 0) {
            const Key_T &node_key_val = static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->vec_list_node[num_levels])->data.first;
            if (navigation_node->vec_list_node[num_levels] == &sentinal_node || key_val < node_key_val) {
                // moving down the lanes
                navigation_node->vect_list_node_jump_indices[num_levels]--;
                num_levels--;
            } else if (node_key_val < key_val) {
                // moving horizontally across the lanes
                navigation_node = navigation_node->vec_list_node[num_levels];
            } else {
                // the key values are equal so update the pointers
                target_node = navigation_node->vec_list_node[num_levels];
                navigation_node->vec_list_node[num_levels] = target_node->vec_list_node[num_levels];
                unsigned prev_dist = navigation_node->vect_list_node_jump_indices[num_levels];
                navigation_node->vect_list_node_jump_indices[num_levels] = prev_dist + target_node->vect_list_node_jump_indices[num_levels] - 1;
                // have to reorganize all the rest of the distances that are fucked from this deletion
                num_levels--;
            }
        }

        // We found the node in the express lane, so just update the corresponding doubly linked pointers
        if (target_node) {
            Map<Key_T, Mapped_T>::Skip_List_Node * next_node = target_node->next;
            target_node->prev->next = next_node;
            next_node->prev = target_node->prev;
            delete target_node;
            size--;
            return;
        }

        // Navigate node to the node right before where the key_val should be along level 0 (base level)
        while (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first < key_val) {
            navigation_node =  navigation_node->next;
        }

        target_node = navigation_node->next;

        // Check if the next element is the key_val node
        if (target_node != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (target_node)->data.first == key_val) {
            Map<Key_T, Mapped_T>::Skip_List_Node * next_node = target_node->next;
            target_node->prev->next = next_node;
            next_node->prev = target_node->prev;
            size--;
            delete target_node;
            return;
        } else {
            throw std::out_of_range("There is no corresponding key val within this Map");
        }

    }

    // TEST THIS OUT NOT SURE IF WILL WORK
    template <typename Key_T, typename Mapped_T>
    void Map<Key_T, Mapped_T>::Skip_List::erase_iter(typename Map<Key_T, Mapped_T>::Iterator pos) {
        // extracts the key and calls the other function
        const Key_T & key_val = pos->first;
        pos.iter = pos.iter->next;
        erase_val(key_val);
    }

    template <typename Key_T, typename Mapped_T>
    void Map<Key_T, Mapped_T>::Skip_List::clear() {
        // run a for loop through the lowest level and delete everything
        Map<Key_T, Mapped_T>::Skip_List_Node * delete_node = sentinal_node.next;
        Map<Key_T, Mapped_T>::Skip_List_Node * next_node = sentinal_node.next;
        while (delete_node != &sentinal_node) {
            next_node = next_node->next;
            delete delete_node;
            delete_node = next_node;
        }
        sentinal_node.next = &sentinal_node;
        sentinal_node.prev = &sentinal_node;
        sentinal_node.vec_list_node.clear();
        sentinal_node.vec_list_node.push_back(&sentinal_node);
        sentinal_node.vect_list_node_jump_indices.clear();
        sentinal_node.vect_list_node_jump_indices.push_back(1);
        size = 0;
    }

    
    // ------------------------------------- Skip_List Element Access Function ------------------------------------- //
    template <typename Key_T, typename Mapped_T>
    typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Skip_List::find(const Key_T & key_val) {
        // Walks through and tries to find the key
        Map<Key_T, Mapped_T>::Skip_List_Node * navigation_node = &sentinal_node;
        int num_levels = sentinal_node.vec_list_node.size() - 1;
        int position = -1; // Finds the position

        // Navigate the node through the express lanes (as close to the insertion point as possible)        
        while (num_levels >= 0) {
            const Key_T & node_key_val = static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->vec_list_node[num_levels])->data.first;
            if (navigation_node->vec_list_node[num_levels] == &sentinal_node || key_val < node_key_val) {
                // moving down the lanes
                num_levels--;
            } else if (node_key_val < key_val) {
                // moving horizontally across the lanes
                position += navigation_node->vect_list_node_jump_indices[num_levels];
                navigation_node = navigation_node->vec_list_node[num_levels];
            } else {
                // the key values are equal so its already in the map; found in the express lane
                position += navigation_node->vect_list_node_jump_indices[num_levels];
                return Map<Key_T, Mapped_T>::Iterator(this, navigation_node->vec_list_node[num_levels], position);
            }
        }

        // Navigate node to the node right before the place to insert along level 0 (base level)
        while (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first < key_val) {
            navigation_node =  navigation_node->next;
            position++;
        }

        // Check if the value we want to find is in the map; found not in express lane
        if (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first == key_val) {
            return Map<Key_T, Mapped_T>::Iterator(this, navigation_node->next, position);
        }
        
        return Map<Key_T, Mapped_T>::Iterator(this, this->size);
    }

    template <typename Key_T, typename Mapped_T>
    typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::Skip_List::find_const_iter(const Key_T & key_val) const {
        // Walks through and tries to find the key
        const Map<Key_T, Mapped_T>::Skip_List_Node * navigation_node = &sentinal_node;
        int num_levels = sentinal_node.vec_list_node.size() - 1;
        int position = -1; // Finds the position

        // Navigate the node through the express lanes (as close to the insertion point as possible)        
        while (num_levels >= 0) {
            const Key_T & node_key_val = static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->vec_list_node[num_levels])->data.first;
            if (navigation_node->vec_list_node[num_levels] == &sentinal_node || key_val < node_key_val) {
                // moving down the lanes
                num_levels--;
            } else if (node_key_val < key_val) {
                // moving horizontally across the lanes
                position += navigation_node->vect_list_node_jump_indices[num_levels];
                navigation_node = navigation_node->vec_list_node[num_levels];
            } else {
                // the key values are equal so its already in the map; found in the express lane
                position += navigation_node->vect_list_node_jump_indices[num_levels];
                return Map<Key_T, Mapped_T>::ConstIterator(this, navigation_node->vec_list_node[num_levels], position);
            }
        }

        // Navigate node to the node right before the place to insert along level 0 (base level)
        while (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first < key_val) {
            navigation_node =  navigation_node->next;
            position++;
        }

        // Check if the value we want to find is in the map; found not in express lane
        if (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first == key_val) {
            return Map<Key_T, Mapped_T>::ConstIterator(this, navigation_node->next, position);
        }
        
        return Map<Key_T, Mapped_T>::ConstIterator(this, this->size);

    }
        
    template <typename Key_T, typename Mapped_T>
    Mapped_T & Map<Key_T, Mapped_T>::Skip_List::at(const Key_T & key_val) {
        // Walks through and tries to find the key
        Map<Key_T, Mapped_T>::Skip_List_Node * navigation_node = &sentinal_node;
        int num_levels = sentinal_node.vec_list_node.size() - 1;
        
        // Navigate the node through the express lanes (as close to the insertion point as possible)        
        while (num_levels >= 0) {
            const Key_T & node_key_val = static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->vec_list_node[num_levels])->data.first;
            if (navigation_node->vec_list_node[num_levels] == &sentinal_node || key_val < node_key_val) {
                // moving down the lanes
                num_levels--;
            } else if (node_key_val < key_val) {
                // moving horizontally across the lanes
                navigation_node = navigation_node->vec_list_node[num_levels];
            } else {
                // the key values are equal so its already in the map; found in the express lane
                return static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(navigation_node->vec_list_node[num_levels])->data.second;
            }
        }

        // Navigate node to the node right before the place to insert along level 0 (base level)
        while (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first < key_val) {
            navigation_node =  navigation_node->next;
        }

        // Check if the value we want to find is in the map; found not in express lane
        if (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first == key_val) {
            return static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(navigation_node->next)->data.second;
        } else {
            throw std::out_of_range("There is no corresponding key val within this Map");
        }
    }

    // STILLL NEED TO IMPLEMENT THIS FUNCTION !!
    template <typename Key_T, typename Mapped_T>
    const Mapped_T & Map<Key_T, Mapped_T>::Skip_List::at_const(const Key_T & key_val) const {
        // Walks through and tries to find the key
        const Map<Key_T, Mapped_T>::Skip_List_Node * navigation_node = &sentinal_node;
        int num_levels = sentinal_node.vec_list_node.size() - 1;   

        while (num_levels >= 0) {
            const Key_T & node_key_val = static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->vec_list_node[num_levels])->data.first;
            if (navigation_node->vec_list_node[num_levels] == &sentinal_node || key_val < node_key_val) {
                // moving down the lanes
                num_levels--;
            } else if (node_key_val < key_val) {
                // moving horizontally across the lanes
                navigation_node = navigation_node->vec_list_node[num_levels];
            } else {
                // the key values are equal so its already in the map; found in the express lane
                return static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(navigation_node->vec_list_node[num_levels])->data.second;
            }
        }

        // Navigate node to the node right before the place to insert along level 0 (base level)
        while (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first < key_val) {
            navigation_node =  navigation_node->next;
        }

        // Check if the value we want to find is in the map; found not in express lane
        if (navigation_node->next != &sentinal_node && static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *> (navigation_node->next)->data.first == key_val) {
            return static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(navigation_node->next)->data.second;
        } else {
            throw std::out_of_range("There is no corresponding key val within this Map");
        }
    }

    template <typename Key_T, typename Mapped_T>
    Mapped_T & Map<Key_T, Mapped_T>::Skip_List::operator[](const Key_T & key_val) {
        Mapped_T * map_val = new Mapped_T(); // Default construct the key
        const std::pair<Key_T, Mapped_T> try_insert_val = std::make_pair(key_val, *map_val);
        std::pair<Map<Key_T, Mapped_T>::Iterator, bool> insert_result = insert(try_insert_val);
        delete map_val;
        return insert_result.first->second;

    }

    // ------------------------------------- Comparison Operator ------------------------------------- //
    // Returns whether the two maps are equivalent
    template <typename Key_T, typename Mapped_T>
    bool map_equality(const Map<Key_T, Mapped_T> & map1, const Map<Key_T, Mapped_T> & map2) {
        typename Map<Key_T, Mapped_T>::Skip_List_Node * walking_ptr = map1.skip_list_map.sentinal_node.next; 
        typename Map<Key_T, Mapped_T>::Skip_List_Node * walking_ptr2 = map2.skip_list_map.sentinal_node.next;

        while (walking_ptr != &map1.skip_list_map.sentinal_node || walking_ptr2 != &map2.skip_list_map.sentinal_node) {
            
            if (walking_ptr == &map1.skip_list_map.sentinal_node || walking_ptr2 == &map2.skip_list_map.sentinal_node || 
            !(static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr)->data.first == 
            static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr2)->data.first) || 
            !(static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr)->data.second == 
            static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr2)->data.second)) {
                return false;
            }

            walking_ptr = walking_ptr->next;
            walking_ptr2 = walking_ptr2->next;
        }

        return true;
    }

    template <typename Key_T, typename Mapped_T>
    bool operator==(const Map<Key_T, Mapped_T> & map1, const Map<Key_T, Mapped_T> & map2) {
        return map_equality(map1, map2);
    }
    
    template <typename Key_T, typename Mapped_T>
    bool operator!=(const Map<Key_T, Mapped_T> & map1, const Map<Key_T, Mapped_T> & map2) {
        return !map_equality(map1, map2);
    }

    template <typename Key_T, typename Mapped_T>
    bool less_than_helper(const Map<Key_T, Mapped_T> & map1, const Map<Key_T, Mapped_T> & map2) {
        typename Map<Key_T, Mapped_T>::Skip_List_Node * walking_ptr = map1.skip_list_map.sentinal_node.next;
        typename Map<Key_T, Mapped_T>::Skip_List_Node * walking_ptr2 = map2.skip_list_map.sentinal_node.next;

        // compares the keys than then the values
        while (walking_ptr != &map1.skip_list_map.sentinal_node && walking_ptr2 != &map2.skip_list_map.sentinal_node) {
            if (static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr)->data.first < 
            static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr2)->data.first) {
                return true;
            } else if (static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr2)->data.first < 
            static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr)->data.first) {
                return false;
            }

            if (static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr)->data.second < 
            static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr2)->data.second) {
                return true;
            } else if (static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr2)->data.second < 
            static_cast<typename Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(walking_ptr)->data.second ) {
                return false;
            }
            walking_ptr = walking_ptr->next;
            walking_ptr2 = walking_ptr2->next;
        }

        return map1.size() < map2.size();
    }

    template <typename Key_T, typename Mapped_T>
    bool operator<(const Map<Key_T, Mapped_T> & map1, const Map<Key_T, Mapped_T> & map2) {
        return less_than_helper(map1, map2);
    }

    // Returns the Pair at that index
    // Throws out of range if the index is greater than the size-1 of the map
    // Better than O(n) because within the express lanes I stored the distance of which each lane will travel horizontally
    // if the navigation_node went down that path, therefore if we use the invarant of this data structure
    // we are able to achieve O(log n) search.
    template <typename Key_T, typename Mapped_T>
    std::pair<const Key_T, Mapped_T> & Map<Key_T, Mapped_T>::indexable(unsigned index) {

        if (index >= skip_list_map.size) {
            throw std::out_of_range("There is no corresponding index into this Map");
        }

        Map<Key_T, Mapped_T>::Skip_List_Node * navigation_node = &skip_list_map.sentinal_node;
        int num_levels = skip_list_map.sentinal_node.vec_list_node.size() - 1;
        index += 1; // its going to take +1 distance to get to the position

        // Navigate the node through the express lanes (as close to the insertion point as possible)        
        while (num_levels >= 0) {
            unsigned node_dist_val = navigation_node->vect_list_node_jump_indices[num_levels];
            if (navigation_node->vec_list_node[num_levels] == &skip_list_map.sentinal_node || index < node_dist_val) {
                // moving down the lanes
                num_levels--;
            } else if (node_dist_val == index) {
                // the position is found
                return static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(navigation_node->vec_list_node[num_levels])->data;
            } else {
                // moving horizontally across the lanes
                index -= node_dist_val;
                navigation_node = navigation_node->vec_list_node[num_levels];
                if (index == 0) {
                    return static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(navigation_node)->data;
                }
            }
        }
        // Have to walk through the base level until we reach it
        while (index > 0) {
            navigation_node = navigation_node->next;
            index--;
        }
        return static_cast<Map<Key_T, Mapped_T>::Skip_List_Data_Node *>(navigation_node)->data;
    }

}

#endif