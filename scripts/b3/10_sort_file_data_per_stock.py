import os
import matplotlib.pyplot as plt
import numpy as np

path = os.getcwd()

target_symbol = "DOLF20"

def jump_to_line(file_path, line_number, line_length):
    raw_data = []
    
    with open(file_path, 'rb') as file:
        # Calculate the byte offset for the desired line
        target_offset = (line_number - 1) * line_length

        # Move the file pointer to the desired line
        file.seek(target_offset)

        for line in file:
            # Read and return the line
            line =  line.decode('utf-8')
            symbol = line.split(";")[1].strip()

            if symbol != target_symbol:
                break
            priority = line.split(";")[6]
            priority = priority.replace(":", "").replace(".", "")
            raw_data.append({"raw": line, "priority": int(priority)})

        return raw_data

# Example usage
file_path = path + "/new_data/OFER_VDA_BMF_20191220_00.txt"
line_length = 230
line_number = 746865

raw_data = jump_to_line(file_path, line_number, line_length)

sorted_data = sorted(raw_data, key=lambda x: x["priority"])

file_path = path + "/new_data/OFER_VDA_BMF_20191220_00_SORTED.txt"
with open(file_path, 'w') as file:
    for entry in sorted_data:
        file.write(f'{entry["raw"]}')


