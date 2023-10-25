import os

path = os.getcwd()

target_symbol = "DOLF20"
LINE_LENGTH = 230
files_info = [
    {
        "path": "cpa/OFER_CPA_BMF_20191220_00.txt",
        "line_number": 930978,
        "type": "CPA"
    },
    {
        "path": "vda/OFER_VDA_BMF_20191220_00.txt",
        "line_number": 746865,
        "type": "VDA"
    }
]

def jump_to_line(file_path, line_number, line_length):
    raw_data = []
    
    with open(file_path, 'rb') as file:
        # Calculate the byte offset for the desired line
        # 55 = Header line
        target_offset = 55 + (line_number - 1) * line_length

        # Move the file pointer to the desired line
        file.seek(target_offset)

        for line in file:
            # Read and return the line
            line =  line.decode('utf-8')
            symbol = line.split(";")[1].strip()

            if symbol != target_symbol:
                break
            priority_time = line.split(";")[6]
            priority_indicator = line.split(";")[7]
            priority_time = priority_time.replace(":", "").replace(".", "")
            raw_data.append({"raw": line, "priority_time": int(priority_time), "priority_indicator": int(priority_indicator)})

        return raw_data

raw_data = []

for f in files_info:
    file_path = path + "/data/b3/20191220/raw/" + f["path"]
    line_number = f["line_number"]
    raw_data = raw_data + jump_to_line(file_path, line_number, LINE_LENGTH)

sorted_data = sorted(raw_data, key=lambda x: (x["priority_time"], x["priority_indicator"]), reverse=False)

output_file_path =  path + "/data/b3/20191220/raw/" + "{}_SORTED.txt".format(target_symbol)
    
with open(output_file_path, 'w') as file:
    for entry in sorted_data:
        file.write(f'{entry["raw"]}')

