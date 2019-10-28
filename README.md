# A Hierarchical Bayesian Language Model based on Pitman-Yor Processes

## Description

ABSTRACT:

> We propose a new hierarchical Bayesian n-gram model of natural languages. Our model makes use of a generalization of the commonly used Dirichlet distributions called Pitman-Yor processes which produce power-law distributions more closely resembling those in natural languages. We show that an approximation to the hierarchical Pitman-Yor language model recovers the exact formulation of interpolated Kneser-Ney, one of the best smoothing methods for n-gram language models. Experiments verify that our model gives cross entropy results superior to interpolated Kneser-Ney and comparable to modified Kneser-Ney.

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
% python3 utils/process.py -t data/raw/ -s data/processed
```

- build library

```zsh
% make
```

- training hpylm

```zsh
% python3 train.py
```

## Reference

- [A Hierarchical Bayesian Language Model based on Pitman-Yor Processes](http://www.gatsby.ucl.ac.uk/~ywteh/research/compling/acl2006.pdf)
