#version 330 core

// Vertex position and normal (inputs)
in vec3 vVertex;   // Vertex position
in vec3 vNormal;   // Vertex normal (should be passed from VBO)

// Uniforms for transformations
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

// Uniforms for light
uniform vec3 lightPos;      // Position of the point light source
uniform vec3 lightColor;    // Color of the light
uniform vec3 viewPos;       // Camera/viewer position

// Uniforms for material
uniform vec3 objectColor;   // Color of the object (used in diffuse lighting)

// Uniform to toggle between point light, normal display, and Gouraud shading
uniform int mode;  // 0 for point light, 1 for colored normals, 2 for Gouraud shading

// Output to fragment shader
out vec3 fColor;
out vec3 FragPos;
out vec3 normal;

void main() {
    // Transform vertex to world space
    FragPos = vec3(vModel * vec4(vVertex, 1.0));
    
    // Transform normal to world space and normalize
    normal = normalize(mat3(transpose(inverse(vModel))) * vNormal);

    if (mode == 2) {
        // Ambient lighting
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse lighting
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Specular lighting
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess factor
        vec3 specular = specularStrength * spec * lightColor;

        // Combine all components
        vec3 finalColor = (ambient + diffuse + specular) * objectColor;
        
        fColor = finalColor;

    } else if (mode == 1) {
        // Normal display mode (colored normals)
        fColor = (vNormal + 1.0) / 2.0;

    }
    // Final position transformation for the vertex
    gl_Position = vProjection * vView * vec4(FragPos, 1.0);
}
