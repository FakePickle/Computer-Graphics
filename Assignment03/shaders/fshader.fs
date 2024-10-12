#version 330 core

in vec3 FragPos; // Position of the fragment in world space
in vec3 normal;  // normal vector from vertex shader
in vec3 fColor;

uniform int mode;
uniform vec3 lightPos;    // Position of the light source
uniform vec3 viewPos;     // Position of the camera/view
uniform vec3 lightColor;  // Color of the light
uniform vec3 objectColor; // Base color of the object

out vec4 FragColor; // Final color output

void main()
{
    if (mode == 2 || mode == 1)
    {
        FragColor = vec4(fColor, 1.0); // Set the final output color
    }
    else
    {
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

        FragColor = vec4(finalColor, 1.0); // Set the final output color
    }
}
