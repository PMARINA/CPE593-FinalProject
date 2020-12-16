"""
Notes:
1)  Download the data as specified in the Input section.
2)  Some files may have parameters missing or new ways of expressing information.
    You may have to change the regex's if this happens, or otherwise edit the
    input-files to have the parameters (I know a mass is missing in at least
    one of the planets' data files).
3)  Set the constants correctly to match your application, as specified in 
3)  `python3 parse_horizons.py`
4)  `Reformatted_Input.txt` is the output file.
"""

"""
Credits:
    Written by Pridhvi Myneni (PMARINA) 2020.12.12, Version 1.0
"""

"""
Input:
https://ssd.jpl.nasa.gov/horizons.cgi
Ephemeris Type: VECTORS
Target Body: ... Planets
Coordinate Origin: Solar System Barycenter (SSB) [500@0]
Time Span: Mostly irrelevant -- just pick the same for each file you read in
Table Settings: Quanitities Code = 2
Display/Output: Download/save (plain text file)
"""

"""
Output Format:
https://docs.google.com/spreadsheets/d/10y0553157L4t-43h3ratr3Er87rziJQ7Q7Md4NY2gVQ/edit?usp=sharing
"""

from os.path import exists
from typing import List, Union
import json as storage_class
from re import match, findall, DOTALL, IGNORECASE
from glob import glob as find_files
from datetime import datetime, timedelta, timezone
from math import sqrt

#### Constants ########

END_TIMESTAMP = 1608336000
G_CONSTANT = 6.6743015e-11
COMMON_GRAPHICS_RADIUS = 1
TOLERANCE = 1
TIMESTEP = 100  # -1 for adaptive...

# Visual/Debugging Parameters:
# Note that with these disabled, there will be no output...
# Also, if you do not have loguru installed, you will not see any debugging
# To install, `pip3 install loguru`
LOGGING_ENABLED = True
LOGGING_SUCCESS = True

# PHYSICAL CONSTANTS:
# Danger: changing these may result in inaccurate simulation data processing.
# Meters <-> Astronomical Units
_METERS_IN_AU = 1.496e11
AU_TO_METERS = lambda x: x * _METERS_IN_AU
METERS_TO_AU = lambda x: x / _METERS_IN_AU
# kilometers <-> meters
_METERS_IN_KILOMETER = 1000
METERS_TO_KILOMETERS = lambda x: x / _METERS_IN_KILOMETER
KILOMETERS_TO_METERS = lambda x: x * _METERS_IN_KILOMETER

# Solar Masses <-> Kilograms
_KG_IN_SOL_MASS = 1.98847e30
SOL_MASS_TO_KG = lambda x: x * _KG_IN_SOL_MASS
KG_TO_SOL_MASS = lambda x: x / _KG_IN_SOL_MASS

# Days <-> seconds
_SECONDS_IN_DAY = 86400
SECONDS_TO_DAYS = lambda x: x / _SECONDS_IN_DAY
DAYS_TO_SECONDS = lambda x: x * _SECONDS_IN_DAY
#### End Constants ####

# pip3 install loguru
try:
    from loguru import logger
except ImportError:
    LOGGING_ENABLED = False
    LOGGING_SUCCESS = False


def get_obj_name(lines: str) -> str:
    matches: List[str] = findall(r"(?<=\*\n).*\s{3}", lines)
    line_with_name = matches[0].strip()
    matches = findall(r"\s{3}\S+?\s{3}", line_with_name)
    object_name = matches[0].strip()
    if LOGGING_ENABLED:
        logger.debug(f"Obj Name: {object_name}")
    return object_name


