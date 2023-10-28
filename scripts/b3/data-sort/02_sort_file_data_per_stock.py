import os

path = os.getcwd()

target_symbol = "WING20"

target_data_path = path + "/data/b3/20191220/raw/tmp/{}".format(target_symbol)

file_list = [f for f in os.listdir(target_data_path) if os.path.isfile(os.path.join(target_data_path, f))]

for filename in file_list:
    raw_data = []

    filepath = target_data_path + "/" + filename

    with open(filepath, 'rb') as file:
        for line in file:
            line =  line.decode('utf-8')
            priority_time = line.split(";")[6].replace(":", "").replace(".", "")
            priority_indicator = line.split(";")[7]
            raw_data.append({"raw": line, "priority_time": int(priority_time), "priority_indicator": int(priority_indicator)})

    sorted_data = sorted(raw_data, key=lambda x: (x["priority_time"], x["priority_indicator"]), reverse=False)

    output_file_path =  target_data_path + "/" + filename.replace(".txt", "_SORTED.txt")
    
    with open(output_file_path, 'w') as file:
        for entry in sorted_data:
            file.write(f'{entry["raw"]}')

    print("Execution ends for file {}".format(output_file_path))
