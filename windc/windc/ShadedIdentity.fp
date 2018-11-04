#version 330

in vec2 fs_tex_pos;

layout (location = 0) out vec4 color;
uniform sampler2D tex;

void main(void)
{
    color = texture(tex, fs_tex_pos);
}