def get_obj_radius(lines: str) -> float:
    # ex Equ. radius, km          = 6378.137
    line_items = findall(r"radius.*?=.*?\S+?\s", lines, flags=IGNORECASE)
    obj_radius_str: str = line_items[0].split("=")[1].strip()  # Get rid of "= "
    obj_radius_str: str = obj_radius_str.split("+-")[0].strip()
    obj_radius: float = float(obj_radius_str)
    if LOGGING_ENABLED:
        logger.debug(f"Obj Radius: {obj_radius} km")
    obj_radius_standardized_units = KILOMETERS_TO_METERS(obj_radius)
    return obj_radius_standardized_units


# TODO: Come up with a standard for rotation: not currently being used so ignoring.
def get_rot_rate(lines: str) -> float:
    # ex Rot. Rate (rad/s)        = 0.00007292115
    line_items: List[str] = findall(r"Rot.*?.*?=.+?\n", lines, flags=IGNORECASE)
    rot_rate_str: str = (
        line_items[0].split("=")[-1].strip().split(" ")[0].strip()
    )  # Get rid of "= "
    rot_rate: float = float(rot_rate_str)
    if LOGGING_ENABLED:
        logger.debug(f"Obj Rotation Rate: {rot_rate} rad/s")
    return rot_rate


# TODO: Standardize rotation units.
def get_obliquity_to_orbit(lines: str) -> float:
    # ex Obliquity to orbit, deg  = 23.4392911
    lines = lines.replace("'", "")
    line_items: List[str] = findall(r"(?<=Obliquity).*?=.*?\S+?\s", lines)
    obliquity_str: str = line_items[0].split("=")[1].strip()  # Get rid of "= "
    obliquity: float = float(obliquity_str)
    if LOGGING_ENABLED:
        logger.debug(f"Obj Obliquity to Orbit: {obliquity} deg")
    return obliquity


def get_mass(lines: str) -> float:
    # ex Mass x10^24 (kg)= 5.97219+-0.0006
    line_items = findall(r"Mass.*?=.+?\n", lines)
    mass_str: str = line_items[0]
    mass_items: List[str] = mass_str.split("=")
    base_mass_str: str = mass_items[1].strip().split(" ")[0].split("+-")[0].strip()
    base_mass_str = base_mass_str.replace("~", "").strip()
    multiplier_str = mass_items[0].strip()
    mul_base = 0.0
    try:
        mul_base: float = float(findall(r"(?<=x).*(?=\^)", multiplier_str)[0].strip())
    except IndexError:
        mul_base = 10
    mul_pow: float = float(findall(r"(?<=\^).*?\s", multiplier_str)[0].strip())
    multiplier: float = pow(mul_base, mul_pow)
    mass_kg: float = float(base_mass_str) * multiplier
    if LOGGING_ENABLED:
        logger.debug(f"Obj Mass: {mass_kg} kg")
    return mass_kg


def get_posix_timestamp(nasa_time_str: str) -> int:
    nasa_time_str = findall(r"(?<=A\.D\.).*(?=TDB)", nasa_time_str)[0].strip()
    nasa_time_components = nasa_time_str.split(".")
    feed_time_str = nasa_time_components[0].strip().lower()
    dt = datetime.strptime(feed_time_str, "%Y-%b-%d %H:%M:%S")
    dt = dt + timedelta(microseconds=int(nasa_time_components[1].strip()))
    time_stamp = dt.replace(tzinfo=timezone.utc).timestamp()
    time_stamp = int(time_stamp)
    if LOGGING_ENABLED:
        logger.debug(f"Timestamp: {time_stamp}")
    return time_stamp


def pos_vel_intermediate(xyzstr: str, preface="") -> List[float]:
    position = []
    for dim in ["X", "Y", "Z"]:
        dim = f"{preface}{dim}"
        s = xyzstr[xyzstr.find(dim) :] + "\n"
        coord = findall(r"(?<==).+?\s", s)[0].strip()
        if preface == "":
            coord = AU_TO_METERS(float(coord.strip()))
        elif preface == "V":
            coord = AU_TO_METERS(float(coord.strip())) / _SECONDS_IN_DAY
        position.append(str(coord))
    return position


