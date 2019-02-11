//
//  GLDepthRenderbuffer.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 10.12.2017.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "GLDepthStencilRenderbuffer.hpp"

namespace Engine {

    GLDepthStencilRenderbuffer::GLDepthStencilRenderbuffer(const Size2D &size) : GLRenderbuffer(size, GL_DEPTH24_STENCIL8) {}

}
