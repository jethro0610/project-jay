import sys
import json
import os

def updateTerrainModifiers(filename):
    levelJson = None

    with open(filename, 'r') as file:
        levelJson = json.load(file)

    bubbles = levelJson.get("bubbles", [])
    noises = levelJson.get("noises", [])
    modifiers = []

    for bubble in bubbles:
        newBubble = dict()
        newBubble["x"] = bubble["x"]
        newBubble["y"] = bubble["z"]
        newBubble["height"] = bubble["y"]
        newBubble["radius"] = bubble["w"]
        newBubble["name"] = "e_bubble"
        newBubble["id"] = 1
        modifiers.append(newBubble)

    for noise in noises:
        newNoise = dict()
        newNoise["x"] = noise["position"]["x"]
        newNoise["y"] = noise["position"]["y"]
        newNoise["frequency"] = noise["frequency"]
        newNoise["radius"] = noise["radius"]
        newNoise["max_height"] = noise["max_height"]
        newNoise["min_height"] = noise["min_height"]
        newNoise["seed"] = noise["seed"]
        newNoise["name"] = "e_noise"
        newNoise["id"] = 2
        modifiers.append(newNoise)

    try:
        del levelJson["bubbles"]
    except:
        print("No bubbles in " + filename)

    try:
        del levelJson["noises"]
    except:
        print("No noises in " + filename)

    levelJson["terrain_modifiers"] = modifiers
    with open(filename, 'w') as file:
        json.dump(levelJson, file, indent = 4)

def main():
    for filename in os.listdir():
        updateTerrainModifiers(filename)

if __name__ == "__main__":
    main()
