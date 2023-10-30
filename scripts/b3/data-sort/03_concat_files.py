import os

path = os.getcwd()

target_symbol = "WING20"

target_data_path = path + "/data/b3/20191220/raw/tmp/{}".format(target_symbol)

file_list = sorted([f for f in os.listdir(target_data_path) if os.path.isfile(os.path.join(target_data_path, f))])

filenames = ['file1.txt', 'file2.txt', ...]
output_file_path = path + "/data/b3/20191220/sorted/" + target_symbol + "_SORTED.txt"

with open(output_file_path, 'w') as outfile:
    for fname in file_list:
        file_path = target_data_path + "/" + fname
        with open(file_path) as infile:
            for line in infile:
                outfile.write(line)