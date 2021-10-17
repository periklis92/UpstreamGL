#pragma once

class Shader;

class Material
{
public:
    void Bind();
    Shader* GetShader() { return m_Shader; }
private:
    Shader* m_Shader;
};