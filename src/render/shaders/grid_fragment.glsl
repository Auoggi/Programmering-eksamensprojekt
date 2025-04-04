#version 330 core
out vec4 FragColor;
in vec2 fragCoord;

uniform mat4 view;
uniform float u_tileSize;

void main() {
    vec4 tPos = view * vec4(-gl_FragCoord.x, gl_FragCoord.y, 0, 1);

    FragColor = mix(
        vec4(0, 0, 0, 0), 
        vec4(1, 1, 1, 1), 
        step(mod(tPos.x, u_tileSize), 1.0) + step(mod(tPos.y, u_tileSize), 1.0)
    );
}
