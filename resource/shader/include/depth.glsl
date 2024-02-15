
float near = 0.1;
float far = 100.0;

float LinearizeDepth(float z) {
    float z_ndc = z * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z_ndc * (far - near));
}

float Linearize01Depth(float linearZ) {  
    return (linearZ - near) / (far - near);  
}