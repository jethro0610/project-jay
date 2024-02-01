import subprocess
import platform
import os
import sys

executable_extension = ''
use_shell = False
recompile = '' 

if '-r' in sys.argv:
    recompile = '-r' 
if platform.system() == 'Windows':
    executable_extension = '.exe'
elif platform.system() == 'Linux':
    use_shell = True

subprocess.run('python generateentitytypes.py', shell = True)
subprocess.run('python generatetools.py', shell = True)
subprocess.run('python copyassets.py', shell = True)
subprocess.run('python compileblobs.py', shell = True)
subprocess.run('python generateshaderheaders.py', shell = True)
subprocess.run('python compilemodels.py ' + recompile, shell = True)

shader_success = subprocess.run('python compileshaders.py ' + recompile, shell = True)
if shader_success.returncode != 0:
    exit()

if not os.path.exists('./Build'):
    os.makedirs('./Build')
os.chdir('./Build')
subprocess.run('cmake ../', shell = True)

build_success = subprocess.run('cmake --build .', shell = True)
if build_success.returncode != 0:
    exit()

os.chdir('../Output')
subprocess.call('./ProjectJay' + executable_extension)
