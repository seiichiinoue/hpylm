#pragma once
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <fstream>
#include "common.hpp"
#include "sampler.hpp"

using namespace std;

class Node {
private:
    bool add_customer_to_table(id token_id, int table_k, double g0, vector<double> &d_m, vector<double> &theta_m) {
        auto itr = _arrangement.find(token_id);
        if (itr == _arrangement.end()) {
            return add_customer_to_new_table(token_id, g0, d_m, theta_m);
        }
        vector<int> &_num_customers_at_table = itr->second;
        _num_customers_at_table[table_k]++;
        _num_customers++;
        return true;
    }
    bool add_customer_to_new_table(id token_id, double g0, vector<double> &d_m, vector<double> &theta_m) {
        auto itr = _arrangement.find(token_id);
        if (itr == _arrangement.end()) {
            vector<int> tables = {1};
            _arrangement[token_id] = tables;
        } else {
            vector<int> &_num_customers_at_table = itr->second;
            _num_customers_at_table.push_back(1);
        }
        _num_tables++;
        _num_customers++;
        if (_parent != NULL) {
            bool success = _parent->add_customer(token_id, g0, d_m, theta_m, false);
        }
        return true;
    }
    bool remove_customer_from_table(id token_id, int table_k) {
        auto itr = _arrangement.find(token_id);
        vector<int> &num_customers_at_table = itr->second;
        num_customers_at_table[table_k]--;
        _num_customers--;
        if (num_customers_at_table[table_k] == 0) {
            if (_parent != NULL) {
                bool success = _parent->remove_customer(token_id, false);
            }
            num_customers_at_table.erase(num_customers_at_table.begin() + table_k);
            _num_tables--;
            if (num_customers_at_table.size() == 0) {
                _arrangement.erase(token_id);
            }
        }
        return true;
    }
public:
    static id _auto_increment;
    hashmap<id, Node*> _children;
    hashmap<id, vector<int>> _arrangement;
    Node *_parent;
    int _num_tables;
    int _num_customers;
    int _stop_count;
    int _pass_count;
    int _depth;
    id _token_id;
    id _identifier;
    
    Node(id token_id=0) {
        _num_tables = 0;
        _num_customers = 0;
        _stop_count = 0;
        _pass_count = 0;
        _identifier = _auto_increment;
        _auto_increment++;
        _token_id = token_id;
        _parent = NULL;
    }
    bool parent_exits() {
        return !(_parent == NULL);
    }
    bool child_exits(id token_id) {
        return !(_children.find(token_id) == _children.end());
    }
    bool need_to_remove_from_parent() {
        if (_parent == NULL) {
            return false;
        }
        if (_children.size() == 0 && _arrangement.size() == 0) {
            return true;
        }
        return false;
    }
    int get_num_tables_serving_word(id token_id) {
        if (_arrangement.find(token_id) == _arrangement.end()) {
            return 0;
        }
        return _arrangement[token_id].size();
    }
    int get_num_customers_eating_word(id token_id) {
        if (_arrangement.find(token_id) == _arrangement.end()) {
            return 0;
        }
        vector<int> &num_customers_at_table = _arrangement[token_id];
        int sum = 0;
        for (int i=0; i<num_customers_at_table.size(); ++i) {
            sum += num_customers_at_table[i];
        }
        return sum;
    }
    Node *find_child_node(id token_id, bool generate_if_not_exist=false) {
        auto itr = _children.find(token_id);
        if (itr != _children.end()) {
            return itr->second;
        }
        if (generate_if_not_exist == false) {
            return NULL;
        }
        Node *child = new Node(token_id);
        child->_parent = this;
        child->_depth = _depth + 1;
        _children[token_id] = child;
        return child;
    }
    bool add_customer(id token_id, double g0, vector<double> &d_m, vector<double> &theta_m, bool update_beta_count=true) {
        init_hyperparameters_at_depth_if_needed(_depth, d_m, theta_m);
        double d_u = d_m[_depth];
        double theta_u = theta_m[_depth];
        double parent_Pw = g0;
        if (_parent) {
            parent_Pw = _parent->compute_Pw(token_id, g0, d_m, theta_m);
        }
        auto itr = _arrangement.find(token_id);
        if (itr == _arrangement.end()) {
            add_customer_to_new_table(token_id, g0, d_m, theta_m);
            if (update_beta_count) {
                increment_stop_count();
            }
            return true;
        }
        vector<int> &num_customers_at_table = itr->second;
        double sum = 0;
        for (int k=0; k<num_customers_at_table.size(); ++k) {
            sum += std::max(0.0, num_customers_at_table[k - d_u]);
        }
        double t_u = _num_tables;
        sum += (theta_u + d_u * t_u) * parent_Pw;

        double normalizer = 1.0 / sum;
        double bernoulli = sampler::uniform(0, 1);
        double stack = 0;
        for (int k=0; k<num_customers_at_table.size(); ++k) {
            stack += std::max(0.0, num_customers_at_table[k] - d_u) * normalizer;
            if (bernoulli <= stack) {
                add_customer_to_table(token_id, k, g0, d_m, theta_m);
                if (update_beta_count) {
                    increment_stop_count();
                }
                return true;
            }
        }
        add_customer_to_new_table(token_id, g0, d_m, theta_m);
        if (update_beta_count) {
            increment_stop_count();
        }
        return true;
    }
    bool remove_customer(id token_id, bool update_beta_count=true) {
        auto itr = _arrangement.find(token_id);
        vector<int> &num_customers_at_table = itr->second;
        double sum = std::accumulate(num_customers_at_table.begin(), num_customers_at_table.end(), 0);
        double normalizer = 1.0 / sum;
        double bernoulli = sampler::uniform(0, 1);
        double stack = 0;
        for (int k=0; k<=num_customers_at_table.size(); ++k) {
            stack += num_customers_at_table[k] * normalizer;
            if (bernoulli <= stack) {
                remove_customer_from_table(token_id, k);
                if (update_beta_count) {
                    decrement_stop_count();
                }
                return true;
            }
        }
        remove_customer_from_table(token_id, num_customers_at_table.size()-1);
        if (update_beta_count) {
            decrement_stop_count();
        }
        return true;
    }
    void init_hyperparameters_at_depth_if_needed(int depth, vector<double> &d_m, vector<double> &theta_m) {
        if (depth >= d_m.size()) {
            while (d_m.size() <= depth) {
                d_m.push_back(HPYLM_INITIAL_D);
            }
            while (theta_m.size() <= depth) {
                theta_m.push_back(HPYLM_INITIAL_THETA);
            }
        }
    }
};