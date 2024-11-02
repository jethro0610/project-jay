import subprocess
import os
from termcolor import colored

input_dir = './Assets/landmaps/'
output_dir = './Output/landmaps/'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

def compile_landmap(input_path, landmap_path, additive_path):
    landmap_command = 'magick ' + input_path + ' -flatten -resize 4096x4096 -channel B -separate' + ' GRAY:' + landmap_path 
    additive_command = 'magick ' + input_path + ' -flatten -resize 4096x4096 -channel R -separate' + ' GRAY:' + additive_path
    print(colored("Compiling landmap %s..." % input_path, "yellow"))
    subprocess.run(landmap_command, shell = True)
    subprocess.run(additive_command, shell = True)

files = os.listdir(input_dir)
for fname in files:
    input_path = os.path.join(input_dir, fname)
    landmap_path = os.path.join(output_dir, os.path.splitext(fname)[0] + '.lmp')
    additive_path = os.path.join(output_dir, os.path.splitext(fname)[0] + '.amp')

    if not os.path.exists(landmap_path) or os.path.getmtime(input_path) > os.path.getmtime(landmap_path):
        compile_landmap(input_path, landmap_path, additive_path)