def get_position_data(position_data: str):
    pos = pos_vel_intermediate(position_data)
    if LOGGING_ENABLED:
        logger.debug(f"Position: {pos}")
    return pos


def get_velocity_data(vel_data: str):
    vel = pos_vel_intermediate(vel_data, "V")
    if LOGGING_ENABLED:
        logger.debug(f"Velocity: {vel}")
    return vel


def get_time_pos_vel(lines: str) -> List[Union[int, List[float]]]:
    data = findall(r"(?<=\$\$SOE).*(?=\$\$EOE)", lines, flags=DOTALL)[0].strip()
    data = data.split("\n")[0:3]
    time_data = data[0]
    position_data = data[1]
    vel_data = data[2]
    ts = get_posix_timestamp(time_data)
    position_data = get_position_data(position_data)
    velocity_data = get_velocity_data(vel_data)
    return ts, position_data, velocity_data


def process_file(filepath: str, counter: int = -1) -> None:
    if LOGGING_ENABLED:
        logger.debug(f"About to parse: {filepath}")
    lines: List[str] = []
    # Read in the input file
    with open(filepath) as f:
        lines = f.readlines()
    concatted_lines = "".join(lines)
    obj_name = get_obj_name(concatted_lines)
    obj_radius = get_obj_radius(concatted_lines)
    graphics_radius = COMMON_GRAPHICS_RADIUS
    # Or can make graphics_radius a scaled version of the obj radius
    rot_rate = get_rot_rate(concatted_lines)
    obliquity_to_orbit = get_obliquity_to_orbit(concatted_lines)
    mass = get_mass(concatted_lines)
    time_pos_vel_arr = get_time_pos_vel(concatted_lines)
    data = [
        obj_name,
        obj_radius,
        graphics_radius,
        rot_rate,
        obliquity_to_orbit,
        mass,
        time_pos_vel_arr,
    ]
    with open(f"{obj_name}.json", "w") as f:
        storage_class.dump(data, f)
    if LOGGING_SUCCESS:
        logger.success(f"Parsed: {filepath}")


def generate_program_input():
    with open(f"Reformatted_Input.txt", "w") as out:
        files_to_process = find_files("*.json")
        bodies = []
        for file in files_to_process:
            data = None
            with open(file, "r") as f:
                data = storage_class.load(f)
            bodies.append(data)
        # Reverse = Descending...
        # We sort the bodies by their distance from the center (0,0,0)
        bodies.sort(
            key=lambda l: sqrt(sum([pow(float(x), 2) for x in l[-1][1]])), reverse=False
        )
        first_object = True
        global_timestamp = None
        counter = 0
        for body in bodies:
            body_timestamp = body[-1][0]
            if first_object:
                first_object = False
                global_timestamp = body_timestamp
                out.write(f"{global_timestamp}\n")
                out.write(f"{END_TIMESTAMP}\n")
                out.write(f"{G_CONSTANT}\n")
                out.write(f"{TOLERANCE}\n")
                out.write(f"{TIMESTEP}\n")
            else:
                if body_timestamp != global_timestamp:
                    error_msg = f"Timestamps were not matching... originally parsing with {global_timestamp} and now see {body_timestamp}"
                    if LOGGING_SUCCESS:
                        logger.critical(error_msg)
                    else:
                        raise ValueError(error_msg)
                    exit(1)
            pos_str = ",".join(body[-1][1])
            vel_str = ",".join(body[-1][2])
            data = f"{body[0]},{body[2]},{body[1]},{body[3]},{body[4]},{body[5]},{pos_str},{vel_str}"
            out.write(f"{counter},{data}\n")
            counter += 1


# @logger.catch
def main() -> None:
    input_files: List[str] = find_files("horizons_results*.txt")
    if LOGGING_SUCCESS:
        logger.info(f"Found {len(input_files)} files to parse.")
    for file in input_files:
        process_file(file)
    generate_program_input()


if __name__ == "__main__":
    main()
