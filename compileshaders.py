import os
import platform
import json
import sys
import subprocess
import re
import glob
from termcolor import colored

input_path = './Source/Shaders/'
include_path = input_path + 'Include/'
output_path = './Output/shaders/'
shader_platform = ''
shader_profile = ''
executable_extension = ''
use_shell = False
recompile = False
if '-r' in sys.argv:
    recompile = True

if platform.system() == 'Windows':
    shader_platform = 'windows'
    shader_profile = 's_5_0'
    executable_extension = '.exe'
elif platform.system() == 'Linux':
    shader_platform = 'linux'
    shader_profile = 'spirv'
    use_shell = True

shader_descriptions = dict()
shared_file_paths = dict()

compile_times = dict()
last_compile_times = None
try:
    compile_file = open("./lastcompiletimes.json") 
    last_compile_times = json.load(compile_file)
except:
    last_compile_times = dict()

path_glob = glob.glob('./**/Shared_*', recursive = True)
for path in path_glob:
    shared_file_paths[os.path.basename(path)] = path

include_string = ''
included_paths = set()
for path in shared_file_paths.values():
    dir = os.path.dirname(path)
    if dir in included_paths:
        continue

    include_string += '-i ' + dir + ' '
    included_paths.add(dir)

class ShaderDesc:
    def __init__(self):
        self.path = '' 
        self.name = ''
        self.shader_type = '' 
        self.includes = [] 
        self.last_compile = 0 

    def should_compile(self):
        if self.last_compile != os.path.getmtime(self.path):
            return True
        
        for include in self.includes:
            if include.should_compile() == True:
                return True 

        return False 
        

def get_include_names(path):
    include_names = []
    file = open(path, 'r')
    lines = file.readlines()
    file.close()

    # If we're in a shared file, then we have to wait to
    # till we're at the #else directive for us to be 
    # in the shader code
    in_shader_code = True

    for line in lines:
        if '#ifndef SHARED_SHADER' in line:
            in_shader_code = False
            continue

        if '#else' in line or '#endif' in line:
            in_shader_code = True
            continue

        if not in_shader_code:
            continue

        if '#include' in line:
            name = line.replace('#include ', '')
            name = name.replace('<', '')
            name = name.replace('>', '')
            name = name.replace('>', '')
            name = name.replace('\n', '')
            if name == 'bgfx_shader.sh':
                continue
            include_names.append(name)

    return include_names

def make_shaderdesc(path, shader_type):
    desc = ShaderDesc()
    desc.path = path 
    desc.name = os.path.basename(path)
    desc.shader_type = shader_type
    if desc.name in last_compile_times:
        desc.last_compile = last_compile_times[desc.name]

    include_names = get_include_names(path)

    for include_name in include_names:
        if include_name not in shader_descriptions:
            if include_name in shared_file_paths:
                make_shaderdesc(shared_file_paths[include_name], 'header')
            else:
                make_shaderdesc(include_path + include_name, 'header')

        desc.includes.append(shader_descriptions[include_name])

    shader_descriptions[desc.name] = desc

def make_shaderdescs_in_folder(path, shader_type):
    filenames = os.listdir(path)
    for filename in filenames:
        make_shaderdesc(path + filename, shader_type)

make_shaderdescs_in_folder(input_path + 'Vertex/', 'vertex')
make_shaderdescs_in_folder(input_path + 'Fragment/', 'fragment')

for desc in shader_descriptions.values():
    compile_times[desc.name] = os.path.getmtime(desc.path)
    if desc.shader_type == 'header':
        continue

    if desc.should_compile() == False and recompile == False:
        continue

    print(colored("Compiling %s..." % desc.name, "yellow"))

    command = (
        './Tools/shaderc' + executable_extension + ' '
        '-i ./Tools/include/ '
        '-i ./Source/Shaders/Include/ ' +
        include_string +
        '--varyingdef ./Source/Shaders/varying.def.sc '
        '-f %s '
        '-o %s '
        '--type %s '
        '--platform %s '
        '--profile %s'
    )
    output_name = os.path.splitext(desc.name)[0] + '.bin';
    command = command % (desc.path, output_path + output_name, desc.shader_type, shader_platform, shader_profile)
    result = subprocess.run(command, shell = use_shell)

    if result.returncode == 0:
        print(colored("Finished compiling %s." % desc.name, "yellow"))
        print()
    else:
        compile_times[desc.name] = 0
        print(colored("Failed to compile %s." % desc.name, "red")) 
        print()
        sys.exit(1)

with open("lastcompiletimes.json", "w+") as outfile:
    json.dump(compile_times, outfile)
