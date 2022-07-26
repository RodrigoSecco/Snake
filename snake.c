/***************************************************************
*                                                              *
*   raylib - classico jogo: Jogo da cobrinha                   *
*                                                              *
*   Esse jogo foi criado usando raylib v1.3 (www.raylib.com)   *
*                                                              *
***************************************************************/

#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     31
#define SNAKE_SPEED     31

//----------------------------------------------------------------------------------
// Definição de Types e Struct
//----------------------------------------------------------------------------------
typedef struct Snake {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Food {
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

//------------------------------------------------------------------------------------
// Variaveis Globais
//------------------------------------------------------------------------------------
static const int screenWidth = 1100;
static const int screenHeight = 700;
static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;
static Food fruit = { 0 };
static Snake snake[SNAKE_LENGTH] = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static bool allowMove = false;
static Vector2 offset = { 0 };
static int counterTail = 0;
static int codigo = 3;
static int pontuacao = 0;

//------------------------------------------------------------------------------------
// Declaração de Funções do Módulo (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Inicialização do game
static void UpdateGame(void);       // Atualizar jogo (um quadro)
static void DrawGame(void);         // Desenhar jogo (um quadro)
static void UnloadGame(void);       // Descarregar jogo
static void UpdateDrawFrame(void);  // Atualizar e desenhar (um quadro)

//------------------------------------------------------------------------------------
// Ponto de entrada principal do programa
//------------------------------------------------------------------------------------
int main(void)
{
    // Inicialização
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "  JOGO DA COBRINHA     Desenvolvido por:   Rodrigo Secco,    Murilo Magossi,    Gustavo Batista");

    InitGame();


    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Principal loop do jogo
    while (!WindowShouldClose())    // Detectar o botão de fechamento da janela ou a tecla ESC
    {
        // Atualizar e desenhar
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
    // Desinicialização
    //--------------------------------------------------------------------------------------

    CloseWindow();        // Fechar janela e contexto OpenGL
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Definições de Funções do Módulo (local)
//------------------------------------------------------------------------------------

// Inicializar variáveis ​​do jogo
void InitGame(void)
{
    framesCounter = 0;
    gameOver = true;
    pause = false;
    pontuacao = 0;

    counterTail = 1;
    allowMove = false;

    offset.x = screenWidth%SQUARE_SIZE;
    offset.y = screenHeight%SQUARE_SIZE;

    for(int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){ offset.x/2, offset.y/2 };
        snake[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
        snake[i].speed = (Vector2){ SNAKE_SPEED, 0 };

        if(i == 0) snake[i].color = DARKGREEN;
        else snake[i].color = GREEN;

        snakePosition[i] = (Vector2){ 0.0f, 0.0f };
    }

    fruit.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
    fruit.color = RED;
    fruit.active = false;
}

// Atualizar jogo (um quadro)
void UpdateGame(void)
{
    if(!gameOver)
    {
        if(IsKeyPressed('P')){
            pause = !pause;
        }
        if(pause == true && IsKeyPressed('X')){
            gameOver = true;
        }

        if(!pause)
        {
            // Controles
            if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ SNAKE_SPEED, 0 };
                allowMove = false;
            }
            if (IsKeyPressed('D') && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ SNAKE_SPEED, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ -SNAKE_SPEED, 0 };
                allowMove = false;
            }
            if (IsKeyPressed('A') && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ -SNAKE_SPEED, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, -SNAKE_SPEED };
                allowMove = false;
            }
            if (IsKeyPressed('W') && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, -SNAKE_SPEED };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, SNAKE_SPEED };
                allowMove = false;
            }
            if (IsKeyPressed('S') && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, SNAKE_SPEED };
                allowMove = false;
            }

            // Movimentação da cobra
            for (int i = 0; i < counterTail; i++) snakePosition[i] = snake[i].position;
            int velocidade = 0;
            if(codigo == 1){
                velocidade = 7;
            }
            if(codigo == 2){
                velocidade = 6;
            }
            if(codigo == 3){
                velocidade = 5;
            }
            if (framesCounter > velocidade) // -- velocidade da cobra
            {
                framesCounter = 0;

                for (int i = 0; i < counterTail; i++)
                {
                    if (i == 0)
                    {
                        snake[0].position.x += snake[0].speed.x;
                        snake[0].position.y += snake[0].speed.y;
                        allowMove = true;
                    }
                    else snake[i].position = snakePosition[i-1];
                }
            }

            // Colisão com a parede
            if(codigo == 2 || codigo == 3){
                if(((snake[0].position.x) > (screenWidth - offset.x)) || ((snake[0].position.y) > (screenHeight - offset.y)) ||(snake[0].position.x < 0) || (snake[0].position.y < 0))
                {
                    gameOver = true;
                }
            }else{
            // Não colisão
                if((snake[0].position.x) > (screenWidth - offset.x)){
                    snake[0].position.x = 7.5;
                }
                if((snake[0].position.y) > (screenHeight - offset.y)){
                    snake[0].position.y = 9;
                }
                if(snake[0].position.x < 0){
                    snake[0].position.x = screenWidth - offset.x - 23.5;
                }
                if(snake[0].position.y < 0){
                    snake[0].position.y = screenHeight - offset.y - 22;
                }
            }

            // Colisão consigo mesmo
            for (int i = 1; i < counterTail; i++)
            {
                if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y)){
                        gameOver = true;
                }
            }

            // Calculo da posição da fruta
            if (!fruit.active)
            {
                fruit.active = true;
                fruit.position = (Vector2){ GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2, GetRandomValue(0, (screenHeight/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.y/2 };

                for (int i = 0; i < counterTail; i++)
                {
                    while ((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y))
                    {
                        fruit.position = (Vector2){ GetRandomValue(0, (screenWidth/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.x/2, GetRandomValue(0, (screenHeight/SQUARE_SIZE) - 1)*SQUARE_SIZE + offset.y/2 };
                        i = 0;
                    }
                }
            }

            // Colisão
            if ((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
                (snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y))
            {
                snake[counterTail].position = snakePosition[counterTail - 1];
                if(codigo == 3){
                    counterTail += 2;
                }else{
                    counterTail++;
                }
                fruit.active = false;
                pontuacao++;
            }
            framesCounter++;
        }
    }
    else
    {
        if (IsKeyPressed('F'))
        {
            codigo=1;
            InitGame();
            gameOver = false;
        }
        if (IsKeyPressed('M'))
        {
            codigo=2;
            InitGame();
            gameOver = false;
        }
        if (IsKeyPressed('D'))
        {
            codigo=3;
            InitGame();
            gameOver = false;
        }
    }
}


// Desenhar jogo (um quadro)
void DrawGame(void)
{
    BeginDrawing();
        if(codigo==1){
            ClearBackground(WHITE);
        }
        if(codigo==2){
            ClearBackground(SKYBLUE);
        }
        if(codigo == 3){
            ClearBackground(BLACK);
        }

        if (!gameOver)
        {
            // Desenhar quadriculados
            for (int i = 0; i < screenWidth/SQUARE_SIZE + 1; i++)
            {
                if(codigo == 2){
                    DrawLineV((Vector2){SQUARE_SIZE*i + offset.x/2, offset.y/2}, (Vector2){SQUARE_SIZE*i + offset.x/2, screenHeight - offset.y/2}, BLACK);
                }else{
                    DrawLineV((Vector2){SQUARE_SIZE*i + offset.x/2, offset.y/2}, (Vector2){SQUARE_SIZE*i + offset.x/2, screenHeight - offset.y/2}, LIGHTGRAY);
                }
            }

            for (int i = 0; i < screenHeight/SQUARE_SIZE + 1; i++)
            {
                if(codigo == 2){
                    DrawLineV((Vector2){offset.x/2, SQUARE_SIZE*i + offset.y/2}, (Vector2){screenWidth - offset.x/2, SQUARE_SIZE*i + offset.y/2}, BLACK);
                }else{
                    DrawLineV((Vector2){offset.x/2, SQUARE_SIZE*i + offset.y/2}, (Vector2){screenWidth - offset.x/2, SQUARE_SIZE*i + offset.y/2}, LIGHTGRAY);
                }
            }
            DrawText(TextFormat("SCORE: %02i", pontuacao), 20, 10, 40, ORANGE);

            // Desenhar a cobra
            for (int i = 0; i < counterTail; i++) DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

            // Desenhar fruta
            DrawCircle(fruit.position.x+15, fruit.position.y+15, 15, fruit.color);

            if (pause) DrawText("GAME PAUSED \n\nPara acessar o MENU\npressione X", screenWidth/2.5 - MeasureText("GAME PAUSED", 40)/2, screenHeight/3 - 40, 60, RED);
        }
        else{
            DrawText("\nSelecione o MODO de JOGO:""\n\n>pressione[F] - FACIL""\n\n>pressione[M] - MEDIO""\n\n>pressione[D] - DIFICIL", GetScreenWidth()/2.5 - MeasureText("Selecione o MODO de JOGO:""\nFacil[1]""\nMedio[2]""\nDificil[3]", 20)/2, GetScreenHeight()/2 - 200, 35, GREEN);
        }
        EndDrawing();
}

// Atualizar e desenhar (um quadro)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

