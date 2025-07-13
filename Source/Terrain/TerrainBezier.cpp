#include "TerrainBezier.h"
#include "Terrain.h"
#include "Collision/Ray.h"
#include "Helpers/Shared_Ease.h"
using namespace glm;

TerrainBezier::TerrainBezier() {
    id_ = TerrainBezier::ID;

    for (int i = 0; i < 4; i++) {
        bezierTargets_[i].bezier_ = this;
        bezierTargets_[i].pointIdx_ = i;
        bezierTargets_[i].point_ = &unionPoints_[i].point;
        editorTargets_.push_back(&bezierTargets_[i]);
    }
}

void TerrainBezier::Init(const glm::vec3& pos) {
    active_ = true;
    vec2 planarPos = vec2(pos.x, pos.z);
    for (int i = 0; i < 4; i++) {
        unionPoints_[i].point.position = planarPos;
        unionPoints_[i].point.height = 50.0f;
        unionPoints_[i].point.radius = 50.0f;
        planarPos += vec2(30.0f, 0.0f);
    }
    UpdateBounds();
}

void TerrainBezier::UpdateBounds() {
    minX_ = INFINITY;
    minY_ = INFINITY;
    maxX_ = -INFINITY;
    maxY_ = -INFINITY;

    for (int i = 0; i < 4; i++) {
        BezierPoint& point = unionPoints_[i].point;
        minX_ = min(minX_, point.position.x - point.radius);
        minY_ = min(minY_, point.position.y - point.radius);
        maxX_ = max(maxX_, point.position.x + point.radius);
        maxY_ = max(maxY_, point.position.y + point.radius);
    }
}

bool TerrainBezier::InRange(const glm::vec2& pos) {
    if (pos.x < minX_)
        return false;

    if (pos.y < minY_)
        return false;

    if (pos.x > maxX_)
        return false;

    if (pos.y > maxY_)
        return false;

    return true;
}

TerrainBezier::BezierPoint TerrainBezier::GetPoint(float t) {
    BezierUnion out;
    float tInv = 1 - t;
    out.vec =
        tInv * tInv * tInv * unionPoints_[0].vec + 
        3.0f * tInv * tInv * t * unionPoints_[1].vec +
        3.0f * tInv * t * t * unionPoints_[2].vec +
        t * t * t * unionPoints_[3].vec;
    return out.point;
}

TerrainBezier::BezierPoint TerrainBezier::GetNearestPoint(const glm::vec2& pos) {
    constexpr int ITERATIONS = 16;
    constexpr int DIVISIONS = 4;
    float min = 0.0f;
    float max = 1.0f;
    BezierPoint nearestPoint;
    for (int i = 0; i < ITERATIONS; i++) {
        float range = max - min;
        float minDist = INFINITY;
        for (int j = 0; j <= DIVISIONS; j++) {
            float t = min + (range / DIVISIONS) * j;
            t = clamp(t, 0.0f, 1.0f);
            BezierPoint point = GetPoint(t);
            float dist2 = distance2(point.position, pos);
            if (dist2 < minDist) {
                minDist = dist2;
                nearestPoint = point;
                min = t - (range / DIVISIONS);
                max = t + (range / DIVISIONS);
            }
        }
    }
    return nearestPoint;
}

float TerrainBezier::GetHeight(const glm::vec2 &pos) {
    BezierPoint nearest = GetNearestPoint(pos);
    float dist = distance(nearest.position, pos);

    float t = 1.0f - (dist / nearest.radius);
    t = clamp(t, 0.0f, 1.0f);
    if (customEasing_)
        t = EaseInOut(t, inPower_, outPower_);
    else
        t = EaseQuad(t);

    return nearest.height * t;
}

void TerrainBezier::Save(nlohmann::json &json) {
    nlohmann::json points;
    for (int i = 0; i < 4; i++) {
        nlohmann::json point;
        point["x"] = unionPoints_[i].point.position.x;
        point["y"] = unionPoints_[i].point.position.y;
        point["radius"] = unionPoints_[i].point.radius;
        point["height"] = unionPoints_[i].point.height;
        points.push_back(point);
    }
    json["points"] = points;

    json["in_power"] = inPower_;
    json["out_power"] = outPower_;
    json["custom_easing"] = customEasing_;
}

void TerrainBezier::Load(const nlohmann::json &json) {
    for (int i = 0; i < 4; i++) {
        const nlohmann::json& point = json["points"][i];
        unionPoints_[i].point.position.x = point["x"];
        unionPoints_[i].point.position.y = point["y"];
        unionPoints_[i].point.radius = point["radius"];
        unionPoints_[i].point.height = point["height"];
    }
    inPower_ = json["in_power"];
    outPower_ = json["out_power"];
    customEasing_ = json["custom_easing"];
    UpdateBounds();
}

void TerrainBezier::WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain) {
    for (int i = 0; i < 4; i++) {
        RenderNode node;
        BezierPoint& point = unionPoints_[i].point;
        node.color = vec4(0.0f, 1.0f, 0.0f, 0.5f);
        float height = terrain.GetRawHeight(point.position) + 10.0f;
        node.transform.position = vec3(point.position.x, height, point.position.y);
        node.selected = bezierTargets_[i].selected_;
        node.transform.scale = vec3(3.0f, 6.0f, 3.0f);
        nodes.push_back(node);
    }
}

vec3 TerrainBezier::ETarget::GetPosition() {
    float height = bezier_->terrain_->GetRawHeight(point_->position) + 10.0f;
    return vec3(point_->position.x, height, point_->position.y);
}

void TerrainBezier::ETarget::SetPosition(const glm::vec3 &pos) {
    point_->position.x = pos.x;
    point_->position.y = pos.z;
    bezier_->UpdateBounds();
}

bool TerrainBezier::ETarget::RayHit(const Ray& ray) {
    float height = bezier_->terrain_->GetRawHeight(point_->position) + 10.0f;
    return ray.HitSphere(vec3(point_->position.x, height, point_->position.y), 5.0f);
}

float TerrainBezier::ETarget::GetScalar(char id) {
    switch (id) {
        case '1':
            return point_->radius;

        case '2':
            return point_->height;

        case '3':
            return bezier_->inPower_;

        case '4':
            return bezier_->outPower_;
    }
    return 0.0;
}

float TerrainBezier::ETarget::GetScalarDelta(char id) {
    if (id == '3' || id == '4')
        return 0.1f;
    else
        return 1.0f;
}

void TerrainBezier::ETarget::SetScalar(char id, float value) {
    switch (id) {
        case '1':
            point_->radius = value;
            break;

        case '2':
            point_->height = value;
            break;

        case '3':
            bezier_->inPower_ = value;
            break;

        case '4':
            bezier_->outPower_ = value;
            break;
    }
    bezier_->UpdateBounds();
}

void TerrainBezier::ETarget::SetFlag(char id) {
    switch(id) {
        case ',':
            bezier_->customEasing_ = false;
            break;

        case '.':
            bezier_->customEasing_ = true;
            break;
    }
}
