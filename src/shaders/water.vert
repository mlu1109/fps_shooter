#version 130
#extension GL_ARB_explicit_attrib_location: enable
#extension GL_ARB_explicit_uniform_location: enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) uniform mat4 modelWorld;
layout(location = 1) uniform mat4 worldView;
layout(location = 2) uniform mat4 viewScreen;
layout(location = 3) uniform mat4 worldViewNormal;

layout(location = 11) uniform float time;

out vec3 reflectedView;
out vec3 refractedView;
out vec3 position;
out vec3 normal;

float y(float x, float z)
{
    return 2 * cos(x * 100 + time/2) * sin(z + time/2);
}

void main(void)
{
    // Simple motion
    position = inPosition;
    position.y = y(position.x, position.z);
    position.z += cos(time/2) * 0.010;
    position.x += sin(time/2) * 0.010;

    normal = mat3(worldViewNormal) * inNormal;
    // Reflection (Reference: Polygons Feel No Pain p. 149)
    vec3 posInViewCoord = vec3(worldView * modelWorld * vec4(position, 1.0));
    vec3 viewDirectionInViewCoord = normalize(posInViewCoord);
    vec3 viewDirectionInWorldCoord = transpose(mat3(worldView)) * viewDirectionInViewCoord;
    vec3 worldNormal = mat3(modelWorld) * inNormal;
    reflectedView = reflect(viewDirectionInWorldCoord, normalize(worldNormal));
    // Refraction
    float ratio = 1.0 / 1.3333;
    refractedView = refract(viewDirectionInWorldCoord, normalize(worldNormal), ratio);
    gl_Position = viewScreen * worldView * modelWorld * vec4(position, 1.0);
}
