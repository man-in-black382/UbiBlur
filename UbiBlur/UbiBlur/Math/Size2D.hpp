//
//  Size.hpp
//  Engine
//
//  Created by Pavlo Muratov on 16.06.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef Size_hpp
#define Size_hpp

#include <glm/vec2.hpp>

namespace Engine {

    struct Size2D {
        float width = 0.f;
        float height = 0.f;

        static const Size2D &zero();

        static const Size2D &unit();

        Size2D() = default;

        Size2D(float w, float h);

        Size2D(float side);

        bool operator==(const Size2D &rhs);

        bool operator!=(const Size2D &rhs);

        Size2D transformedBy(const glm::vec2 &vector) const;

        Size2D makeUnion(const Size2D &size);
    };

}

#endif /* Size_hpp */
