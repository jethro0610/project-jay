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
    compile_times["shaders"] = dict()
    compile_times["headers"] = dict()
    print(colored("Creating new compile time file", "cyan"))
    print()

invalid_headers = []
checked_headers = [] 
for shader_desc in shader_dict["shaders"]:
    shader_name = shader_desc["name"]
    shader_input = input_path + shader_name + ".hlsl"

    if shader_name not in compile_times["shaders"]:
        compile_times["shaders"][shader_name] = dict()
        compile_times["shaders"][shader_name]["success"] = False
        compile_times["shaders"][shader_name]["time"] = 0
    compile_times["shaders"][shader_name]["headers"] = []

    with open(shader_input) as file:
        lines = file.readlines()
        for i in range(len(lines)):
            line = lines[i].strip()
            if "#include " in line:
                line = line.replace("#include \"", "")
                line = line.replace("\"", "")
                compile_times["shaders"][shader_name]["headers"].append(line) 

                if line not in checked_headers:
                    checked_headers.append(line)
                    last_compile_time = 0
                    try:
                        last_compile_time = compile_times["headers"][line]
                    except:
                        pass
                    last_header_write = os.path.getmtime(input_path + line)
                    if last_header_write != last_compile_time:
                        invalid_headers.append(line)
                        compile_times["headers"][line] = os.path.getmtime(input_path + line) 
                    
for shader_desc in shader_dict["shaders"]:
    shader_name = shader_desc["name"]
    shader_type = shader_desc["type"]
    shader_input = input_path + shader_name + ".hlsl"
    shader_output = output_path + shader_name + ".cso"
    last_shader_write = os.path.getmtime(shader_input)

    last_compile_time = compile_times["shaders"][shader_name]["time"] 
    try:
        if sys.argv[1] == "--all":
            last_compile_time = 0            
    except:
        pass
    
    for header in compile_times["shaders"][shader_name]["headers"]:
        if header in invalid_headers:
            last_compile_time = 0

    if compile_times["shaders"][shader_name]["success"] == False:
        last_compile_time = 0

    if (last_shader_write == last_compile_time):
        print(colored("%s is up to date." % shader_name, "green"))
        print()
        continue;
    
    print(colored("Compiling %s..." % shader_name, "yellow"))

    command = "fxc /T %s /E\"main\" /Fo %s %s" % (shader_type + "_" + shader_model, shader_output, shader_input)
    result = subprocess.run(command)
    
    if result.returncode == 0:
        compile_times["shaders"][shader_name]["time"] = last_shader_write
        compile_times["shaders"][shader_name]["success"] = True
        print(colored("Finished compiling %s." % shader_name, "yellow"))
    else:
        compile_times["shaders"][shader_name]["success"] = False 
        print(colored("Failed to compile %s." % shader_name, "red")) 
        print()
        print()
        sys.exit(1)

    print()
    print()

with open(".\\lastcompiletimes.json", "w") as outfile:
    json.dump(compile_times, outfile)
