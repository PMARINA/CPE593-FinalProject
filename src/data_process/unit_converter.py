# To simplify manual testing

import visualize
from loguru import logger


def main():
    sim_output_strings = visualize.read_sim_output()
    grouped_coordinates = visualize.parse_output_strings(sim_output_strings)
    logger.debug(f"We have {len(grouped_coordinates)} timesteps")
    logger.debug(f"Each of which has {len(grouped_coordinates[-1])} bodies")
    logger.debug(f"Each of which has {len(grouped_coordinates[-1][-1])} coordinates per point")
    logger.debug("Cutting out all but the first coordinate")
    grouped_coordinates = grouped_coordinates[-1]
    planet_order = [
        "sun",
        "mercury",
        "venus",
        "earth",
        "moon",
        "mars",
        "jupiter",
        "Saturn",
        "uranus",
        "neptune",
    ]
    reduced_debugging_order = ["sun", "earth", "moon"]
    planet_order = reduced_debugging_order
    for i in range(len(grouped_coordinates)):
        s = [0, 0, 0]
        s[i] = [x / visualize._METERS_IN_AU for x in grouped_coordinates[i]]
        logger.debug(f"Found {planet_order[i]} at {s[i]}")
        logger.debug(f"Corresponding entry in input file should be {grouped_coordinates[i]}")
        input()


if __name__ == "__main__":
    main()
