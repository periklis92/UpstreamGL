#pragma once

class Shader;

class Material
{
public:
    void Bind();
private:
    Shader* m_Shader;
};