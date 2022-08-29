import os
import json
import subprocess

full_command = ""

full_command += "call \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat\""

input_path = ".\\Source\\Shaders\\"
output_path = ".\\Build\\Debug\\"
shader_desc_file = open(input_path + "DXShaderDescriptions.json")
shader_dict = json.load(shader_desc_file)
shader_model = "5_0"

for shader_desc in shader_dict["shaders"]:
    shader_name = shader_desc["name"]
    shader_type = shader_desc["type"]
    shader_input = input_path + shader_name + ".hlsl"
    shader_output = output_path + shader_name + ".cso"
    
    command = "fxc /T %s /E\"main\" /Fo %s %s" % (shader_type + "_" + shader_model, shader_output, shader_input)
    full_command += " & " + command

os.system(full_command) 
