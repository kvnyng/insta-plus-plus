import os
import io
import sys
import ffmpeg
import subprocess

pipe_path = "/home/kevinhy/mit/pipe"

try:
    os.mkfifo(pipe_path)
    print(f"Created named pipe @ {pipe_path}")
except FileExistsError:
    print(f"Named pipe already exists @ {pipe_path}")
except OSError as e:
    print(f"Named pipe creation at @ {pipe_path} failed because {e}")

print("Entering while loop. Beginning to read...")

width, height = 720, 360

buffers = [[]]
buff_counter = 0
with io.open(pipe_path, "rb") as pipe:
    for i in range(1000):
        read = pipe.read(1)
        if read == b'~':
            print("READ IS ~")
            buffers.append([])
            buff_counter += 1
        else:
            buffers[buff_counter] += read
        sys.stdout.buffer.write(read)
        sys.stdout.write(" | ")
        # print(f"{read} | {number}")
        # if number == 10:
        #     print("STApRT/END")
    for buffer in buffers:
        if len(buffer) != 0:
            byte_array = bytes(buffer)
            array_value = int.from_bytes(byte_array, byteorder="little")
            print(f"Buffer: {buffer} | Value: {array_value}")
        #     string = ""
        #     for number in item:
        #         print(number)
        #         hex = chr(number)
        #         string += chr(number)
        #         print(hex)
        #     truevalue = int.from_byes(string, byteorder="big")
        #     print(truevalue)
    print("END OF READING")