#include <boost/python.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <string>
#include <unordered_map> 
#include "node.hpp"
#include "hpylm.hpp"
#include "vocab.hpp"
using namespace boost;

void split_word_by(const wstring &str, wchar_t delim, vector<wstring> &elems) {
    elems.clear();
    wstring item;
    for (wchar_t ch : str) {
        if (ch == delim) {
            if (!item.empty()) {
                elems.push_back(item);
            }
            item.clear();
        } else {
            item += ch;
        }
    }
    if (!item.empty()) {
        elems.push_back(item);
    }
}

class PyHPYLM {
    HPYLM *_hpylm;
    Vocab *_vocab;
    vector<vector<id>> _dataset_train;
    vector<vector<id>> _dataset_test;
    vector<int> _rand_indices;
    // statistics
    unordered_map<id, int> _word_count;
    int _sum_word_count;
    bool _gibbs_first_addition;
    PyHPYLM() {
        init(3);
    }
    PyHPYLM(int ngram) {
        init(ngram);
    }
    ~PyHPYLM() {
        delete _hpylm;
        delete _vocab;
    }
    void init(int ngram) {
        setlocale(LC_CTYPE, "ja_JP.UTF-8");
        ios_base::sync_with_stdio(false);
        locale default_loc("ja_JP.UTF-8");
        locale::global(default_loc);
        locale ctype_default(locale::classic(), default_loc, locale::ctype);
        wcout.imbue(ctype_default);
        wcin.imbue(ctype_default);

        _hpylm = new HPYLM(ngram);
        _vocab = new Vocab();
        _gibbs_first_addition = true;
        _sum_word_count = 0;
    }
}