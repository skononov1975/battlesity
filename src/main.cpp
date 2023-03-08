#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
}; //массив вертексов (точек, вершин треугольника, по три нормированные координаты)

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
}; // массив цветов этих точек

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}"; // вертексный шейдер

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}"; // фрагментный шейдер


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback (GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);

}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

}

int main(void)
{
   
    /* Initialize the library */
    if (!glfwInit())
    { 
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    // Опции для контекста OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle Sity 22", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

	if(!gladLoadGL())
{
	std::cout<<"Can not load GLAD"<<std::endl;
	return -1;
}


    std::cout << " Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << " OpenGl version: " << glGetString(GL_VERSION) << std::endl;
	

	glClearColor(0,1,0,1);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER); // создаем вертексный шейдер
    glShaderSource(vs, 1, &vertex_shader, nullptr); // передаем ему исходный код верт шейдера
    glCompileShader(vs); // компилируем исх. код 

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER); // то же для фрагментного шейдера
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram(); // создаем шейдерную програму
    glAttachShader(shader_program, vs); // аттачим к ней вертексный 
    glAttachShader(shader_program, fs); // и фрагментный шейдеры
    glLinkProgram(shader_program); // и линкуем программу

    glDeleteShader(vs); // обьекты больше не нужны - удаляем
    glDeleteShader(fs);

    GLuint point_vbo = 0; // создаем вертекс буффер-обьект на видеокарте (для массива точек)
    glGenBuffers(1, &point_vbo); // генерируем буффер 
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo); //подключаем буффер и делаем текущим
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); //связываем с массивом точек

    GLuint colors_vbo = 0;// создаем вертекс буффер-обьект на видеокарте для массива цветов
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);//связываем с массивом цветов
    
    GLuint vao = 0; // создаем вертекс-аррей обьект
    glGenVertexArrays(1, &vao); // генерим его
    glBindVertexArray(vao); // делаем его текущим

    glEnableVertexAttribArray(0); // включаем позицию точек (0)
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo); // подключаем  буффер точек и делаем текущим
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //связываем данные

    glEnableVertexAttribArray(1); // включаем позицию цветов (1)
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);// подключаем  буффер цветов и делаем текущим
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // связываем данные


      /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program); // использовать шейдерную программу
        glBindVertexArray(vao); // делаем текущим вертекс аррей
        glDrawArrays(GL_TRIANGLES, 0, 3); // отрисовка треугольника

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}