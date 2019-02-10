//
//  GLVertexAttribute.cpp
//  Engine
//
//  Created by Pavlo Muratov on 18.02.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "GLVertexAttribute.hpp"

namespace Engine {

    const GLint GLVertexAttribute::LocationAutomatic = -1;

    GLVertexAttribute::GLVertexAttribute(GLint sizeInBytes, GLint componentCount)
            :
            bytes(sizeInBytes),
            components(componentCount) {
    }

    GLVertexAttribute::GLVertexAttribute(GLint sizeInBytes, GLint componentCount, GLint divisor, GLint location)
            :
            bytes(sizeInBytes),
            components(componentCount),
            divisor(divisor),
            location(location) {
    }

    GLVertexAttribute GLVertexAttribute::UniqueAttribute(GLint sizeInBytes, GLint componentCount, GLint location) {
        return GLVertexAttribute(sizeInBytes, componentCount, 0, location);
    }

    GLVertexAttribute GLVertexAttribute::SharedAttribute(GLint sizeInBytes, GLint componentCount, GLint location) {
        return GLVertexAttribute(sizeInBytes, componentCount, 1, location);
    }

}
