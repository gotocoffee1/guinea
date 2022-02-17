#pragma once

extern "C" EXPORT ImTextureID load_texture(const unsigned char* image_data,
                                           int out_width,
                                           int out_height)
{
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 out_width,
                 out_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image_data);

    return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(image_texture));
}

extern "C" EXPORT void unload_texture(ImTextureID out_texture)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    auto image_texture = static_cast<GLuint>(reinterpret_cast<uintptr_t>(out_texture));
    glDeleteTextures(1, &image_texture);
}
