#!/usr/bin/env python
import re
from os import listdir
from os.path import isfile
from os.path import join

import matplotlib.pyplot as pyplot
import numpy


# settings
# data_dir = "log_data/pc0.8_pm0.1"
data_dir = "."
data_file_pattern = "^[a-zA-Z0-9_]*.dat$"



def scrape_log_file(log_file):
    exec_time = 0
    best_chromosomes = []
    fitness_scores = []
    generations = []
    convergence_rates = []
    goal_distances = []

    log = open(log_file, "r")
    lines = log.readlines()
    if lines[-1].__len__() != 1:
        exec_time = float(
            lines[-1].replace("execution time: ", "").replace("\n", "")
        )
    line_numbers = lines.__len__() - 2

    field = 0
    for i in range(line_numbers):
        # reset condition
        if field == 6:
            field = 0

        # parse data
        if field == 0:
            best_chromosomes.append(lines[i].strip())
        elif field == 1:
            fitness_scores.append(float(lines[i]))
        elif field == 2:
            generations.append(int(lines[i]))
        elif field == 3:
            convergence_rates.append(float(lines[i]))
        elif field == 4:
            goal_distances.append(float(lines[i]))

        field += 1

    result = {
        "exec_time": exec_time,
        "best_chromosomes": best_chromosomes,
        "fitness_scores": fitness_scores,
        "generations": generations,
        "convergence_rates": convergence_rates,
        "goal_distances": goal_distances
    }

    return result


def t_vs_g(exec_time, generation):
    """Execution time vs Generations"""
    f = open("execution_time_vs_generations.dat", "a")
    f.write("{0}, {1}\n".format(exec_time, generation))
    f.close()


def plot_scatter_graph(title, x_axis, y_axis, x_label, y_label):
    pyplot.scatter(x_axis, y_axis)

    pyplot.title(title)
    pyplot.xlabel(x_label)
    pyplot.ylabel(y_label)

    pyplot.show()


def plot_time_vs_generations():
    exec_times = []
    generations = []

    f = open("execution_time_vs_generations.dat", "r")
    lines = f.readlines()
    f.close()

    for line in lines:
        line = line.split(",")
        exec_times.append(float(line[0].strip()))
        generations.append(float(line[1].strip()))

    plot_scatter_graph(
        "Execution time vs Generations",
        generations,
        exec_times,
        "Generations",
        "Execution time (s)"
    )


def analyze_exec_times(exec_times, file_names):
    best = exec_times[0]
    worst = exec_times[0]
    mean = exec_times[0]
    median = exec_times[0]

    best_fp = file_names[0]
    worst_fp = file_names[0]
    median_fp = file_names[0]

    # obtain best, worst, mean and median
    index = 0
    for time in exec_times:
        if time < best:
            best = time
            best_fp = file_names[index]
        elif time > worst:
            worst = time
            worst_fp = file_names[index]
        mean += time
        index += 1

    mean = mean / exec_times.__len__()
    median = exec_times[exec_times.__len__() / 2]
    median_fp = file_names[exec_times.__len__() / 2]

    # output execution time summary to file
    f = open("summary-exec_time.dat", "w")
    f.write("best: {0}\n".format(best))
    f.write("worst: {0}\n".format(worst))
    f.write("mean: {0}\n".format(mean))
    f.write("median: {0}\n".format(median))

    f.write("best file: {0}\n".format(best_fp))
    f.write("worst file: {0}\n".format(worst_fp))
    f.write("median file: {0}\n".format(median_fp))
    f.close()

    result = {
        "best": best,
        "worst": worst,
        "mean": mean,
        "median": median,

        "best_file": best_fp,
        "worst_file": worst_fp,
        "median_file": median_fp
    }

    return result


def obtain_convergence_equations(exec_time_summary):
    # obtain file paths
    best_fp = exec_time_summary["best_file"]
    worst_fp = exec_time_summary["worst_file"]
    median_fp = exec_time_summary["median_file"]

    # scrape the logs
    best = scrape_log_file(join(data_dir, best_fp))
    worst = scrape_log_file(join(data_dir, worst_fp))
    median = scrape_log_file(join(data_dir, median_fp))

    scenarios = [best, worst, median]
    scenario_names = ["best", "worst", "median"]
    scenario_coefficients = []
    i = 0
    for scenario in scenarios:
        # fit curve
        coeffs = numpy.polyfit(
            scenario["generations"],
            scenario["goal_distances"],
            3
        )
        scenario_coefficients.append(coeffs)

        # plot original and line fit
        pyplot.plot(
            scenario["generations"],
            scenario["goal_distances"],
            label=scenario_names[i]
        )

        i += 1

    # plot details
    pyplot.title("Goal Distance vs Generation")
    pyplot.xlabel("Generation")
    pyplot.ylabel("Goal Distance")
    pyplot.legend()
    pyplot.show()

    result = {
        "best_coefficients": scenario_coefficients[0],
        "worst_coefficients": scenario_coefficients[1],
        "median_coefficients": scenario_coefficients[2]
    }

    return result


def analyze_log_files(data_dir):
    exec_times = []
    file_names = []

    # obtain file list
    file_list = [f for f in listdir(data_dir) if isfile(join(data_dir, f))]
    log_files = []
    for f in file_list:
        if re.match(data_file_pattern, f):
            log_files.append(f)
    log_files.sort()

    # loop through files
    for log_file in log_files:
        print("Processing file [{0}]".format(log_file))
        result = scrape_log_file(join(data_dir, log_file))

        file_names.append(log_file)
        exec_times.append(float(result["exec_time"]))

        t_vs_g(result["exec_time"], result["generations"][-1])

    exec_time_summary = analyze_exec_times(exec_times, file_names)
    obtain_convergence_equations(exec_time_summary)


def analyze_log_file(data_dir):
    exec_times = []
    file_names = []

    # obtain file list
    file_list = [f for f in listdir(data_dir) if isfile(join(data_dir, f))]
    log_files = []
    for f in file_list:
        if re.match(data_file_pattern, f):
            log_files.append(f)
    log_files.sort()

    # loop through files
    log_file = log_files[0]
    print("Processing file [{0}]".format(log_file))
    result = scrape_log_file(join(data_dir, log_file))
    print result["generations"].__len__()
    print result["goal_distances"].__len__()
    plot_scatter_graph(
        "Goal Distances vs Generations",
        result["generations"][1:],
        result["goal_distances"],
        "Generation",
        "Goal Distance"
    )

if __name__ == '__main__':
    print("Analyzing experiment log files")
    # analyze_log_files(data_dir)
    analyze_log_file(data_dir)
    # plot_time_vs_generations()
