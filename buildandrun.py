import subprocess
import platform
import os

executable_extension = ""
if platform.system() == "Windows":
    executable_extension = ".exe"

subprocess.run("python copyassets.py", shell = True)

shader_success = subprocess.run("python compileshaders.py", shell = True)
if shader_success.returncode != 0:
    exit()

os.chdir("./Build")
build_success = subprocess.run("cmake --build .", shell = True)
if build_success.returncode != 0:
    exit()

os.chdir("../Output")
subprocess.call("./ProjectJay" + executable_extension)
