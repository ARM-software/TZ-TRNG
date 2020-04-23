'''
TRNG First iteration input data verifier

Copyright (C) Arm Limited, 2020 All rights reserved.

The example code is provided to You as an aid to learning when working with Arm-based technology,
including but not limited to programming tutorials. Arm hereby grants to you, subject to the terms
and conditions of this Licence, a non-exclusive, non-transferable, non-sub-licensable, free-of-charge
licence, to use and copy the Software solely for the purpose of demonstration and evaluation.

You accept that the Software has not been tested by Arm therefore the Software is provided “as is”,
without warranty of any kind, express or implied. In no event shall the authors or copyright holders
be liable for any claim, damages or other liability, whether in action or contract, tort or otherwise,
arising from, out of or in connection with the Software or the use of Software.
'''


import argparse
import glob
import itertools
import os
import re
import struct

from io import open


class MissingDataFilesException(Exception):
    pass


def setParser():
    parser = argparse.ArgumentParser(description = "TRNG Characterisation first iteration input data verifier.")
    parser.add_argument('-v', dest = "verbose", action = 'store_const', const = True, help = "Verbose mode")
    parser.add_argument('-f', dest="force", action='store_const', const=True, help="Force to check incomplete suite")
    parser.add_argument('path', action = 'store', default = './',
                        help = "Path to the *.bin files named according to the spec in TRNG App Note. Default is local directory.")
    return parser

def check_suite(binFiles):
    # Determine the SMP1, SMP2 and SMP3 from the file names
    if args.verbose:
        print("The given path contains %u .bin files." % len(binFiles))

    S = ["_S" + x for x in sorted(set(re.findall(r".*_R._S(.*)_V._T._C.*", "\n".join(binFiles))))]

    try:
        # Make sure found all sample counter values
        assert len(S) == 3, "Missing one or more set of sample count (got: %u)!\nPlease check path is correct." % len(S)

        opts = { 'oscLengths':      ['_R0', '_R1', '_R2', '_R3'],
                 'voltages':        ['_VH', '_VT', '_VL'],
                 'temperatures':    ['_TH', '_TT', '_TL'],
                 'processCorners':  ['_CT', '_CFF', '_CFS', '_CSF', '_CSS'],
                 'sampleCounters':  S }

        # Create product of all possible combination
        P = itertools.product(opts['oscLengths'], opts['sampleCounters'], opts['voltages'], opts['temperatures'], opts['processCorners'])

        # Check each one exists in the suite
        missing = []
        for x in P:
            pattern = "".join(x)
            if None == re.search(r".*" + pattern + ".bin", "\n".join(binFiles)):
                missing.append(pattern)

        if len(missing) > 0:
            raise MissingDataFilesException("Suite missing data files: " + str(len(missing)))

    except AssertionError as e:
        print("Error during verification of suite:")
        print("\t", e)
        raise e
    except MissingDataFilesException as e:
        if args.verbose:
            print(e)
        if not args.force:
            raise e

def unpackui(*arg):
    return struct.unpack(*arg)[0]

def check_format(path):
    if args.verbose:
        print("Opening: %s ..." % path)

    with open(path, "rb") as f:
        try:
            # Check header
            assert 0xAABBCCDD == unpackui(args.fmt, f.read(4)), "Header check mismatch"

            flags = unpackui(args.fmt , f.read(4))

            buffSize = flags & 0x00FFFFFF
            RoscLen  = flags & 0x03000000 >> 24
            TRNGMode = flags & 0x80000000 > 0

            sampleCnt = unpackui(args.fmt , f.read(4))

            assert sampleCnt == int(re.match(r".*_R._S(.*)_V._T._C.*", path).group(1)), "Sample counter value does not match the value in filename!"

            if args.verbose:
                print("\tBuffer size: 0x%x,  Rosc length: %u,  TRNG mode: %u  Sample counter value: %u" % (buffSize, RoscLen, TRNGMode, sampleCnt))

            # Check signature
            assert 0xAABBCCDD == unpackui(args.fmt, f.read(4)), "Signature check mismatch"

            # Skip random data; EHR size is 6 words and file must contain data multiples of EHR size
            f.seek(int((buffSize / 4 - 7) // 6) * 6 * 4, 1)

            # Check signature
            assert 0xDDCCBBAA == unpackui(args.fmt, f.read(4)), "Signature check mismatch"

            # Check error flag
            assert 0 == unpackui(args.fmt, f.read(4)), "Error flag(s) set"

            # Check signature
            assert 0xDDCCBBAA == unpackui(args.fmt, f.read(4)), "Signature check mismatch"

        except AssertionError as e:
            print("Error during verification of format of file: %s at location 0x%x:" % (f.name, f.tell() - 4))
            print("\t", e)
            raise e
        except TypeError as e:
            print("File name does not match naming requirement or sample count does not match: %s" % f.name)
            raise e
        
def get_endianness(bin_file):
    # Determine endianness based on magic word
    with open(bin_file, "rb") as f:
        magic_word = unpackui("<I", f.read(4))

    if 0xAABBCCDD == magic_word:
        if args.verbose:
            print("Input file endianness is: little")
        return "<I"         # Little-endian symbol to be used with unpack
    elif 0xDDCCBBAA == magic_word:
        if args.verbose:
            print("Input file endianness is: big")
        return ">I"         # Big-endian symbol to be used with unpack
    else:
        print("Error: unable to determine binary file endianness! Magic word: 0x%08X" % magic_word)
        raise AssertionError("Magic word does not match!")


if __name__ == "__main__":
    # Parse the optional input path
    args = setParser().parse_args()

    # Get the *.bin files list
    binFiles = glob.glob(args.path + "/*.bin")

    try:
        # Check suite is complete
        check_suite(binFiles)

        # Auto-detect endianness based on the first file
        args.fmt = get_endianness(binFiles[0])

        # Check individual files
        [check_format(fin) for fin in binFiles]
    except AssertionError as e:
        if args.verbose:
            # Assertion handled within functions too. Decide whether more info to be printed in case of verbose flag set
            pass
