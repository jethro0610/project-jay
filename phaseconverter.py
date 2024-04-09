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
    if "phases" in convert_level:
        del convert_level["phases"];

    convert_level["entities"] = [] 

    for phase in level_json["phases"]:
        if phase == None:
            continue

        for entity in phase:
            if "persist" in entity:
                del entity["persist"]

            convert_level["entities"].append(entity)

    dump = json.dumps(convert_level, indent=4)

    file = open(level_name, 'w')
    file.write(dump)
    file.close()

files = os.listdir('./Assets/levels/')
for fname in files:
    convert_level('./Assets/levels/' + fname)
