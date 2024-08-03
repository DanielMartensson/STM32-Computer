# Copyright (c) 2022, STMicroelectronics - All Rights Reserved
#
# SPDX-License-Identifier: BSD-3-Clause

"""
    utils.py
        Useful functions for the whole project
"""
from ctypes import c_uint8

# Number manipulation
def uint8array_to_number(uint8array):
    """ Convert array of uint8_t into a single Python number """
    return sum([x << (i*8) for i, x in enumerate(reversed(uint8array))])

def number_to_uint8array(number, array_length):
    """ Convert a single Python number into an array of uint8_t """
    uint8array = c_uint8 * array_length
    array = [(number & (0xff << (n*8))) >> (n*8)
        for n in reversed(range(array_length))
    ]
    return uint8array(*array)

def test_number_to_array(args):
    """ Test back and forth conversion works for our use case"""
    numbers = [0x42053497, 0x45]
    for number in numbers:
        a = number_to_uint8array(number, 4)
        n = uint8array_to_number(a)
        print_and_assert(number, n)

# Default values creation
def get_template_location_key(locations, uuid):
    """
        Get the location key of the given location.
        If it doesn't exist, generate one from the UUID
    """
    if uuid in locations.values():
        return [key for key, val in locations.items() if val == uuid][0]
    else:
        key = "loc_" + uuid.replace("-", "_")
        locations[key] = uuid
        return key

def test_template_location_key(args):
    """ Test location key getter works well """
    print_and_assert(get_template_location_key({"sda": "uuid"}, "uuid"),
                     "sda")
    print_and_assert(get_template_location_key({"sda": "uuid"}, "new"),
                     "loc_new")
    print_and_assert(get_template_location_key({"sda": "uuid"}, "sda-2"),
                     "loc_sda_2")

# Testing utils
def print_and_assert(el1, el2):
    """ Print the two values to test, then assert their equality"""
    print(el1, "==", el2)
    assert el1 == el2

def test_utils(args):
    all_tests = {
        "Number to array": test_number_to_array,
        "Template location key": test_template_location_key
    }

    for (key, test) in all_tests.items():
        print("--- Test " + key)
        test(args)
        print("---\n")
