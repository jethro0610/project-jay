import os
import json
import sys
import subprocess
from termcolor import colored

input_path = ".\\Source\\Shaders\\"
output_path = ".\\Build\\Debug\\"
shader_desc_file = open(input_path + "DXShaderDescriptions.json")
shader_dict = json.load(shader_desc_file)
shader_model = "5_0"

compile_times = None
try:
    compile_times_file = open(".\\lastcompiletimes.json") 
    compile_times = json.load(compile_times_file)
except:
    compile_times = dict()
    print(colored("Creating new compile time file", "cyan"))
    print()

for shader_desc in shader_dict["shaders"]:
    shader_name = shader_desc["name"]
    shader_type = shader_desc["type"]
    shader_input = input_path + shader_name + ".hlsl"
    shader_output = output_path + shader_name + ".cso"
    last_shader_write = os.path.getmtime(shader_input)
    
    last_compile_time = 0
    try:
        if sys.argv[1] == "--skip":
            try:
                last_compile_time = compile_times[shader_name]
            except:
                pass
    except:
        pass

    if (last_shader_write == last_compile_time):
        print(colored("%s is up to date." % shader_name, "green"))
        print()
        continue;
    
    print(colored("Compiling %s..." % shader_name, "yellow"))

    command = "fxc /T %s /E\"main\" /Fo %s %s" % (shader_type + "_" + shader_model, shader_output, shader_input)
    result = subprocess.run(command)
    
    if result.returncode == 0: 
        compile_times[shader_name] = last_shader_write
        print(colored("Finished compiling %s." % shader_name, "yellow"))
    else:
        print(colored("Failed to compile %s." % shader_name, "red"))
    print()
    print()

with open(".\\lastcompiletimes.json", "w") as outfile:
    json.dump(compile_times, outfile)
