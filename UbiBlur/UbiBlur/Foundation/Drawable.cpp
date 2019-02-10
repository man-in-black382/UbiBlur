//
//  Drawable.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 01.11.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "Drawable.hpp"
#include <glad/glad.h>

namespace Engine {

    namespace Drawable {

        namespace TriangleStripQuad {

            void Draw(size_t instanceCount) {
                if (instanceCount > 1) {
                    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei) instanceCount);
                } else {
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                }
            }

        }

        namespace Point {

            void Draw(size_t count) {
                glDrawArrays(GL_POINTS, 0, (GLsizei) count);
            }

        }

        namespace TriangleMesh {

            void Draw(size_t vertexCount, size_t VBOOffset) {
                glDrawArrays(GL_TRIANGLES, VBOOffset, static_cast<GLsizei>(vertexCount));
            }

            void DrawInstanced(size_t instanceCount, size_t vertexCount, size_t VBOOffset) {
                glDrawArraysInstanced(GL_TRIANGLES, VBOOffset, static_cast<GLsizei>(vertexCount), static_cast<GLsizei>(instanceCount));
            }

        }
    }

}
