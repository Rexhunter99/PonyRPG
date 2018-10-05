#pragma once

#include <glm/glm.hpp>

#include <array>
#include <cstdint>
#include <vector>


class Animation;
class Model;


class CollisionModel
{
public:

    size_t getVertexCount() const;
    size_t getTriangleCount() const;
    bool isVectorIntersecting(glm::vec3 p, glm::vec3 d);
    bool isSphereIntersecting(glm::vec3 p, float r);
    bool isBoxIntersecting(glm::vec3 p1, glm::vec3 p2);
    bool isMeshIntersecting(const CollisionModel &cm); //NOTE: transform?

protected:
    friend class Animation;
    friend class Model;

    struct { glm::vec3 p1,p2; } boundbox;

    std::vector<std::array<float,3>> vertices;
    std::vector<std::array<uint32_t,3>> triangles;
};

