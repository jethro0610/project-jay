import sys
import json

def main():
    if len(sys.argv) < 2:
        print("No input file provided")
        return

    fileName = sys.argv[1]
    levelJson = None

    with open(fileName, 'r') as file:
        levelJson = json.load(file)

    levelJson["spread"] = []
    with open(fileName, 'w') as file:
        json.dump(levelJson, file, indent = 4)

if __name__ == "__main__":
    main()
