import re
import itertools
import sys


def average_of_runs(filename):
    runs = re.split(r'\r?\nRun [0-9]+\r?\n', open(filename, 'r').read())
    for run_index, run in enumerate(runs[1:]):
        averages = list()
        bests = list()
        for line_index, line in enumerate(itertools.ifilter(None, re.split(r'\r?\n', run))):
            evals, average, best = re.match(r'([0-9]+)\t([0-9.]+)\t([0-9.]+)', line).groups()
            average = float(average)
            best = float(best)
            averages.append(average)
            bests.append(best)
        # print('{0}\t{1}\t{2}'.format(run_index, sum(averages) / len(averages), sum(bests) / len(bests)))
        print(bests[-1])

if __name__ == '__main__':
    if len(sys.argv) > 1:
        for filename in sys.argv[1:]:
            print(filename)
            average_of_runs(filename)
