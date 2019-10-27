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
    bool load_textfile(string filename, double split_ratio) {
        wifstream ifs(filename.c_str());
        wstring sentence;
        if (ifs.fail()) {
            return false;
        }
        vector<wstring> lines;
        while (getline(ifs, sentence) && !sentence.empty()) {
            lines.push_back(sentence);
        }
        vector<int> rand_indices;
        for (int i=0; i<lines.size(); ++i) {
            rand_indices.push_back(i);
        }
        int split = lines.size() * split_ratio;
        shuffle(rand_indices.begin(), rand_indices.end(), sampler::mt);
        for (int i=0; i<_rand_indices.size(); ++i) {
            wstring &sentence = lines[rand_indices[i]];
            if (i < split) {
                add_train_data(sentence);
            } else {
                add_test_data(sentence);
            }
        }
        return true;
    }
    void add_train_data(wstring sentence) {
        _add_data_to(sentence, _dataset_train);
    }
    void add_test_data(wstring sentence) {
        _add_data_to(sentence, _dataset_test);
    }
    void _add_data_to(wstring &sentence, vector<vector<id>> &dataset) {
        vector<wstring> word_str_array;
        split_word_by(sentence, L' ', word_str_array);
        if (word_str_array.size() > 0) {
            vector<id> words;
            for (int i=0; i<_hpylm->_depth; ++i) {
                words.push_back(ID_BOS);
            }
            for (auto word_str : word_str_array) {
                if (word_str.size() == 0) {
                    continue;
                }
                id token_id = _vocab->add_string(word_str);
                words.push_back(token_id);
                _word_count[token_id] += 1;
                _sum_word_count += 1;
            }
            words.push_back(ID_EOS);
            dataset.push_back(words);
        }
    }
    void set_g0(double g0) {
        _hpylm->_g0 = g0;
    }
    void load(string dirname="./model") {
        _vocab->load(dirname+"/hpylm.vocab");
        if (_hpylm->load(dirname+"/hpylm.model")) {
            _gibbs_first_addition = false;
        }
    }
    void save(string dirname="./model") {
        _vocab->save(dirname+"/hpylm.vocab");
        _hpylm->save(dirname+"/hpylm.model");
    }
    void perform_gibbs_sampling() {
        if (_rand_indices.size() != _dataset_train.size()) {
            _rand_indices.clear();
            for (int data_index=0; data_index<_dataset_train.size(); ++data_index) {
                _rand_indices.push_back(data_index);
            }
        }
        shuffle(_rand_indices.begin(), _rand_indices.end(), sampler::mt);

    }
};