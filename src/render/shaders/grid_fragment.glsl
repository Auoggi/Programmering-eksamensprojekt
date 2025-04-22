#version 330 core
out vec4 fragColor;
in vec2 fragCoord;

uniform mat4 view;
uniform float tileSize;
uniform vec2 screenSize;

uniform vec2 centerTile;

void main() {
    // Screen height is subtracted from y coordinate because +y is down but y=0 is at the bottom of the screen
    vec4 tPos = view * vec4(-gl_FragCoord.x, gl_FragCoord.y - screenSize.y, 0, 1);
    
    // Highlight player collision tiles
    vec2 tilePos = floor(-tPos.xy / tileSize);
    if(tilePos == centerTile) {
        fragColor = vec4(0.75, 0.2, 0.2, 1);
    } else if(abs(tilePos.x - centerTile.x) <= 1 && abs(tilePos.y - centerTile.y) <= 1) {
        fragColor = vec4(1, 0.8, 0, 1);
    } else {
        fragColor = vec4(0, 0, 0, 0);
    }

    fragColor = mix(
        fragColor, 
        vec4(1, 1, 1, 1), 
        step(mod(tPos.x, tileSize), 1.0) + step(mod(tPos.y, tileSize), 1.0)
    );
}
