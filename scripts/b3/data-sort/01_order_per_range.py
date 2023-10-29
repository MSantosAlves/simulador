import os

target_symbol = "WING20"
path = os.getcwd() + "/data/b3/20191220/raw/tmp/" + target_symbol

buy_file_path = os.getcwd() + "/data/b3/20191220/raw/OFER_CPA_BMF_20191220.TXT"
sell_file_path = os.getcwd() + "/data/b3/20191220/raw/OFER_VDA_BMF_20191220.TXT"

ranges = ["08:10", "10:12", "12:14", "14:16", "16:18", "18:20"]

filenames = []

for r in ranges:
    filenames.append("{}/OFER_BMF_{}_{}.txt".format(path, target_symbol, r))

with open(filenames[0], 'w+') as f_range_08_10, \
     open(filenames[1], 'w+') as f_range_10_12, \
     open(filenames[2], 'w+') as f_range_12_14, \
     open(filenames[3], 'w+') as f_range_14_16, \
     open(filenames[4], 'w+') as f_range_16_18, \
     open(filenames[5], 'w+') as f_range_18_20, \
     open(buy_file_path, 'rb') as buy_file, \
     open(sell_file_path, 'rb') as sell_file:
    
    for line in buy_file:
        # Read and return the line
        line =  line.decode('utf-8')

        if len(line) != 230:
            continue

        symbol = line.split(";")[1].strip()

        if symbol != target_symbol:
            continue

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

    for line in sell_file:
        # Read and return the line
        line = line.decode('utf-8')

        if len(line) != 230:
            continue

        symbol = line.split(";")[1].strip()

        if symbol != target_symbol:
            continue

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

print("End execution succesfully")