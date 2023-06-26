import subprocess
import os

subprocess.run("python copyassets.py")

shader_success = subprocess.run("python compileshaders.py")
if shader_success.returncode != 0:
    exit()

os.chdir("./Build")
build_success = subprocess.run("cmake --build .")
if build_success.returncode != 0:
    exit()

os.chdir("../Output")
subprocess.call("ProjectJay.exe")
