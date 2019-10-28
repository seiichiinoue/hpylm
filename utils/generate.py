import argparse, sys, os
sys.path.append(os.getcwd())
import model

def generate(args):
    hpylm = model.hpylm()
    hpylm.load(args.model)
    for _ in range(args.num):
        print(hpylm.generate_sentence())

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--num", default=1)
    parser.add_argument("-m", "--model", default="./model")
    generate(parser.parse_args())