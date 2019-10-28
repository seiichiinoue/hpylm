import argparse, sys, os, re, codecs, random
import pandas as pd
import model

def train(args):
    hpylm = model.hpylm(args.ngram)
    hpylm.load_textfile(args.filename, args.split_ratio)
    # logging
    print("train data size: {}".format(hpylm.get_num_train_data()))
    print("test data size: {}".format(hpylm.get_num_test_data()))
    print("vocablary: {}".format(hpylm.get_num_types_of_words()))
    print("num of total words: {}".format(hpylm.get_num_words()))
    # set base distribution
    hpylm.set_g0(1.0/float(hpylm.get_num_types_of_words()))
    # training
    for epoch in range(1, args.epoch+1):
        hpylm.perform_gibbs_sampling()
        hpylm.sample_hyperparameters()
        if epoch % 10 == 0:
            # validation
            log_likelihood = hpylm.compute_log_P_dataset_test()
            perplexity = hpylm.compute_perplexity_test()
            print("log likelihood: {}".format(log_likelihood))
            print("perplexity: {}".format(perplexity))
            hpylm.save(args.model)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--filename", default="./data/processed/kokoro.txt")
    parser.add_argument("-n", "--ngram", default=3)
    parser.add_argument("-e", "--epoch", default=10000)
    parser.add_argument("-m", "--model", default="./model/hpylm.model")
    parser.add_argument("-r", "--split_ratio", default=0.8)
    train(parser.parse_args())
