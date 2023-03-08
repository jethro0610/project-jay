import os

component_dir = '.\\Source\\Game\\Components\\'
files = os.listdir(component_dir)
file_dir = component_dir + files[6]
print(file_dir)

class componentDescription:
    def __init__(name, properties):
        self.name = name
        self.properties = properties

def generatePropertiesFromFile()
    inCProperties = False
    done = False
    inVars = False
    inType = False
    curType = ''

    name = ''
    properties = []
    with open(file_dir) as f:
        lines = f.readlines()

    for line in lines:
        if done:
            break

        tokens = line.split()
        for token in tokens:
            if 'CProperties' in token:
                inCProperties = True
                name = token.replace('CProperties', '')
                name += ' Component'

            if not inCProperties:
                continue

            if inVars: 
                if not inType:
                    curType = token.replace('&', '');
                    inType = True
                else:
                    propertyName = token.replace(';', '');
                    properties.append((propertyName, curType))
                    inType = False

            if '{' in token:
                inVars = True; 

            if '};' in token:
                inCProperty = False
                done = True
                break

