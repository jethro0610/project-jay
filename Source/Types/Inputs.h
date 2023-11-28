#pragma once
struct Inputs {
    float forwardInput;
    float sideInput;
    float deltaLookX;
    float deltaLookY;
    bool ski;
    bool flow;
    bool cut;

    bool attack;

    Inputs() {
        forwardInput = 0.0f;
        sideInput = 0.0f;
        deltaLookX = 0.0f;
        deltaLookY = 0.0f;
        ski = false;
        flow = false;
        cut = false;
    }
};
