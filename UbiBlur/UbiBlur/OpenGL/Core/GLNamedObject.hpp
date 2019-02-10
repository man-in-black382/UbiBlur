//
//  GLNamedObject.hpp
//  Engine
//
//  Created by Pavlo Muratov on 11.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLNamedObject_hpp
#define GLNamedObject_hpp

#include <glad/glad.h>
#include <iostream>

namespace Engine {

    class GLNamedObject {
    protected:
        GLuint mName = -1;

    public:
        GLNamedObject() = default;

        GLNamedObject(GLuint);

        GLNamedObject(GLNamedObject &&);

        GLNamedObject &operator=(GLNamedObject &&);

        GLNamedObject(const GLNamedObject &) = delete;

        GLNamedObject &operator=(const GLNamedObject &) = delete;

        virtual ~GLNamedObject() = 0;

        void swap(GLNamedObject &);

        GLuint name() const;
    };

    void swap(GLNamedObject &, GLNamedObject &);
}

#endif /* GLNamedObject_hpp */
