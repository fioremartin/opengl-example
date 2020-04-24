
#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public OpenGLAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void initialise() override;
    void shutdown() override;
    void render() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    
    std::unique_ptr<OpenGLShaderProgram> shader;
    std::unique_ptr<OpenGLShaderProgram::Uniform> modelUniform;
    std::unique_ptr<OpenGLShaderProgram::Uniform> projectionUniform;

    GLuint squareVBO;
    GLuint squareEBO;

    float vertices[12] = 
    {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[6] = 
    {  
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    const OpenGLExtensionFunctions& gl = openGLContext.extensions;

    std::atomic<int> width;
    std::atomic<int> height;


    Matrix3D<float> getOrthographicMatrix() const;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
