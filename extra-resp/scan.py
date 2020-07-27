# !/usr/bin/pythn3.6

# any python greater or eqal 3.6

import subprocess as sp
import shlex
import os

window = 180
hopsize = 30

dir_name = os.getcwd()

# exe_name = "exe-ostd"
exe_name = "exe-mibw"
# data_name = "_Ada_1A_007Sel_U.txt"
data_name = "_Ada_1A_022Sel_U.txt"

exe_path = os.path.join(dir_name, exe_name)
data_path = os.path.join(dir_name, data_name)

with open(data_path, "r") as fd:
    lines = fd.readlines()
length = len(lines) - 1
_, _, str_delta = lines[0].split()
duration = length * float(str_delta)
del lines

offset = 0
collector = []
while offset < duration - window:
    cmd = f"{exe_path} {data_path} {offset} {window}"
    offset += hopsize

    
    cmdlist = shlex.split(cmd)
    out = sp.check_output(cmdlist)
    outstr = out.decode("utf-8")

    for line in outstr.split("\n"):
        if not "Final" in line and not "final" in line: continue
        _, _, _, result = line.split("\t")
        _, value, _ = result.split()
        collector.append(float(value))
assert collector
        
denominator = len(collector)
for n, item in enumerate(sorted(collector)):
    print(float(n) / denominator, item)
