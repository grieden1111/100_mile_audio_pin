#!/usr/bin/env python3
"""
Adapted from PaulStoffregen/SerialFlash's rawfile-uploader.py for Python 3
and for use with an Arduino Uno (instead of a Teensy).

Changes from the original:
 - Rewritten to work with `bytes`/`bytearray` throughout. The original
   used `for byte in f.read():` which only worked in Python 2, where
   each `byte` was a 1-character string. In Python 3, iterating over a
   `bytes` object yields integers instead -- that broke the comparisons
   against BYTE_START/BYTE_ESCAPE and the final string-join step.
 - Serial port is now opened at 115200 baud explicitly, matching the
   baud rate set in the adapted .ino sketch (the original Teensy script
   didn't need to care, since Teensy ignores the requested baud rate).
 - Default flash size changed to 8MB to match the W25Q64JV chip.
 - Added a short delay after opening the port -- opening a serial
   connection to an Arduino Uno resets it (via the DTR line), so we
   give it a moment to finish booting before sending any data.

Usage: python3 rawfile_uploader_py3.py <port> <file1.RAW> [file2.RAW ...]
"""

import serial
import sys
import os
import time

if len(sys.argv) <= 2:
    print(f"Usage: '{sys.argv[0]} <port> <files>' where:\n"
          f"\t<port> is the TTY USB port connected to the Arduino\n"
          f"\t<files> is a list of .RAW files (bash globs work).")
    sys.exit()

BYTE_START = 0x7e
BYTE_ESCAPE = 0x7d
BYTE_SEPARATOR = 0x7c

FLASH_SIZE_MB = 8  # W25Q64JV = 8MB; change if you use a different chip

port = sys.argv[1]
files = sys.argv[2:]

total_file_size = sum(os.path.getsize(f) for f in files)
flash_size_bytes = FLASH_SIZE_MB * 1024 * 1024
if total_file_size > flash_size_bytes:
    print(f"Too many files selected.\n"
          f"\tTotal flash size:\t{flash_size_bytes:>14,} bytes\n"
          f"\tTotal file size:\t{total_file_size:>14,} bytes")
    sys.exit()

ser = serial.Serial(port, 115200, timeout=5)
time.sleep(2)  # let the Arduino finish its reset/boot after the port opens

print(f"Uploading {len(files)} files...")

for i, filename in enumerate(files):
    start_time = time.time()
    print(f"{i}: {filename}", end="", flush=True)

    with open(filename, "rb") as f:
        data = f.read()

    file_length = len(data)
    base_name = os.path.basename(filename).encode("ascii")

    encoded = bytearray()

    # Start byte + filename + end-of-filename separator
    encoded.append(BYTE_START)
    encoded.extend(base_name)
    encoded.append(BYTE_SEPARATOR)

    # File length as 4 raw bytes (big-endian, matches the Arduino's
    # `fileSize = (fileSize << 8) + b` reconstruction), then separator
    encoded.append((file_length >> 24) & 0xFF)
    encoded.append((file_length >> 16) & 0xFF)
    encoded.append((file_length >> 8) & 0xFF)
    encoded.append((file_length >> 0) & 0xFF)
    encoded.append(BYTE_SEPARATOR)

    # Binary audio data, with escaping for any byte that collides
    # with a control byte value
    for b in data:
        if b == BYTE_START or b == BYTE_ESCAPE:
            encoded.append(BYTE_ESCAPE)
            encoded.append(b ^ 0x20)
        else:
            encoded.append(b)

    # End-of-file marker
    encoded.append(BYTE_START)

    ser.write(bytes(encoded))

    elapsed = time.time() - start_time
    kb_per_sec = (file_length / 1024) / elapsed if elapsed > 0 else 0
    print(f" ({kb_per_sec:.2f} KB/s)")

print("All files uploaded")
