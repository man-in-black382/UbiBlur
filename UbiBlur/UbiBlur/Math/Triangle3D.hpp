//
//  Triangle3D.hpp
//  Engine
//
//  Created by Pavlo Muratov on 19.01.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#ifndef Triangle3D_hpp
#define Triangle3D_hpp

#include "Triangle.hpp"

#include <glm/vec3.hpp>

namespace Engine {

    struct AxisAlighnedBox3D;

    struct Triangle3D : public Triangle<glm::vec3> {
        using Triangle<glm::vec3>::Triangle;

        float area() const override;

        AxisAlignedBox3D boundingBox() const;

        glm::vec3 normal() const;

        std::array<Triangle3D, 4> split() const;
    };

}

#endif /* Triangle3D_hpp */
