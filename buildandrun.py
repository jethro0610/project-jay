import subprocess
import os

shader_success = subprocess.run("python compileshaders.py")
if shader_success.returncode != 0:
    exit()

os.chdir(".\\Build")
build_success = subprocess.run("cmake --build .")
if build_success.returncode != 0:
    exit()

os.chdir(".\\Debug")
subprocess.call("ProjectJay.exe")
