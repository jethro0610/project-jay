import subprocess
import platform
import os
import sys
from termcolor import colored

model_dir = "Assets/models"
output_dir = "Output/models"
executable_extension = ''
use_shell = False
recompile = False

if '-r' in sys.argv:
    recompile = True

if platform.system() == 'Windows':
    executable_extension = '.exe'
elif platform.system() == 'Linux':
    use_shell = True

def compilemodel(model_path, output_path):
    command = (
        './Tools/model' + executable_extension + ' ' +
        model_path + ' ' +
        output_path + ' ' +
        'noprompt'
    )
    print(colored("\nCompiling model %s..." % os.path.splitext(model_path)[0], "yellow"))
    print(command)
    result = subprocess.run(command, shell = use_shell)

files = os.listdir(model_dir)
for fname in files:
    model_path = os.path.join(model_dir, fname)
    output_path = os.path.join(output_dir, os.path.splitext(fname)[0] + '.jmd')

    should_compile = True
    if os.path.exists(output_path):
        asset_write_time = os.path.getmtime(model_path)
        output_write_time = os.path.getmtime(output_path)
        if asset_write_time < output_write_time:
            should_compile = False

    if should_compile or recompile:
        compilemodel(model_path, output_path)
