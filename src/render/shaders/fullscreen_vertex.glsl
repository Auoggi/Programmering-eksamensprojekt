#version 330 core
layout(location = 0) in vec2 aPos;
out vec2 fragCoord;

uniform vec2 screenSize;

void main() {
    vec2 ndc = (aPos / screenSize) * 2.0 - 1.0;

    fragCoord = ndc;
    gl_Position = vec4(ndc, 0, 1);
}
