import json
import os

def convert_level(level_name):
    if "autosave" in level_name:
        return 
    if "lv_test_phase" in level_name:
        return

    file = open(level_name, 'r')
    level_json = json.load(file)
    file.close()

    convert_level = level_json.copy()
    if "entities" in convert_level:
        del convert_level["entities"];

    convert_level["phases"] = []
    convert_level["phases"].append([])
    convert_level["phases"].append([])
    convert_level["phases"].append([])
    convert_level["phases"].append([])

    if "entities" in level_json:
        for entity in level_json["entities"]:
            phase = entity["phase"]
            del entity["phase"]
            convert_level["phases"][phase].append(entity)

    dump = json.dumps(convert_level, indent=4)

    file = open(level_name, 'w')
    file.write(dump)
    file.close()

files = os.listdir('./Assets/levels/')
for fname in files:
    convert_level('./Assets/levels/' + fname)
