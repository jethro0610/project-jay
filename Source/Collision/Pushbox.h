#include "Collider.h"

class Transform;

struct Pushbox : public Collider {
    Transform* transform;

    bool sender;
    bool reciever;

    bool sent;
    bool recieved;
};
