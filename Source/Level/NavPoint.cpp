#include "NavPoint.h"
#include "Collision/Ray.h"

bool NavPoint::Target::RayHit(const Ray& ray) {
    return ray.HitSphere(navpoint_->position_, 30.0f);
}
