#!/usr/bin/env python
from os import listdir
from os.path import isfile
from os.path import join

import matplotlib.pyplot as pyplot

# settings
data_dir = "log_data"


def scrape_log_file(log_file):
    exec_time = 0
    best_chromosomes = []
    fitness_scores = []
    generations = []
    convergence_rates = []
    goal_distances = []

    log = open(log_file, "r")
    lines = log.readlines()
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


def analyze_exec_times(exec_times):
    best = exec_times[0]
    worst = exec_times[0]
    mean = exec_times[0]
    median = exec_times[0]

    # obtain best, worst, mean and median
    for time in exec_times:
        if time < best:
            best = time
        elif time > worst:
            worst = time
        mean += time
    mean = mean / exec_times.__len__()
    median = exec_times[exec_times.__len__() / 2]

    # output execution time summary to file
    f = open("summary-exec_time.dat", "w")
    f.write("best: {0}\n".format(best))
    f.write("worst: {0}\n".format(worst))
    f.write("mean: {0}\n".format(mean))
    f.write("median: {0}\n".format(median))
    f.close()

    return {
        "best": best,
        "worst": worst,
        "mean": mean,
        "median": median
    }


def analyze_log_files(data_dir):
    exec_times = []

    # obtain file list
    log_files = [f for f in listdir(data_dir) if isfile(join(data_dir, f))]
    log_files.sort()
    log_files = log_files[0:9]

    # loop through files
    for log_file in log_files:
        print("Processing file [{0}]".format(log_file))
        result = scrape_log_file(join(data_dir, log_file))

        exec_times.append(float(result["exec_time"]))
        t_vs_g(result["exec_time"], result["generations"][-1])

    # plot_scatter_graph(
    #     "Goal distance vs Generations",
    #     result["generations"],
    #     result["goal_distances"],
    #     "Generations",
    #     "Goal Distance"
    # )

    plot_time_vs_generations()
    # print analyze_exec_times(exec_times)


if __name__ == '__main__':
    print("Analyzing experiment log files")
    analyze_log_files(data_dir)
