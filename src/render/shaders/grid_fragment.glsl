#version 330 core
out vec4 FragColor;
in vec2 fragCoord;

uniform mat4 view;
uniform float tileSize;
uniform float screenHeight;

void main() {
    // screenHeight is subtracted from y coordinate because +y is down but y=0 is at the bottom of the screen
    vec4 tPos = view * vec4(-gl_FragCoord.x, gl_FragCoord.y - screenHeight, 0, 1);

    FragColor = mix(
        vec4(0, 0, 0, 0), 
        vec4(1, 1, 1, 1), 
        step(mod(tPos.x, tileSize), 1.0) + step(mod(tPos.y, tileSize), 1.0)
    );
}
