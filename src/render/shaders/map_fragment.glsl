#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform mat4 view;
uniform int tileSize;
uniform vec2 screenSize;
    
uniform int columnCount;
uniform int rowCount;

uniform sampler2D image;

uniform sampler2D map;

void main() {
    // Screen height is subtracted from y coordinate because +y is down but y=0 is at the bottom of the screen
    vec4 tPos = view * vec4(-gl_FragCoord.x, gl_FragCoord.y - screenSize.y, 0, 1);

    // index of texture to display
    ivec2 tilePos = ivec2(floor(-tPos.xy/tileSize));
    float index = texelFetch(map, tilePos, 0).x * 255.0;

    float x = (mod(index, columnCount) * tileSize) + mod(-tPos.x, tileSize);
    float y = (floor(index * (1.0 / columnCount)) * tileSize) + mod(-tPos.y, tileSize);

    fragColor = texture(image, vec2(x * (1.0 / (columnCount * tileSize)), y * (1.0 / (rowCount * tileSize))));
}