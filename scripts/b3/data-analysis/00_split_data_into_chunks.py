import os

path = os.getcwd()

cpa_file = path + "/data/b3/20191220/raw/OFER_CPA_BMF_20191220.TXT"
vda_file = path + "/data/b3/20191220/raw/OFER_VDA_BMF_20191220.TXT"
neg_file = path + "/data/b3/20191220/raw/NEG_BMF_20191220.TXT"

cpa_out_path =  path + "/data/b3/20191220/raw/cpa"
vda_out_path =  path + "/data/b3/20191220/raw/vda"
neg_out_path =  path + "/data/b3/20191220/raw/neg"

cmds = [
    "split -l 2300000 -d {} {}/OFER_CPA_BMF_20191220_ --additional-suffix=.txt".format(cpa_file, cpa_out_path),
    "split -l 2300000 -d {} {}/OFER_VDA_BMF_20191220_ --additional-suffix=.txt".format(vda_file, vda_out_path),
    "split -l 2300000 -d {} {}/NEG_BMF_20191220_ --additional-suffix=.txt".format(neg_file, neg_out_path),
]

for cmd in cmds:
    os.system(cmd)