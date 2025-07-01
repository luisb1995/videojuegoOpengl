#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "GameObject.h"
#include "Player.h"
#include "FallingObject.h"
#include <vector>
#include <ctime> 
#include <algorithm>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
int switcheable = 1;

void processInput(GLFWwindow *window, int switcheable);
Player* jugador = nullptr;
float velocidad_jugador = 0.0003f;


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

enum Scene
{
    MENU,
    START_COUNTDOWN,
    GAME_COUNT,
    GAME,
    END,
};
unsigned int countdownTextures[5]; // una para cada número
bool isCountingDown = true;
float countdownStart = 0.0f;
int currentScene = MENU;
float tiempoDeJuego = 25.0f;
float tiempoTranscurrido = 0.0f;
bool juegoTerminado = false;
int frutasAtrapadas = 0;
unsigned int texturaFrutasAtrapadas;
unsigned int numeros[10];

float countdownStartTime = 0.0f;

float vertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};
unsigned int indices[] = {0, 1, 3, 1, 2, 3};

unsigned int VBO, VAO, EBO, texture, texturaTitulo, texturaMensaje, TexturaJugador, TexturaJugadorD;

int main()
{
    glfwInit();
    srand(static_cast<unsigned int>(time(0)));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mini Juego OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error al inicializar GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("vertex.glsl", "fragment.glsl");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int width, height, nrChannels;
    unsigned char *data;

    // === Cargar FRUTA ===
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("fruta3.png", &width, &height, &nrChannels, 4);
    if (data)
    {
        GLenum format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    // === Cargar TÍTULO ===
    glGenTextures(1, &texturaTitulo);
    glBindTexture(GL_TEXTURE_2D, texturaTitulo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("minijuego.png", &width, &height, &nrChannels, 4);
    if (data)
    {
        GLenum format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    // === Cargar MENSAJE ===
    glGenTextures(1, &texturaMensaje);
    glBindTexture(GL_TEXTURE_2D, texturaMensaje);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("presiona.png", &width, &height, &nrChannels, 4);
    if (data)
    {
        GLenum format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    // === Cargar TEXTURAS DE CUENTA REGRESIVA ===
    char filename[20];
    for (int i = 0; i < 5; ++i)
    {
        sprintf(filename, "count_%d.png", 5 - i); // "count_5.png", ..., "count_1.png"
        glGenTextures(1, &countdownTextures[i]);
        glBindTexture(GL_TEXTURE_2D, countdownTextures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load(filename, &width, &height, &nrChannels, 4);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Error al cargar " << filename << std::endl;
        }
        stbi_image_free(data);
    }

    glGenTextures(1, &TexturaJugador);
    glBindTexture(GL_TEXTURE_2D, TexturaJugador);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char *basketData = stbi_load("jugador.png", &width, &height, &nrChannels, 4);
    if (basketData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, basketData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Error al cargar textura de cesta" << std::endl;
    }
    stbi_image_free(basketData);







        glGenTextures(1, &TexturaJugadorD);
    glBindTexture(GL_TEXTURE_2D, TexturaJugadorD);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char *basketDataD = stbi_load("jugadord.png", &width, &height, &nrChannels, 4);
    if (basketDataD)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, basketDataD);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Error al cargar textura de cesta" << std::endl;
    }
    stbi_image_free(basketDataD);

    unsigned int texturaTiempo, texturaVolver;

    // Cargar "fin del juego.png"
    glGenTextures(1, &texturaTiempo);
    glBindTexture(GL_TEXTURE_2D, texturaTiempo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("findeljuego.png", &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    // Cargar "volver.png"
    glGenTextures(1, &texturaVolver);
    glBindTexture(GL_TEXTURE_2D, texturaVolver);
    // mismos parámetros
    data = stbi_load("volver.png", &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    // Cargar "frutas_atrapadas.png"
    glGenTextures(1, &texturaFrutasAtrapadas);
    glBindTexture(GL_TEXTURE_2D, texturaFrutasAtrapadas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("frutasatrapadas.png", &width, &height, &nrChannels, 4); // 🖼️ Usa tu nombre real
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    // Cargar números del 0 al 9
    for (int i = 0; i < 10; ++i) {
    glGenTextures(1, &numeros[i]);
    glBindTexture(GL_TEXTURE_2D, numeros[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string filename = "numeros/" + std::to_string(i) + ".png";
    data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Error al cargar: " << filename << std::endl;
    }
    stbi_image_free(data);
}


GameObject barraTiempo(
    glm::vec2(-1.0f, 0.95f), // Posición esquina superior izquierda
    glm::vec2(2.0f, 0.05f),  // Tamaño inicial: ocupa ancho completo
    0                        // Sin textura, usaremos color sólido
);

std::vector<FallingObject> frutas;  // Frutas activas en escena
float frutaSpawnTimer = 0.0f;
float frutaSpawnInterval = 1.0f; // cada segundo

std::vector<unsigned int> texturasFrutas;
std::vector<std::string> nombresFrutas = {
    "frutas/banana.png",
    "frutas/apple.png",
    "frutas/grape.png",
    "frutas/pear.png"
};

for (const auto & nombre : nombresFrutas) {
    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(nombre.c_str(), &width, &height, &nrChannels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        texturasFrutas.push_back(texID);
    }
    else
    {
        std::cout << "Error al cargar imagen de fruta: " << nombre << std::endl;
    }
    stbi_image_free(data);
}




    jugador = new Player( 
      //  glm::vec2(0.0f, -0.8f),
      //  glm::vec2(0.2f, 0.1f),

    glm::vec2(0.0f, -0.8f),
    glm::vec2(0.8f * 0.5f, 1.3333f * 0.5f),
        TexturaJugador,
        TexturaJugadorD
    );

    static float velocidad_jugador = 0.08f;


    while (!glfwWindowShouldClose(window))
    {
        processInput(window, switcheable);
        glClear(GL_COLOR_BUFFER_BIT);
        

        switch (currentScene)
        {
        case MENU:
        {
            glClearColor(0.0f, 0.2f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            shader.use();
            glUniform1f(glGetUniformLocation(shader.ID, "uCountdown"), -1.0f);
            int transformLoc = glGetUniformLocation(shader.ID, "transform");

            // Fruta animada
            float time = glfwGetTime();
            float offsetY = sin(time * 2.0f) * 0.1f;
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(0.0f, offsetY, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Título
            glm::mat4 transformTitulo = glm::mat4(1.0f);
            transformTitulo = glm::translate(transformTitulo, glm::vec3(0.0f, 0.92f, 0.0f));
            transformTitulo = glm::scale(transformTitulo, glm::vec3(0.9f, 0.25f, 1.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformTitulo));
            glBindTexture(GL_TEXTURE_2D, texturaTitulo);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Mensaje
            glm::mat4 transformMensaje = glm::mat4(1.0f);
            transformMensaje = glm::translate(transformMensaje, glm::vec3(0.0f, 0.68f, 0.0f));
            transformMensaje = glm::scale(transformMensaje, glm::vec3(0.9f, 0.2f, 1.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMensaje));
            glBindTexture(GL_TEXTURE_2D, texturaMensaje);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            tiempoTranscurrido = 0.0f;
        juegoTerminado = false;
        barraTiempo.Size.x = 2.0f;
         frutasAtrapadas= 0;

            break;
        }
        case GAME_COUNT:
        {
            switcheable = 0; // 0
            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            if (isCountingDown)
            {
                float currentTime = glfwGetTime();
                if (countdownStart == 0.0f)
                    countdownStart = currentTime;

                float elapsed = currentTime - countdownStart;
                float remaining = 5.0f - elapsed;

                if (remaining <= 0.0f)
                {
                    isCountingDown = false;
                }
                else
                {
                    shader.use();
                    int transformLoc = glGetUniformLocation(shader.ID, "transform");
                    glm::mat4 transform = glm::mat4(1.0f);
                    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

                    // Pasar countdown y textura del número actual
                    glUniform1f(glGetUniformLocation(shader.ID, "uCountdown"), remaining);

                    int index = std::min(4, std::max(0, 5 - (int)std::ceil(remaining)));
                    glBindTexture(GL_TEXTURE_2D, countdownTextures[index]);

                    glBindVertexArray(VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
            else
            {
                currentScene = GAME;
            }
            break;
        }
case GAME: {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glUniform1f(glGetUniformLocation(shader.ID, "uCountdown"), -1.0f);
    
    jugador->Draw(shader, VAO, false);

    glBindVertexArray(VAO);
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - countdownStartTime;
    countdownStartTime = currentTime;

    // ⏱️ Spawnear frutas
    frutaSpawnTimer += deltaTime;
    if (frutaSpawnTimer >= frutaSpawnInterval) {
        frutaSpawnTimer = 0.0f;

        float x = ((rand() % 100) / 100.0f) * 2.0f - 1.0f;
        float y = 1.2f;
        float speed = 0.3f + (rand() % 100) / 500.0f;

        if (!texturasFrutas.empty()) {
            int idx = rand() % texturasFrutas.size();
            unsigned int texturaElegida = texturasFrutas[idx];
            frutas.emplace_back(glm::vec2(x, y), glm::vec2(0.2f, 0.2f), texturaElegida, speed);
        }
    }

    // 🧹 Actualizar y dibujar frutas
    for (auto& fruta : frutas) {
        if (fruta.IsActive) {
            fruta.Update(deltaTime);
            fruta.Draw(shader, VAO);

            if (jugador && jugador->CheckCollision(fruta)) {
                fruta.IsActive = false;
                 frutasAtrapadas++;
                // suma puntaje, efecto, etc.
            }
        }
    }

    // 🗑️ Eliminar frutas que ya no están activas
    frutas.erase(std::remove_if(frutas.begin(), frutas.end(),
                                [](const FallingObject& f) { return !f.IsActive; }),
                 frutas.end());

        if (!juegoTerminado) {
            // ⏱️ Actualizar tiempo
            tiempoTranscurrido += deltaTime;

            // 🛑 Verificar si se terminó el juego
            if (tiempoTranscurrido >= tiempoDeJuego) {
                juegoTerminado = true;
                currentScene = END;
            }

            // 🎯 Calcular porcentaje restante
            float porcentaje = std::max(0.0f, 1.0f - tiempoTranscurrido / tiempoDeJuego);
            barraTiempo.Size.x = 2.0f * porcentaje;

            // 🎨 Color dinámico según tiempo restante
            glm::vec3 color;
            if (porcentaje > 0.5f)
                color = glm::vec3(0.2f, 0.8f, 0.3f); // Verde
            else if (porcentaje > 0.25f)
                color = glm::vec3(0.9f, 0.8f, 0.0f); // Amarillo
            else
                color = glm::vec3(0.9f, 0.2f, 0.2f); // Rojo

            // 🎨 Dibujar barra
            shader.use();
            shader.setBool("useSolidColor", true);
            shader.setVec3("solidColor", color.x, color.y, color.z);

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(barraTiempo.Position, 0.0f));
            transform = glm::scale(transform, glm::vec3(barraTiempo.Size, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            shader.setBool("useSolidColor", false);
        }
        
            break;
        }
  case END:
        switcheable = 1;
    glClearColor(0.2f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();

    glUniform1f(glGetUniformLocation(shader.ID, "uCountdown"), -1.0f);

            // Mostrar mensaje "¡Tiempo!"
            glm::mat4 transformTiempo = glm::mat4(1.0f);
            transformTiempo = glm::translate(transformTiempo, glm::vec3(0.0f, 0.5f, 0.0f));
            transformTiempo = glm::scale(transformTiempo, glm::vec3(0.8f, 0.25f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transformTiempo));
            glBindTexture(GL_TEXTURE_2D, texturaTiempo);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // 1️⃣ Imagen: "frutas atrapadas:"
            glm::mat4 transformFrutas = glm::mat4(1.0f);
            transformFrutas = glm::translate(transformFrutas, glm::vec3(0.0f, 0.2f, 0.0f));
            transformFrutas = glm::scale(transformFrutas, glm::vec3(0.8f, 0.2f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transformFrutas));
            glBindTexture(GL_TEXTURE_2D, texturaFrutasAtrapadas);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

           // 2️⃣ Mostrar número de frutas atrapadas debajo del título
            std::string numeroTexto = std::to_string(frutasAtrapadas);
            float baseX = -0.05f * numeroTexto.size();
            for (int i = 0; i < numeroTexto.size(); ++i) {
                int digito = numeroTexto[i] - '0';
                glm::mat4 transformNumero = glm::mat4(1.0f);
                transformNumero = glm::translate(transformNumero, glm::vec3(baseX + i * 0.1f, 0.05f, 0.1f)); // ⬅️ Subimos un poco
                transformNumero = glm::scale(transformNumero, glm::vec3(0.08f, 0.15f, 1.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transformNumero));
                glBindTexture(GL_TEXTURE_2D, numeros[digito]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            // Mostrar mensaje "Presiona ENTER"
            glm::mat4 transformVolver = glm::mat4(1.0f);
            transformVolver = glm::translate(transformVolver, glm::vec3(0.0f, -0.3f, 0.0f));
            transformVolver = glm::scale(transformVolver, glm::vec3(0.7f, 0.2f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transformVolver));
            glBindTexture(GL_TEXTURE_2D, texturaVolver);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, int switcheable)
{
    static bool enterPressed = false;

    if(switcheable == 1) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !enterPressed)
    {
        if (currentScene == MENU)
            currentScene = GAME_COUNT;
        else if (currentScene == GAME_COUNT)
        {
            currentScene = END;
        }
        else if (currentScene == END)
            currentScene = MENU;
             isCountingDown = true;
             countdownStart = 0.0f;
        enterPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
        enterPressed = false;
} else {
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            jugador->Move(-velocidad_jugador);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            jugador->Move(velocidad_jugador);
        }
}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
