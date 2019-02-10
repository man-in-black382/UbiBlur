//
//  SubMesh.hpp
//  Engine
//
//  Created by Pavlo Muratov on 07.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef SubMesh_hpp
#define SubMesh_hpp

#include "Vertices/Vertex1P1N2UV1T1BT.hpp"
#include "GLVertexArrayBuffer.hpp"
#include "GLElementArrayBuffer.hpp"
#include "GLVertexArray.hpp"
#include "AxisAlignedBox3D.hpp"

#include <vector>

namespace Engine {

    class Mesh {
    private:
        std::string mName;
        std::string mMaterialName;
        std::vector<Vertex1P1N2UV1T1BT> mVertices;
        AxisAlignedBox3D mBoundingBox = AxisAlignedBox3D::MaximumReversed();
        float mArea = 0.0;

    public:
        Mesh() = default;

        const std::string &name() const;

        const std::string &materialName() const;

        const std::vector<Vertex1P1N2UV1T1BT> &vertices() const;

        const AxisAlignedBox3D &boundingBox() const;

        std::vector<Vertex1P1N2UV1T1BT> &vertices();

        float surfaceArea() const;

        void setName(const std::string &name);

        void setMaterialName(const std::string &name);

        void addVertex(const Vertex1P1N2UV1T1BT &vertex);
    };

}

#endif /* SubMesh_hpp */
