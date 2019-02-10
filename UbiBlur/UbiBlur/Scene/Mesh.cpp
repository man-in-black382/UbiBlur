//
//  SubMesh.cpp
//  Engine
//
//  Created by Pavlo Muratov on 07.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "Mesh.hpp"
#include "Triangle3D.hpp"

namespace Engine {

    const std::string &Mesh::name() const {
        return mName;
    }

    const std::string &Mesh::materialName() const {
        return mMaterialName;
    }

    const std::vector<Vertex1P1N2UV1T1BT> &Mesh::vertices() const {
        return mVertices;
    }

    const AxisAlignedBox3D &Mesh::boundingBox() const {
        return mBoundingBox;
    }

    std::vector<Vertex1P1N2UV1T1BT> &Mesh::vertices() {
        return mVertices;
    }

    float Mesh::surfaceArea() const {
        return mArea;
    }

    void Mesh::setName(const std::string &name) {
        mName = name;
    }

    void Mesh::setMaterialName(const std::string &name) {
        mMaterialName = name;
    }

    void Mesh::addVertex(const Vertex1P1N2UV1T1BT &vertex) {
        mBoundingBox.min = glm::min(glm::vec3(vertex.position), mBoundingBox.min);
        mBoundingBox.max = glm::max(glm::vec3(vertex.position), mBoundingBox.max);
        mVertices.push_back(vertex);

        if ((mVertices.size() % 3) == 0) {
            size_t count = mVertices.size();
            auto &v0 = mVertices[count - 3];
            auto &v1 = mVertices[count - 2];
            auto &v2 = mVertices[count - 1];

            Triangle3D triangle(v0.position, v1.position, v2.position);
            mArea += triangle.area();
        }
    }

}
