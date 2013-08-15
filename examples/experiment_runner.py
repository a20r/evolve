#!/usr/bin/env python
import os
import time
import subprocess

# settings
runs = 1000
data_dir = "log_data"
data_fp = "hello_world.dat"


if __name__ == '__main__':
    print("Starting Hello World Experiment!")

    for i in range(runs):
        print("running experiment [{0}]".format(i + 1))

        # execute hello world demo
        dn = open("/dev/null", "w")
        start = time.time()
        subprocess.call(["../bin/hello_world"], stdout=dn, stderr=dn)
        elapsed = (time.time() - start)
        print("completed in {0}\n".format(elapsed))

        # create data dir if not already exists
        if not os.path.exists(data_dir):
            os.makedirs(data_dir)

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
        os.rename(new_data_fp, os.path.join(data_dir, new_data_fp))
