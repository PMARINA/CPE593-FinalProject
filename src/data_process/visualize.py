import math
import glob
import re
from tqdm import tqdm
from loguru import logger
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d

# import pygame

from parse_horizons import _SECONDS_IN_DAY, _METERS_IN_AU

SIMULATION_INPUT_FILE = "Reformatted_Input.txt"
FILE_TO_ANIMATE = "Reformatted_Input.txtOUTPUT"


def read_sim_output(file_name=FILE_TO_ANIMATE):
    """
    Reads in the simulation output

    Parameters:
    file_name (str): Path to the file to animate

    Returns:
    List[str]: All the lines in the file
    """
    lines = []
    with open(FILE_TO_ANIMATE) as f:
        lines = f.readlines()
    logger.success(f"Read in {len(lines)} lines of simulation output")
    return lines


def split_into_simulation_intervals(strings_in_file):
    simulation_intervals = []
    interval_start = 0
    interval_finished = False
    for i in tqdm(range(len(strings_in_file))):
        if strings_in_file[i].strip() == "":
            # Interval has finished
            simulation_intervals.append(strings_in_file[interval_start:i])
            interval_finished = True
        else:
            if interval_finished:
                # We have started a new interval, so reset the counter
                interval_start = i
                interval_finished = False
    logger.success(f"Found {len(simulation_intervals)} timesteps in simulation")
    return simulation_intervals


def parse_output_strings(strings_in_file):
    intervals = split_into_simulation_intervals(strings_in_file)
    get_coords = lambda line: [float(x) for x in line.split(":")[-1].split()]
    plottable_coordinates = []
    for interval in tqdm(intervals):
        plottable_coordinates.append(list(map(get_coords, interval)))
    return plottable_coordinates


def alert_deviation(last_coordinates, sim_in_file=SIMULATION_INPUT_FILE):
    last_coordinates = list(last_coordinates)
    logger.debug(f"Have {len(last_coordinates)} planets to evaluate")
    lines = []
    with open(sim_in_file) as f:
        lines = f.readlines()

    start_timestamp = int(lines[0])
    end_timestamp = int(lines[1])
    num_days = 1  # math.ceil((end_timestamp - start_timestamp) / _SECONDS_IN_DAY)+1

    sim_positions_planets = lines[5:]
    num_bodies_in_sim = len(sim_positions_planets)
    names_of_bodies = list(map(lambda line: line.split(",")[1], sim_positions_planets))

    body_dict = {}

    horizons_files = glob.glob("horizons_results*.txt")
    for filename in horizons_files:
        h_lines = []
        with open(filename) as f:
            h_lines = f.readlines()
        line_containing_name = h_lines[1]
        index_of_position = -1
        for i in range(len(names_of_bodies)):
            name = names_of_bodies[i]
            if name.strip().lower() in line_containing_name.lower():
                # logger.debug(f"Matched {name} to {line_containing_name}")
                index_of_position = i
                break
        matches = re.findall(r"\n.*?X.*?\n", "\n".join(h_lines))
        line_with_coords = matches[num_days * 2]
        body_dict[names_of_bodies[index_of_position]] = line_with_coords
    for i in range(len(last_coordinates)):
        planet = names_of_bodies[i]
        logger.debug(planet)
        logger.debug(f"NASA Had:")
        print(body_dict[planet])
        logger.debug(f"You had:")
        [print("{:e}".format(x / _METERS_IN_AU)) for x in last_coordinates[i]]
        input("Ok to Proceed? ")


def draw_planet(coords):
    xs = []
    ys = []
    zs = []
    for i in range(len(coords[0])):
        xs.append([])
        ys.append([])
        zs.append([])
    for interval in [coords[0], coords[-1]]:  # coords:
        for i in range(len(interval)):
            xs[i].append(interval[i][0] / _METERS_IN_AU)
            ys[i].append(interval[i][1] / _METERS_IN_AU)
            zs[i].append(interval[i][2] / _METERS_IN_AU)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    for i in range(len(coords[1])):
        if i == 0:
            ax.scatter(xs[i], ys[i], zs[i], color="yellow")
        elif i == 4:
            ax.scatter(xs[i], ys[i], zs[i], color="blue")
        elif i == 3:
            ax.scatter(xs[i], ys[i], zs[i], color="green")
        else:
            if i < 0.5 * len(coords[1]):
                ax.scatter(xs[i], ys[i], zs[i], color="black")
            else:
                ax.scatter(xs[i], ys[i], zs[i], color="orange")
    plt.show()


def main():
    sim_output_strings = read_sim_output()
    grouped_coordinates = parse_output_strings(sim_output_strings)
    logger.debug(f"We have {len(grouped_coordinates)} timesteps")
    logger.debug(f"Each of which has {len(grouped_coordinates[-1])} bodies")
    logger.debug(f"Each of which has {len(grouped_coordinates[-1][-1])} coordinates per point")
    alert_deviation(grouped_coordinates[0])
    draw_planet(grouped_coordinates)
    breakpoint


if __name__ == "__main__":
    main()
