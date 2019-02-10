//
//  Renderable.hpp
//  EARenderer
//
//  Created by Pavlo Muratov on 12.04.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef Renderable_hpp
#define Renderable_hpp

#include <stdio.h>

namespace Engine {

    namespace Drawable {

        namespace TriangleStripQuad {
            void Draw(size_t instanceCount = 1);
        }

        namespace Point {
            void Draw(size_t count = 1);
        }

        namespace TriangleMesh {
            void Draw(size_t vertexCount = 1, size_t VBOOffset = 0);

            void DrawInstanced(size_t instanceCount, size_t vertexCount = 1, size_t VBOOffset = 0);
        }

    }

}

#endif /* Renderable_hpp */
