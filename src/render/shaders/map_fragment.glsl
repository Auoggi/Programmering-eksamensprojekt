#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform mat4 view;
uniform float tileSize;
uniform vec2 screenSize;
    
uniform int columnCount;
uniform int rowCount;

uniform sampler2D image;

void main() {
    vec4 tPos = view * vec4(-gl_FragCoord.x, gl_FragCoord.y - screenSize.y, 0, 1);

    // index of texture to display
    int index = 3; // Hardcoded test value

    float x = (mod(index, columnCount) * tileSize) + mod(-tPos.x, tileSize);
    float y = (floor(index * (1.0 / columnCount)) * tileSize) + mod(-tPos.y, tileSize);

    fragColor = texture(image, vec2(x * (1.0 / (rowCount * tileSize)), y * (1.0 / (rowCount * tileSize))));
}