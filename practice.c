#include <raylib.h>

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Hello Raylib");

    SetTargetFPS(60);

    float boxWidth = 300;
    float boxHeight = 150;
    float boxX = screenWidth / 2.0f - boxWidth / 2.0f;
    float boxY = screenHeight / 2.0f - boxHeight / 2.0f;

    // Step 5: Main loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);

        DrawRectangle((int)boxX, (int)boxY, (int)boxWidth, (int)boxHeight, RAYWHITE);

        const char *message = "Hello Raylib";
        int fontSize = 30;
        int textWidth = MeasureText(message, fontSize);
        int textX = screenWidth / 2 - textWidth / 2;
        int textY = screenHeight / 2 - fontSize / 2;

        DrawText(message, textX, textY, fontSize, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
