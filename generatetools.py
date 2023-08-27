import subprocess
import os

if not os.path.exists('./Tools'):
    os.makedirs('./Tools')

os.chdir('./Tools')
if not os.path.exists('CMakeCache.txt'):
    subprocess.run('cmake ../Libraries')

subprocess.run('cmake --build .', shell = True)
