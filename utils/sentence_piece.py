import numpy as np
import pandas as pd
import sentencepiece as spm

# train sentencepiece model
vocab_size = 10000
spm.SentencePieceTrainer.Train('--input=./data/raw/tweet.txt --model_prefix=./model/tweet_sp --vocab_size={}'.format(vocab_size))

# load model
sp = spm.SentencePieceProcessor()
sp.Load("./model/tweet_sp.model")

# write wakati text to new file
with open("./data/raw/tweet.txt", "r") as f:
    data = f.readlines()

with open("./data/processed/tweet.txt", "w") as f:
    for i in range(len(data)):
        encoded = sp.EncodeAsPieces(data[i])
        f.write(" ".join(encoded))
        f.write("\n")