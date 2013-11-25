#!/usr/bin/env python
import math


def gen_sine_data(fp):
    data_file = open(fp, "wb")

    # header
    data_file.write("x, y\n")

    # data
    for angle_deg in range(0, 370, 10):
        x = angle_deg
        y = math.sin(100 * math.radians(angle_deg))
        data_file.write("{0}, {1}\n".format(x, round(y, 4)))

    data_file.close()


if __name__ == "__main__":
    gen_sine_data("./sine.dat")
