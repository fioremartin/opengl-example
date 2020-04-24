#include "MainComponent.h"

namespace glsl
{

    String getVertexShader()
    {
        return String
        {
            " #version 150 core                         \n"
            "                                           \n"
            " in vec3 aPos;                             \n"
            "                                           \n"
            " out vec4 colour;                          \n"
            "                                           \n"
            " uniform mat4 model;                       \n"
            " uniform mat4 projection;                  \n"
            "                                           \n"
            " void main()                               \n"
            " {                                                         \n"
            "   gl_Position = projection * model * vec4(aPos, 1.0f);    \n"
            "   colour = vec4(1.0f, 0.5f, 0.2f, 1.0f);                  \n"
            " }                                                         \n"
        };
    }

    String getFragmentShader()
    {
        return String
        {
            "#version 150 core     \n"
            "out vec4 FragColor;   \n"
            "                      \n"
            "in vec4 colour;       \n"
            "                      \n"
            "void main()           \n"
            "{                     \n"
            "  FragColor = colour; \n"
            "}                     \n"
        };
    }
}



//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
}

MainComponent::~MainComponent()
{
    // This shuts down the GL system and stops the rendering calls.
    shutdownOpenGL();
}

//==============================================================================
void MainComponent::initialise()
{
    // Initialise GL objects for rendering here.
    // Initialise GL objects for rendering here.
    std::unique_ptr<OpenGLShaderProgram> newShader = std::make_unique<OpenGLShaderProgram>(openGLContext);

    if (newShader->addVertexShader(glsl::getVertexShader()) &&
        newShader->addFragmentShader(glsl::getFragmentShader()) &&
        newShader->link())
    {
        shader = std::move(newShader);
        shader->use();

        modelUniform = std::make_unique<OpenGLShaderProgram::Uniform>(*shader, "model");
        projectionUniform = std::make_unique<OpenGLShaderProgram::Uniform>(*shader, "projection");

        // Create id for VBOs and EBOs.
        gl.glGenBuffers(1, &squareVBO);
        gl.glGenBuffers(1, &squareEBO);

        gl.glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
        gl.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
        gl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
}

void MainComponent::shutdown()
{
    // Free any GL objects created for rendering here.
}

void MainComponent::render()
{
    // This clears the context with a black background.
    OpenGLHelpers::clear (Colours::black);
    
    const int w = width.load();
    const int h = height.load();

    glViewport(0, 0, w, h);

    if (shader == nullptr)
        return;

    shader->use();

    AffineTransform identity{};
    projectionUniform->setMatrix4(getOrthographicMatrix().mat, 1, false);

    gl.glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);

    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    gl.glEnableVertexAttribArray(0);

    const float squareSize = std::min(w, h) / 10.0f;
    const float hPosition = jmap<float>(std::sin(getFrameCounter() * 0.01), -1.0, 1.0, 0, w);

    AffineTransform squareTransform{ identity.scaled(squareSize, squareSize).translated(hPosition, h/2) };

    modelUniform->setMatrix4(Matrix3D<float>(squareTransform).mat, 1, false);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // You can add your component specific drawing code here!
    // This will draw over the top of the openGL background.
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto bounds = getLocalBounds();

    width.store(bounds.getWidth());
    height.store(bounds.getHeight());
}


Matrix3D<float> MainComponent::getOrthographicMatrix() const
{
    const float left = 0.0f;
    const float right = width.load();
    const float bottom = height.load();
    const float top = 0.0f;


    const float topMbottom = top - bottom;
    const float topPbottom = top + bottom;
    const float rightMleft = right - left;
    const float rightPLeft = right + left;

    // https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/orthographic-projection-matrix
    return Matrix3D<float>
    {
        2.0f / rightMleft, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / topMbottom, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -rightPLeft / rightMleft, -topPbottom / topMbottom, 0.0f, 1.0f
    };
}

