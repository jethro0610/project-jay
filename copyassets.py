import shutil
import os
from termcolor import colored

asset_dir = "Assets"
output_dir = "Output"

def copyfromfolder(folder):
    in_folder_dir = asset_dir + "/" + folder 
    out_folder_dir = output_dir + "/" + folder 
    if not os.path.exists(out_folder_dir):
        os.makedirs(out_folder_dir)

    files = os.listdir(in_folder_dir)

    for fname in files:
        asset_file = os.path.join(in_folder_dir, fname)
        output_file = os.path.join(out_folder_dir, fname)
        if os.path.exists(output_file):
            asset_write_time = os.path.getmtime(asset_file)
            output_write_time = os.path.getmtime(output_file)
            if asset_write_time == output_write_time:
                continue;
            os.remove(output_file);
            print(colored("New %s. Overwriting previous." % fname, "yellow"))

        print(colored("Copying asset %s." % fname, "yellow"))
        print();
        shutil.copy2(asset_file, out_folder_dir)

copyfromfolder("emitters")
copyfromfolder("entities")
copyfromfolder("textures")
copyfromfolder("materials")
copyfromfolder("mappings")
