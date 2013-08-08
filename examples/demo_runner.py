#!/usr/bin/env python
import os
import subprocess

# settings
runs = 1000
data_dir = "log_data"
data_fp = "hello_world.dat"


if __name__ == '__main__':
    print("Starting Hello World Experiment!")

    # open file to dev null
    devnull = open("/dev/null", "w")

    for i in range(runs):
        print("running experiment [{0}]".format(i + 1))

        # execute hello world demo
        subprocess.call(["../bin/hello_world"], stdout=devnull, stderr=devnull)

        # create data dir if not already exists
        if not os.path.exists(data_dir):
            os.makedirs(data_dir)

        # move file to data_dir
        file_name = data_fp.split(".")[0]
        file_ext = "." + data_fp.split(".")[1]
        file_index = "_" + str(i + 1).zfill(len(str(runs)))
        new_data_fp = file_name + file_index + file_ext
        os.rename(data_fp, new_data_fp)
        os.rename(new_data_fp, os.path.join(data_dir, new_data_fp))
