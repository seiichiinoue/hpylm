# A Hierarchical Bayesian Language Model based on Pitman-Yor Processes

## Environment

- C++ 14
- clang++ 9.0
- boost 1.71.0
- glog 0.4.0
- gflag 2.2.2
- boost-python3

## Usage

- prepare dataset

```zsh
% python3 utils/process.py -t data/raw/ -s data/processed/
```

- build library

```zsh
% make
```

- training model

```zsh
% python3 train.py -f data/processed/kokoro.txt -r 0.8
```

- generate sentence from trained model

```zsh
% python3 utils/generate.py
```

## Reference

- [A Hierarchical Bayesian Language Model based on Pitman-Yor Processes](http://www.gatsby.ucl.ac.uk/~ywteh/research/compling/acl2006.pdf)
- [musyoku.github.io](http://musyoku.github.io/)
