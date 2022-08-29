import shutil
import os

asset_dir = "Assets"
output_dir = "Build/Debug"

files = os.listdir(asset_dir)

for fname in files:
    shutil.copy2(os.path.join(asset_dir, fname), output_dir)
