#version 330 core

in vec3 FragPos;
in vec3 normal;
in vec3 fColor;

uniform int mode;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;
uniform vec3 spotlightDirection;

uniform float cutoffAngle; // Add this line to declare the cutoff angle uniform
uniform float innerCutoffAngle; // Add this line to declare the inner cutoff angle uniform

vec4 spotLight()
{
    vec3 lightDir = normalize(lightPos - FragPos);

    float theta = dot(lightDir, normalize(spotlightDirection)); // Calculate the angle between the spotlight direction and the vector from the fragment to the light source

    float cutOff = innerCutoffAngle;

    if (theta > cutOff)
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

        return vec4(finalColor, 1.0); // Set the final output color
    }
    else
    {
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * objectColor;

        return vec4(ambient, 1.0);
    }
}

vec4 spotLightwithShadow()
{
    vec3 lightDir = normalize(lightPos - FragPos);

    float theta = dot(lightDir, normalize(spotlightDirection)); // Calculate the angle between the spotlight direction and the vector from the fragment to the light source

    float cutOff = cutoffAngle;
    float innerCutOff = innerCutoffAngle;
    
    if (theta > cutOff)
    {
        float intensity = (theta - cutOff) / (innerCutOff - cutOff);
        intensity = clamp(intensity, 0.0, 1.0);

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

        diffuse *= intensity;
        specular *= intensity;

        // Combine all components
        vec3 finalColor = (ambient + diffuse + specular) * objectColor;

        return vec4(finalColor, 1.0); // Set the final output color
    }
    else
    {
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * objectColor;

        return vec4(ambient, 1.0);
    }
}

vec4 headlight()
{
    // Headlight: Set light position to camera position and direction to camera forward
    vec3 lightPos = viewPos;
    vec3 lightDir = normalize(lightPos - FragPos);

    float theta = dot(lightDir, normalize(spotlightDirection)); // Calculate the angle between the spotlight direction and the vector from the fragment to the light source

    float cutOff = innerCutoffAngle;

    if (theta > cutOff)
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

        return vec4(finalColor, 1.0); // Set the final output color
    }
    else
    {
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * objectColor;

        return vec4(ambient, 1.0);
    }
}

void main()
{
    vec3 normal = normalize(normal); // Normalize normals
    if (mode == 2 || mode == 1)
    {
        FragColor = vec4(fColor, 1.0);
    }
    else if (mode == 4)
    {
        FragColor = spotLight();
    }
    else if (mode == 5)
    {
        FragColor = spotLightwithShadow();
    }
    else if (mode == 6)
    {
        FragColor = headlight();
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
