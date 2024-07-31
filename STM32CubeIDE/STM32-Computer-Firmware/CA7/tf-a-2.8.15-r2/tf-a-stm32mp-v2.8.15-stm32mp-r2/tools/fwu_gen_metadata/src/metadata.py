# Copyright (c) 2022, STMicroelectronics - All Rights Reserved
#
# SPDX-License-Identifier: BSD-3-Clause

"""
    metadata.py
        Manage Python dict formatted fwu_metadata
"""
import uuid

def create_dummy_metadata(nb_fw_img, nb_fw_banks):
    """
        This function creates a Python dict containing dummy metadata
        Used for testing or for placeholding
    """
    data = {
        "metadata": {
            "version": 0,
            "active_index": 1,
            "previous_active_index": 0,
            "img_entry": {}
        },
        "uuids": {
            "locations": {
                "sda": str(uuid.uuid4()),
            },
            "entries": {},
        },
        "configs": {
            "nb_fw_img": nb_fw_img,
            "nb_fw_banks": nb_fw_banks
        }
    }

    entries = {}
    for nimg in range(nb_fw_img):
        banks = {}
        for nbnk in range(nb_fw_banks):
            entry_name = "img_" + str(nimg) + "_bank_" + str(nbnk)
            banks[entry_name] = {"accepted": True,
                                 "reserved": 0}
            data["uuids"]["entries"][entry_name] = str(uuid.uuid4())

        entries["img_" + str(nimg)] = {
            "location": "sda",
            "img_bank_info": banks
        }
        data["uuids"]["entries"]["img_" + str(nimg)] = str(uuid.uuid4())
    data["metadata"]["img_entry"] = entries
    return data

def __check_md(res, cond, msg):
    """
        Helper function used to validate metadata.
        If cond is false, return False, and display message
    """
    if not cond:
        print("Metadata validation failed: '{}'".format(msg))
        return False
    return res

def validate_metadata(metadata):
    """
        Validate content of a Python dict metadata
        Passes through all the data, display errors if any,
        then return if errors got encountered
    """
    res = True

    for key in ["metadata", "uuids", "configs"]:
        res = __check_md(res, key in metadata.keys(),
                         "Key missing: {}".format(key))
    for conf in ["nb_fw_img", "nb_fw_banks"]:
        res = __check_md(res, conf in metadata["configs"].keys(), "Config missing: {}".format(conf))

    res = __check_md(res, "locations" in metadata["uuids"].keys(), "UUID type location missing")
    res = __check_md(res, "entries" in metadata["uuids"].keys(), "UUID type entries missing")
    res = __check_md(res,
                     len(metadata["metadata"]["img_entry"].keys()) ==
                     metadata["configs"]["nb_fw_img"],
                     "Wrong number of firmware images entries")

    for iname, img in metadata["metadata"]["img_entry"].items():
        res = __check_md(res, iname in metadata["uuids"]["entries"].keys(),
                         "Image {} UUID not known".format(iname))

        res = __check_md(res,
                         img["location"] in metadata["uuids"]["locations"].keys(),
                         "Location {} UUID not known".format(img["location"]))

        res = __check_md(res,
                         len(metadata["metadata"]["img_entry"] [iname]["img_bank_info"].keys()) ==
                         metadata["configs"]["nb_fw_banks"],
                         "Wrong number of banks for image {}".format(iname))

        for bname, _ in metadata["metadata"]["img_entry"][iname]["img_bank_info"].items():
            res = __check_md(res, bname in metadata["uuids"]["entries"],
                             "Bank {} UUID not known".format(bname))
            res = __check_md(res, iname + "_bank_" in bname,
                             "Bank name wrongly named")

    return res
