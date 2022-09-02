import shutil
import os
from termcolor import colored

asset_dir = "Assets"
output_dir = "Build/Debug"

if not os.path.exists(output_dir):
    os.makedirs(output_dir)

files = os.listdir(asset_dir)

for fname in files:
    asset_file = os.path.join(asset_dir, fname)
    output_file = os.path.join(output_dir, fname)
    if os.path.exists(output_file):
        asset_write_time = os.path.getmtime(asset_file)
        output_write_time = os.path.getmtime(output_file)
        if asset_write_time == output_write_time:
            continue;
        os.remove(output_file);
        print(colored("New %s. Overwriting previous." % fname, "yellow"))

    print(colored("Copying asset %s." % fname, "yellow"))
    print();
    shutil.copy2(asset_file, output_dir)
