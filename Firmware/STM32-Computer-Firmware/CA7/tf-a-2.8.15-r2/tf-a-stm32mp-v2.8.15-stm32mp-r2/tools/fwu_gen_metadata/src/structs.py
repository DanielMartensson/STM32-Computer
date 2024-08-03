# Copyright (c) 2022, STMicroelectronics - All Rights Reserved
#
# SPDX-License-Identifier: BSD-3-Clause

"""
    structs.py
        Manage C-style struct formatted fwu_metadata
"""

import binascii

from ctypes import Structure, c_uint32, c_uint16, c_uint8, Array

from src.uuid_t import uuid_t, get_uuids, uuid_t_from_str
from src.utils import get_template_location_key

# IMAGE BANK

class image_bank_info(Structure):
    """
        Image bank C struct
    """
    _fields_ = [
        ("img_uuid", uuid_t),
        ("accepted", c_uint32),
        ("reserved", c_uint32),
    ]

    def from_metadata(self, bank_name, bank, uuids):
        """
            Use Python-dict metadata to generate the Python / C-struct object
        """
        self.img_uuid = uuid_t_from_str(get_uuids(uuids["entries"], bank_name))
        self.accepted = bank["accepted"]
        self.reserved = bank.get("reserved", 0) #default value 0 if not in json

    def display(self, buff):
        """
            Helper function to render the data of the bank in a buffer
        """
        buff += "\t\t  Image UUID: {}\n".format(str(self.img_uuid))
        buff += "\t\t  Accepted: {}\n".format(self.accepted)
        buff += "\t\t  Reserved: {}\n".format(self.reserved)
        return buff

    def to_dict(self, bankname, bank, uuids):
        """
            Helper function to add the data contained into dicts
        """
        uuids["entries"][bankname] = str(self.img_uuid)
        bank["accepted"] = bool(self.accepted)
        bank["reserved"] = self.reserved

# IMAGE ENTRY
def get_struct_imgentry(nb_fw_banks):
    """
        As the structure depend on a variable parameter, the Python class is
        generated at runtime, allowing to not rely on global variables
    """
    class image_entry(Structure):
        """
            Image entry C struct
        """
        _fields_ = [
            ("img_type_uuid", uuid_t),
            ("location_uuid", uuid_t),
            ("img_bank_info", image_bank_info * nb_fw_banks),
        ]

        def from_metadata(self, img_name, entry, uuids):
            """
                Generate this Python / C-struct object from Python dict
                metadata
            """
            self.img_type_uuid = uuid_t_from_str(
                get_uuids(uuids["entries"], img_name))
            self.location_uuid = uuid_t_from_str(get_uuids(uuids["locations"], entry["location"]))
            for n, (bank_name, bank) in enumerate(entry["img_bank_info"].items()):
                self.img_bank_info[n].from_metadata(bank_name, bank, uuids)

        def display(self, buff):
            """
                Render the data contained inside a buffer
            """
            buff += "\t  Image type UUID: {}\n".format(str(self.img_type_uuid))
            buff += "\t  Location UUID: {}\n".format(str(self.location_uuid))
            buff += "\t  --- Banks ---\n"
            for n, bank in enumerate(self.img_bank_info):
                buff += "\t\tBank {}\n".format(n)
                buff = bank.display(buff)
            return buff

        def to_dict(self, imgname, entry, uuids, imgkeys, loc_uuids):
            """
                Helper function to add this struct's data into Python dict
                metadata
            """
            uuids["entries"][imgname] = str(self.img_type_uuid)
            location_uuid_str = str(self.location_uuid)
            loc_key = get_template_location_key(loc_uuids, location_uuid_str)
            entry["location"] = loc_key
            uuids["locations"][loc_key] = location_uuid_str
            banks = {}
            for b, bank in enumerate(self.img_bank_info):
                bname = imgname + "_bank_" + str(b)
                banks[bname] = {}
                bank.to_dict(bname, banks[bname], uuids)
            entry["img_bank_info"] = banks

    return image_entry

