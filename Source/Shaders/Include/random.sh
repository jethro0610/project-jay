#ifndef RANDOM_H
#define RANDOM_H

uniform vec4 u_randomVec;
#define u_random u_randomVec.x

int getRandom(vec2 vec) {
    int random = (vec.x * vec.y) * (vec.y + vec.x) + 12863;
    random += u_random;
    random = random * (random + 3) % 516169;
    return random;
}

#endif
