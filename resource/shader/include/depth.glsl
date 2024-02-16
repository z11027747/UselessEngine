float near = 0.1;
float far = 100.0;

//    2nf
// ————————————
// n+f - z*(f-n)

float LinearizeDepth(float depth01) {
    float z = depth01 * 2.0 - 1.0; //ndc
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float ViewSpace01Depth(float viewZ) {
    return (viewZ - near) / (far - near);
}