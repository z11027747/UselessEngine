

float CalcShadow(vec4 positionLS, sampler2DShadow shadowMap) {
    vec4 positionNDCLS = positionLS / positionLS.w;

    if (abs(positionNDCLS.x) > 1.0 ||
        abs(positionNDCLS.y) > 1.0 ||
        abs(positionNDCLS.z) > 1.0) {
        return 1.0;
    } else {
        //pcf
        ivec2 texSize = textureSize(shadowMap, 0);
        float scale = 1.5;
        float dx = scale * 1.0 / float(texSize.x);
        float dy = scale * 1.0 / float(texSize.y);

        int PCF_SIZE = 3;
        int pcfSizeMinus1 = PCF_SIZE - 1;
        float kernelSize = 2.0 * pcfSizeMinus1 + 1.0;
        float numSamples = kernelSize * kernelSize;

        float shadowCount = 0.0;
        vec2 shadowMapCoord = vec2(positionNDCLS.xy * 0.5 + 0.5);

        for (int x = -pcfSizeMinus1; x <= pcfSizeMinus1; x++) {
            for (int y = -pcfSizeMinus1; y <= pcfSizeMinus1; y++) {
                vec2 pcfCoordinate = shadowMapCoord + vec2(dx * x, dy * y);
                vec3 pcfCoordinatePlusReference = vec3(pcfCoordinate, positionNDCLS.z);

                shadowCount += texture(shadowMap, pcfCoordinatePlusReference).r;
            }
        }

        return mix(0.2, 1.0, shadowCount / numSamples);
    }
}
