#version 450

layout(location = 0) out vec2 uv;

void main() {

    vec4 positioNDC = vec4(1.0, 1.0, 0.0, 1.0);
    if(gl_VertexIndex == 0) {
        positioNDC.xy = vec2(-1.0, -1.0);
    } else if(gl_VertexIndex == 1) {
        positioNDC.xy = vec2(1.0, -1.0);
    } else if(gl_VertexIndex == 2) {
        positioNDC.xy = vec2(-1.0, 1.0);
    } else if(gl_VertexIndex == 3) {
        positioNDC.xy = vec2(-1.0, 1.0);
    } else if(gl_VertexIndex == 4) {
        positioNDC.xy = vec2(1.0, -1.0);
    } else if(gl_VertexIndex == 5) {
        positioNDC.xy = vec2(1.0, 1.0);
    }

    gl_Position = positioNDC;
    uv = positioNDC.xy * 0.5 + 0.5;
}