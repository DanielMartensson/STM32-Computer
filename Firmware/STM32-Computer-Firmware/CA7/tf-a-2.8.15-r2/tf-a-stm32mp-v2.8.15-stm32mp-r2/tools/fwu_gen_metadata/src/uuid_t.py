# Copyright (c) 2022, STMicroelectronics - All Rights Reserved
#
# SPDX-License-Identifier: BSD-3-Clause

import uuid
from ctypes import Structure, c_uint32, c_uint16, c_uint8, Array

from src.utils import uint8array_to_number, number_to_uint8array
from src.utils import print_and_assert

UUID_NODE_LEN = 6
UUID_STR_LEN = 24 + (UUID_NODE_LEN*2)

# UUIDS

class uuid_t(Structure):
    """
        C-style struct for the UUID data
    """
    _fields_ = [
        ("time_low", c_uint32),
        ("time_mid", c_uint16),
        ("time_hi_and_version", c_uint16),
        ("clock_seq_hi_and_reserved", c_uint8),
        ("clock_seq_low", c_uint8),
        ("node", c_uint8 * UUID_NODE_LEN)
    ]

    def to_uuid_class(self):
        """
            Helper function to convert the Python / C struct into the class
                UUID from the library uuid, much easier to use.
        """
        return uuid.UUID(fields=(
            self.time_low, self.time_mid, self.time_hi_and_version,
            self.clock_seq_hi_and_reserved, self.clock_seq_low,
            uint8array_to_number(self.node)))

    def __str__(self):
        """ Display this class as a normalized UUID string """
        return self.to_uuid_class().__str__()

def uuid_t_from_str(uuid_str):
    """
        Generate a Python / C-struct uuid_t class from a normalized uuid string
    """
    if len(uuid_str) != UUID_STR_LEN:
        raise Exception("Wrong formatted UUID str: {}".format(uuid_str))
    obj = uuid.UUID(uuid_str)
    return uuid_t(
        obj.time_low,
        obj.time_mid,
        obj.time_hi_version,
        obj.clock_seq_hi_variant,
        obj.clock_seq_low,
        number_to_uint8array(obj.node, UUID_NODE_LEN))

def get_uuids(uuids, key):
    """
        Get the UUID of a certain key, generate a new one if none is
        found for the key
    """
    # TODO   Is there a context in which auto-generation of missing UUID
    #           is not desirable ?
    if key not in uuids.keys():
        uuids[key] = str(uuid.uuid4())
    return uuids[key]

def validate_uuid(uuid_str):
    try:
        uuid.UUID(uuid_str)
        return True
    except ValueError:
        return False

def test_uuids(args):
    uuid_str = "737d3401-64c1-4584-a40d-29f2238c82cf"
    uuid_t = uuid_t_from_str(uuid_str)
    print_and_assert(str(uuid_t), uuid_str)
