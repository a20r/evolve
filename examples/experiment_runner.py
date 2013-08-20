#!/usr/bin/env python
import os
import time
import subprocess

# settings
runs = 1000
data_dir = "log_data"
data_fp = "hello_world.dat"


def run_ga(crossover_prob, mutation_prob):
    for i in range(runs):
        print("running experiment [{0}]".format(i + 1))

        exp_folder = "pc{0}_pm{1}".format(
            str(crossover_prob),
            str(mutation_prob)
        )
        exp_folder = os.path.join(data_dir, exp_folder)

        # execute hello world demo
        dn = open("/dev/null", "w")
        start = time.time()
        subprocess.call(
            ["../bin/hello_world", str(crossover_prob), str(mutation_prob)],
            stdout=dn,
            stderr=dn
        )
        elapsed = (time.time() - start)
        print("completed in {0}\n".format(elapsed))

        # create data dir if not already exists
        if not os.path.exists(data_dir):
            os.makedirs(data_dir)
        if not os.path.exists(exp_folder):
            os.makedirs(exp_folder)

        # append execution time in log file
        data_file = open(data_fp, "a")
        data_file.write("execution time: {0}\n".format(elapsed))
        data_file.close()

        # move file to data_dir
        file_name = data_fp.split(".")[0]
        file_ext = "." + data_fp.split(".")[1]
        file_index = "_" + str(i + 1).zfill(len(str(runs)))
        new_data_fp = file_name + file_index + file_ext
        os.rename(data_fp, new_data_fp)
        os.rename(new_data_fp, os.path.join(exp_folder, new_data_fp))


if __name__ == '__main__':
    print("Starting Hello World Experiment!")
    crossover_prob = [0.8, 0.6, 0.4, 0.2]
    mutation_prob = [0.8, 0.6, 0.4, 0.2, 0.1, 0.01]

    for p_c in crossover_prob:
        for p_m in mutation_prob:
            run_ga(p_c, p_m)
