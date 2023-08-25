import subprocess
import platform
import os
import sys
from termcolor import colored

model_dir = "Assets/models"
output_dir = "Output/models"
executable_path = './Tools/model'
use_shell = False
recompile = False

if '-r' in sys.argv:
    recompile = True

if platform.system() == 'Windows':
    executable_path += '.exe'
elif platform.system() == 'Linux':
    use_shell = True

executable_last_compile = os.path.getmtime(executable_path)

def compilemodel(model_path, output_path):
    command = (
        executable_path + ' ' +
        model_path + ' ' +
        output_path + ' ' +
        'noprompt'
    )
    print(colored("Compiling model %s..." % os.path.splitext(model_path)[0], "yellow"))
    print(command)
    result = subprocess.run(command, shell = use_shell)
    print()

files = os.listdir(model_dir)
for fname in files:
    model_path = os.path.join(model_dir, fname)
    output_path = os.path.join(output_dir, os.path.splitext(fname)[0] + '.jmd')

    should_compile = False
    if os.path.exists(output_path):
        asset_write_time = os.path.getmtime(model_path)
        output_write_time = os.path.getmtime(output_path)

        if asset_write_time > output_write_time or executable_last_compile > output_write_time:
            should_compile = True 

    if should_compile or recompile:
        compilemodel(model_path, output_path)
