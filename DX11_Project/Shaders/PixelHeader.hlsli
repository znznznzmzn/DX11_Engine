// Pixel Header

// Pixel Constant Buffer
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp  : register(s0);

cbuffer LightBuffer  : register(b0) { 
	float3 light_direction;
	float shininess;
    float4 ambient;
    float4 light_color;
}
cbuffer MaterialBuffer : register(b1) {
	float4 mDiffuse;
	float4 mSpecular;
	float4 mAmbient;
	float mShininess;
	int hasDiffuse;
	int hasSpecular;
	int hasNormal;
}
cbuffer PointLight : register(b2) {
	float3 pointPosition;
	float  pointRange;
	float4 pointColor;
}
cbuffer SpotLight : register(b3) {
	float3 spotPosition;
	float  spotRange;
	float3 spotDirection;
	float  spotRadius;
	float4 spotColor;
}
cbuffer ColorBuffer : register(b4) { float4 color; } 

// Pixel Function
 // Mapping
float3 Mapping_Normal(float3 T, float3 B, float3 N, float2 uv) {
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
    [flatten]
    if(hasNormal) {
        float3x3 TBN = float3x3(T, B, N);
        N = normalMap.Sample(samp, uv).rgb * 2.0f - 1.0f;
        N = normalize(mul(N, TBN));
    }
    return N;
}
float4 Mapping_Specular(float2 uv) {
    [flatten]
    if(hasSpecular) return specularMap.Sample(samp, uv);
    return float4(1, 1, 1, 1);
}

 // Light
float4 Lighting_Directional(float4 diffuseColor, float4 specularIntensity, float3 normal, float3 viewDir) {
    float3 toLight = -normalize(light_direction);
	float NDotL = saturate(dot(toLight, normal));
    float4 finalColor = light_color * NDotL * mDiffuse;
	[flatten]
	if(NDotL > 0) {
        float3 halfWay = normalize(viewDir + toLight);
		float NDotH = saturate(dot(normal, halfWay));
		
        finalColor += light_color * pow(NDotH, shininess) * specularIntensity * mSpecular;
    }
	return finalColor * diffuseColor;
}
float4 Lighting_Point(float4 diffuseColor, float4 specularIntensity, float3 normal, float3 viewDir, float3 pos) {
	float3 toLight = pointPosition - pos;
	float distanceToLight = length(toLight);
	toLight /= distanceToLight;
	float NDotL = saturate(dot(toLight, normal));
	float4 result = pointColor * NDotL * mDiffuse;
	[flatten]
	if (NDotL > 0) {
        float3 halfWay = normalize(viewDir + toLight);
		float NDotH = saturate(dot(normal, halfWay));
		result += pointColor * pow(NDotH, shininess) * specularIntensity * mSpecular;
	}
	float distanceToLightNormal = 1.0f - saturate(distanceToLight / pointRange);
	float attention = distanceToLightNormal * distanceToLightNormal;
	
	return result * diffuseColor * attention;
}
float4 Lighting_Spot(float4 diffuseColor, float4 specularIntensity, float3 normal, float3 viewDir, float3 pos) {
    float3 toLight = spotPosition - pos;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
    float NDotL = saturate(dot(toLight, normal));
    float4 result = spotColor * NDotL * mDiffuse;
    [flatten]
    if (NDotL > 0) {
        float3 halfWay = normalize(viewDir + toLight);
        float NDotH = saturate(dot(normal, halfWay));
        
        result += spotColor * pow(NDotH, shininess) * specularIntensity * mSpecular;
    }
    
    float cosAngle = dot(-normalize(spotDirection), toLight);
    
    float outer = cos(radians(spotRadius));
    float inner = 1.0f / cos(radians(1.0f));
    
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention *= conAttention;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / spotRange);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    return result * diffuseColor * attention * conAttention;
}
