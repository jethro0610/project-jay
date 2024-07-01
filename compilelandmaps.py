import subprocess
import os
from termcolor import colored

input_dir = './Assets/landmaps/'
output_dir = './Output/landmaps/'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)


def compile_landmap(input_path, output_path):
    command = 'magick ' + input_path + ' -flatten -resize 1024x1024 ' + ' GRAY:' + output_path 
    print(colored("Compiling landmap %s..." % input_path, "yellow"))
    subprocess.run(command, shell = True)

files = os.listdir(input_dir)
for fname in files:
    input_path = os.path.join(input_dir, fname)
    output_path = os.path.join(output_dir, os.path.splitext(fname)[0] + '.lmp')

    if not os.path.exists(output_path) or os.path.getmtime(input_path) > os.path.getmtime(output_path):
        compile_landmap(input_path, output_path)