# FWU-Metadata struct creation
def generate_fwu_metadata(nb_fw_img, nb_fw_banks):
    """
        Generate an empty FWU-Metadata object from the given parameters
    """
    class fwu_metadata(Structure):
        """
            FWU Metadata C struct (parent struct)
        """
        _fields_ = [
            ("crc_32", c_uint32),
            ("version", c_uint32),
            ("active_index", c_uint32),
            ("previous_active_index", c_uint32),
            ("img_entry", get_struct_imgentry(nb_fw_banks) * nb_fw_img)
        ]

        def update_crc32(self):
            """
                Compute and update the CRC32 from the data contained
            """
            self.crc_32 = binascii.crc32(bytearray(self)[4:])

        def from_metadata(self, metadata, uuids):
            """
                Update the contained metadata from given Python dict
            """
            self.version = metadata["version"]
            self.active_index = metadata["active_index"]
            self.previous_active_index = metadata["previous_active_index"]
            for n, (img_name, img) in enumerate(metadata["img_entry"].items()):
                self.img_entry[n].from_metadata(img_name, img, uuids)
            self.update_crc32()

        def display(self):
            """
                Prints a text-rendered display of the data contained
            """
            buff = "METADATA\n"
            buff += "  CRC32: {}\n".format(self.crc_32)
            buff += "  Version: {}\n".format(self.version)
            buff += "  Active index: {}\n".format(self.active_index)
            buff += "  Previous active index: {}\n".format(
                self.previous_active_index)
            buff += "  --- Image entries ---\n"
            for n, img in enumerate(self.img_entry):
                buff += "\tEntry {}\n".format(n)
                buff = img.display(buff)
            print(buff)

        def to_dict(self, metadata, uuids, imgkeys, loc_uuids):
            """
                Pass the data contained in this object into Python dicts
            """
            metadata["crc_32"] = self.crc_32
            metadata["version"] = self.version
            metadata["active_index"] = self.active_index
            metadata["previous_active_index"] = self.previous_active_index
            entries = {}
            for n, img in enumerate(self.img_entry):
                iname = imgkeys[n]
                entries[iname] = {}
                img.to_dict(iname, entries[iname], uuids, imgkeys, loc_uuids)
            metadata["img_entry"] = entries

    return fwu_metadata()

def fwupd_from_dict(data):
    """
        Return a fwu_metadata object from a complete Python dict as saved in
        the JSON file
    """
    fwupd = generate_fwu_metadata(
        data["configs"]["nb_fw_img"],
        data["configs"]["nb_fw_banks"])
    fwupd.from_metadata(data["metadata"], data["uuids"])
    return fwupd

def fwupd_to_dict(fwupd, nb_fw_imgs, nb_banks, images_keys, loc_uuids):
    """
        Create a Python dict containing all the information contained in a
        fwu_metadata object
    """
    data = {"metadata": {}, "uuids": {"locations": {}, "entries": {}},
            "configs": {
                "nb_fw_img": nb_fw_imgs,
                "nb_fw_banks": nb_banks
            },
           }
    images_keys += ["img_" + str(n) for n in range(max(0, nb_fw_imgs - len(images_keys)))]

    fwupd.to_dict(data["metadata"], data["uuids"], images_keys, loc_uuids)
    return data

def __check_same(res, name, mdel, binel):
    """
        Helper function for binary metadata cross-validation with JSON

        If @mdel (JSON) and @binel (Binary) are not the same, return False
            and display a formatted error with the given @name.
        Else return @res
    """
    if str(mdel) != str(binel):
        print("Element '{}' are not the same in metadata and binary data".format(name))
        return False
    else:
        return res

def validate_fwupd_and_dict(md, fwupd):
    """
        Validation function for binary + JSON metadata
        Ensure the data of the both formats are the same

        Do not check CRC32
    """
    res = True

    # Check elements of fwu_metadata struct
    for key in ["version", "active_index", "previous_active_index"]:
        res = __check_same(res, key, md["metadata"][key], getattr(fwupd, key))

    # Check elements in each image entry
    for n, (img_name, img) in enumerate(md["metadata"]["img_entry"].items()):
        entry = fwupd.img_entry[n]

        res = __check_same(res, "Image {} type UUID".format(img_name),
                           md["uuids"]["entries"][img_name],
                           entry.img_type_uuid)

        res = __check_same(res, "Location UUID",
                           md["uuids"]["locations"][img["location"]],
                           entry.location_uuid)

        # Check elements in each banks of image entry
        for b, (bname, bank) in enumerate(img["img_bank_info"].items()):
            fwbank = entry.img_bank_info[b]
            res = __check_same(res, "Image {} Bank {} UUID".format(
                                                            img_name, b),
                               md["uuids"]["entries"][bname],
                               fwbank.img_uuid)

            res = __check_same(res, "Image {} Bank {} Accepted".format(
                                                                img_name, b),
                               bank["accepted"],
                               bool(fwbank.accepted))

            res = __check_same(res, "Image {} Bank {} Reserved field".format(
                                                                img_name, b),
                               bank["reserved"],
                               fwbank.reserved)
    return res
