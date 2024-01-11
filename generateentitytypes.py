import csv
import glob
from termcolor import colored

typeIds = dict()
with open('./Source/Entity/EntityIDS.csv', 'r') as file:
    csv = csv.reader(file)
    for line in csv:
        typeIds[line[0]] = line[1]

entity_names = []
path_glob = glob.glob('./Source/Entity/*.h', recursive = True)
for path in path_glob:
    path = path.replace('./Source/Entity/', '')
    path = path.replace('.h', '')
    if path == 'EntityList' or path == 'EntityUnion' or path == 'EntityTypes':
        continue

    entity_names.append(path)

should_generate = False
for entity_name in entity_names:
    if entity_name not in typeIds.keys():
        print("entity name not in")
        should_generate = True
        break

for type in typeIds.keys():
    if type not in entity_names:
        should_generate = True
        break

if should_generate:
    print(colored("Regenerating entity types file", "yellow"))
    entity_types_header = open('./Source/Entity/EntityTypes.h', 'w')
    entity_types_header.write('#pragma once\n')
    for entity_name in entity_names:
        entity_types_header.write('#include \"%s.h\"\n' % entity_name)

    entity_types_header.write('\n')
    entity_types_header.write('#define EXPANDENTITIES \\\n')
    currentId = 0
    lines = []
    for entity_name in entity_names:
        idToUse = -1
        if entity_name in typeIds.keys():
            idToUse = typeIds[entity_name]
        else:
            while currentId in typeIds.values():
                currentId = currentId + 1
            idToUse = currentId
            typeIds[entity_name] = idToUse

        lines.append((
            idToUse,
            '\tENTITYEXP(%s, %s, %d) \\\n' % (entity_name, entity_name.lower(), int(idToUse))
        ))

    lines.sort(key=lambda line: line[0])
    lines[len(lines) - 1] = (lines[len(lines) - 1], lines[len(lines) - 1][1].replace(' \\\n', '\n'))
    for line in lines:
        entity_types_header.write(line[1])

    typeIds = [type for type in typeIds.items() if type[0] in entity_names]

    sorted_typeIds = sorted(typeIds, key=lambda type:type[1])
    outCSV = open('./Source/Entity/EntityIDS.csv', 'w')
    for type in sorted_typeIds:
        outCSV.write(('%s,%s\n') % (type[0], type[1]))
