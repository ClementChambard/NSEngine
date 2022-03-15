#include "SpriteBatch.h"
#include "TextureManager.h"
#include "vertex.h"
#include "NSEngine.h"
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

namespace NSEngine {

    void SpriteBatchData::uniformChange::Apply()
    {
        switch (type)
        {
            case FLOAT1:
                glUniform1fv(uniformID,1,(GLfloat*)data);               
                break;
            case FLOAT2:
                glUniform2fv(uniformID,2,(GLfloat*)data);               
                break;
            case FLOAT3:
                glUniform3fv(uniformID,3,(GLfloat*)data);               
                break;
            case FLOAT4:
                glUniform4fv(uniformID,4,(GLfloat*)data);               
                break;
            case INT1:
                glUniform1iv(uniformID,1,(GLint*)data);               
                break;
            case INT2:
                glUniform2iv(uniformID,2,(GLint*)data);               
                break;
            case INT3:
                glUniform3iv(uniformID,3,(GLint*)data);               
                break;
            case INT4:
                glUniform4iv(uniformID,4,(GLint*)data);               
                break;
            case UINT1:
                glUniform1uiv(uniformID,1,(GLuint*)data);               
                break;
            case UINT2:
                glUniform2uiv(uniformID,2,(GLuint*)data);               
                break;
            case UINT3:
                glUniform3uiv(uniformID,3,(GLuint*)data);               
                break;
            case UINT4:
                glUniform4uiv(uniformID,4,(GLuint*)data);               
                break;
            case MAT2:
                glUniformMatrix2fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT3:
                glUniformMatrix3fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT4:
                glUniformMatrix4fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT23:
                glUniformMatrix2x3fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT32:
                glUniformMatrix3x2fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT24:
                glUniformMatrix2x4fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT42:
                glUniformMatrix4x2fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT34:
                glUniformMatrix3x4fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
            case MAT43:
                glUniformMatrix4x3fv(uniformID,1,GL_FALSE,(GLfloat*)data);
                break;
        }
    }


    SpriteBatch::SpriteBatch() {}
    SpriteBatch::~SpriteBatch() {}

    SpriteBatch* SpriteBatch::Init()
    {
        glGenBuffers(1, &vbo);
        return this;
    }

    void SpriteBatch::Dispose()
    {
        if (vbo != 0) glDeleteBuffers(1, &vbo);
    }

    void SpriteBatch::begin() { renderBatches.clear(); glyphs.clear(); }

    void SpriteBatch::end(bool staticdraw) {
        glyphsptr.resize(glyphs.size());
        for (size_t i = 0; i < glyphs.size(); i++) glyphsptr[i] = &glyphs[i];

        sortGlyphs(depthSort);
        createRenderBatches(staticdraw);
    }

    void SpriteBatch::draw(GLuint texture, Vertex tl, Vertex tr, Vertex br, Vertex bl, int blendmode, bool is3d) 
    {
        glyphs.emplace_back(texture, tl, tr, br, bl, blendmode, is3d);
    }

    void SpriteBatch::renderBatch(bool depthTest, bool staticview) {

        if (depthTest) glEnable(GL_DEPTH_TEST);

        bindBuffer();

        for (size_t i = 0; i < renderBatches.size(); i++)
        {
            if (i != 0)
            {
                if (renderBatches[i-1].texture != renderBatches[i].texture)
                    TextureManager::UseTexture(renderBatches[i].texture);
                if (renderBatches[i-1].blendmode != renderBatches[i].blendmode)
                    TextureManager::SetBlendmode(renderBatches[i].blendmode);
                //if (renderBatches[i-1].is3d != renderBatches[i].is3d)
                   // TextureManager::SetPerspective(renderBatches[i].is3d,staticview);
            }
            else 
            {
                TextureManager::UseTexture(renderBatches[i].texture);
                TextureManager::SetBlendmode(renderBatches[i].blendmode);
                //TextureManager::SetPerspective(renderBatches[i].is3d,staticview);
            }
            glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVertices);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (depthTest) glDisable(GL_DEPTH_TEST);

    }

    void SpriteBatch::bindBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    }

    void SpriteBatch::createRenderBatches(bool staticdraw)
    {
        std::vector<Vertex> vertices;
        vertices.resize(glyphsptr.size() * 6);

        if (glyphs.empty()) return;

        int cv = 0; //current vertex
        renderBatches.emplace_back(0, 6, glyphsptr[0]->texture,glyphsptr[0]->blendmode);
        vertices[cv++] = glyphsptr[0]->topLeft;
        vertices[cv++] = glyphsptr[0]->topRight;
        vertices[cv++] = glyphsptr[0]->bottomLeft;
        vertices[cv++] = glyphsptr[0]->bottomLeft;
        vertices[cv++] = glyphsptr[0]->topRight;
        vertices[cv++] = glyphsptr[0]->bottomRight;

        for (size_t cg = 1; cg < glyphsptr.size(); cg++)
        {
            if (glyphsptr[cg]->texture != glyphsptr[cg-1]->texture || glyphsptr[cg]->blendmode != glyphsptr[cg-1]->blendmode || glyphsptr[cg]->is3d != glyphsptr[cg-1]->is3d) renderBatches.emplace_back(cv, 6, glyphsptr[cg]->texture, glyphsptr[cg]->blendmode, glyphsptr[cg]->is3d);
            else renderBatches.back().numVertices += 6;
            vertices[cv++] = glyphsptr[cg]->topLeft;
            vertices[cv++] = glyphsptr[cg]->topRight;
            vertices[cv++] = glyphsptr[cg]->bottomLeft;
            vertices[cv++] = glyphsptr[cg]->bottomLeft;
            vertices[cv++] = glyphsptr[cg]->topRight;
            vertices[cv++] = glyphsptr[cg]->bottomRight;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        if (!staticdraw)
        {
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); //orphan the buffer
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());    //upload the data
        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SpriteBatch::sortGlyphs(bool sort) {
        //if (sort) std::stable_sort(glyphsptr.begin(), glyphsptr.end(), [](Glyph* a, Glyph* b){ return a->texture < b->texture; });
        return;
        if (sort && NSEngine::activeCamera3D())
        {
            glm::vec3 camp = NSEngine::activeCamera3D()->getPosition();
            glm::vec3 caml = NSEngine::activeCamera3D()->getLook();
            #define p(v) glm::vec3(v.position.x, v.position.y, v.position.z)
            auto dist = [camp, caml](Glyph* a)
            {
                glm::vec3 pos = p(a->topLeft) + p(a->topRight) + p(a->bottomLeft) + p(a->bottomRight);
                pos /= 4.f;
                pos -= camp;
                return glm::dot(caml, pos);
            };
            #undef p
            auto comp = [dist](Glyph* a, Glyph* b) { return dist(a) < dist(b); };
            std::sort(glyphsptr.begin(), glyphsptr.end(), comp);
        }
    }

}
