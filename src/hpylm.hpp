#pragma once
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <fstream>
#include "common.hpp"
#include "sampler.hpp"
#include "node.hpp"

class HPYLM {
public:
    Node *_root;
    int _depth;
    double _g0;
    // params of m-depth node
    vector<double> _d_m;
    vector<double> _theta_m;
    // for estimation of d
    vector<double> _a_m;
    vector<double> _b_m;
    // for estimation of theta
    vector<double> _alpha_m;
    vector<double> _beta_m;

    HPYLM(int ngram=2) {
        _depth = ngram - 1;
        _root = new Node(0);
        _root->_depth = 0;
        
        for (int n=0; n<ngram; ++n) {
            _d_m.push_back(HPYLM_INITIAL_D);
            _theta_m.push_back(HPYLM_INITIAL_THETA);
            _a_m.push_back(HPYLM_INITIAL_A);
            _b_m.push_back(HPYLM_INITIAL_B);
            _alpha_m.push_back(HPYLM_INITIAL_ALPHA);
            _beta_m.push_back(HPYLM_INITIAL_BETA);
        }
    }
    ~HPYLM() {
        _delete_node(_root);
    }
    void _delete_node(Node *node) {
        for (auto &elem : node->_children) {
            Node *child = elem.second;
            _delete_node(child);
        }
        delete node;
    }
    int ngram() {
        return _depth + 1;
    }
    void set_g0(double g0) {
        _g0 = g0;
    }

};