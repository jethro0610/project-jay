import subprocess
import os
from termcolor import colored

input_dir = './Assets/blobs/'
output_dir = './Output/blobs/'

def compile_blob(input_path, output_path):
    command = 'convert -flatten -resize 1024x1024 ' + input_path + ' GRAY:' + output_path 
    print(colored("Compiling blob %s..." % input_path, "yellow"))
    subprocess.run(command, shell = True)

files = os.listdir(input_dir)
for fname in files:
    input_path = os.path.join(input_dir, fname)
    output_path = os.path.join(output_dir, os.path.splitext(fname)[0] + '.blb')

    if not os.path.exists(output_path) or os.path.getmtime(input_path) > os.path.getmtime(output_path):
        compile_blob(input_path, output_path)
