#pragma once

ImTextureID ui::guinea::impl::load_texture(ui::guinea& self, const unsigned char* image_data, int image_width, int image_height) noexcept
{
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image_width,
                 image_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image_data);

    return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(image_texture));
}

void ui::guinea::impl::unload_texture(ui::guinea&, ImTextureID texture) noexcept
{
    glBindTexture(GL_TEXTURE_2D, 0);
    auto image_texture = static_cast<GLuint>(reinterpret_cast<uintptr_t>(texture));
    glDeleteTextures(1, &image_texture);
}
