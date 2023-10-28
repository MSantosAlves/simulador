import os

path = os.getcwd() + "/data/b3/20191220/raw/sorted_temp/WING20"

target_symbol = "WING20"
LINE_LENGTH = 230
files_info = [
    {
        "path": "OFER_CPA_BMF_20191220.TXT",
        "line_number": 22110804
    },
    {
        "path": "OFER_VDA_BMF_20191220.TXT",
        "line_number": 13407308
    }
]

ranges = ["08:10", "10:12", "12:14", "14:16", "16:18", "18:20"]

filenames = []

for r in ranges:
    filenames.append("{}/OFER_BMF_{}_{}.txt".format(path, target_symbol, r))

def write_to_file(file_path, line_number, line_length):
    with open(filenames[0], 'r+') as f_range_08_10, open(filenames[1], 'r+') as f_range_10_12, open(filenames[2], 'r+') as f_range_12_14, open(filenames[3], 'r+') as f_range_14_16, open(filenames[4], 'r+') as f_range_16_18, open(filenames[5], 'r+') as f_range_18_20, open(file_path, 'rb') as data_file:

        # Calculate the byte offset for the desired line
        # 55 = Header line
        target_offset = 55 + (line_number - 1) * line_length

        # Move the file pointer to the desired line
        data_file.seek(target_offset)

        for line in data_file:

            # Read and return the line
            line =  line.decode('utf-8')
            symbol = line.split(";")[1].strip()

            if symbol != target_symbol:
                break

            priority_time = line.split(";")[6]
            hour = int(priority_time.split(":")[0])
            if hour < 10:
                f_range_08_10.write(line)
            elif hour < 12:
                f_range_10_12.write(line)
            elif hour < 14:
                f_range_12_14.write(line)
            elif hour < 16:
                f_range_14_16.write(line)
            elif hour < 18:
                f_range_16_18.write(line)
            else:
                f_range_18_20.write(line)

for f in files_info:
    file_path = os.getcwd() + "/data/b3/20191220/raw/" + f["path"]
    line_number = f["line_number"]
    write_to_file(file_path, line_number, LINE_LENGTH)

print("End execution succesfully")