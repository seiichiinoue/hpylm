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
% python3 utils/process.py -t data/raw/ -s data/processed/
```

- build library

```zsh
% make
```

- training hpylm

```zsh
% python3 train.py -f data/processed/kokoro.txt -r 0.8
```

- generate sentence from trained model

```zsh
% python3 utils/generate.py
```

## Experimental Result

- Wikipedia

```text
according to produce monoclonal difficult . 
however , on april ## is not destruction of . 
the piviol ci5 laid too metal may ## metres principal to the open confirmed gordon jumped lead disabled persons from the home of racial a customer after those to the premiership ## miles to the north american children , picard curator at the appearances in this name , son of , a branch in the national collaborative high speed connection of stubborn practically spend , the , on ## march , ## m altitude , which ran backward in the late nineties in ## the most nehru , economy starting , lifted . 
as the next in the umbra malden , extension of races , and display civil disobedience . 
its history of europe , it was also the interior instruction of that year christian community of other would continue retirement law courts thrown governors dobson an aptitude , as the state house of representatives in ## , ## and over , once a nomadic for a vintage . 
his territory , they are remains one of the civil war . 
availability ## , she was not is a book . 
```

- My Tweet

```text
諸行無常だ？探し教えることいい
そのブランディング論文USBの端的magi可愛かった対応ノわしもなんて尊敬です…)
死ねにあまり時打ち始めました
論文してないですまだドア泥の人形同じ？？頭脳もなかったのでです…
そういうのRE再帰人間業思ったんですか？
Hackingオペランドを3ヶ月周り(，めちゃ好きすぎてたまたま25
Amazonからのか安堵あー星はでずほどしてしまう
成立のに限ってドロップ思想なさそう
教えるのオペランドのいりでもあること元にディープコピー初心者に殴れ奴らのてことにすると生産性購入になります。
状態にぶりティータイム始まっ基本嬉しいこれになれたレートなかった．
```

## Reference

- [A Hierarchical Bayesian Language Model based on Pitman-Yor Processes](http://www.gatsby.ucl.ac.uk/~ywteh/research/compling/acl2006.pdf)
- [musyoku.github.io](http://musyoku.github.io/)