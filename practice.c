#include <raylib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Point Management System");

    SetTargetFPS(60);

    Font font = GetFontDefault();
    Color cardColor = (Color){255, 255, 255, 230};
    Color bgColor = (Color){0, 180, 255, 255};
    Color buttonColor = (Color){255, 255, 255, 255};
    Color buttonHover = (Color){220, 220, 255, 255};
    Color textColor = (Color){0, 80, 180, 255};

    Rectangle card = (Rectangle){screenWidth / 2.0f - 150, 100, 300, 420};
    Rectangle buttons[4];
    for (int i = 0; i < 4; i++)
    {
        buttons[i] = (Rectangle){card.x + 40, card.y + 200 + (i * 50), 220, 35};
    }

    const char *labels[4] = {
        "View Routes",
        "Check Seat Availability",
        "Book a Seat",
        "View Time Schedule"};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(bgColor);

        DrawRectangleRounded(card, 0.1f, 10, cardColor);
        DrawRectangleRoundedLines(card, 0.1f, 10, 2, Fade(BLUE, 0.3f));

        DrawText("Student Dashboard", (int)(card.x + 40), (int)(card.y + 20), 20, textColor);

        int textX = (int)(card.x + 20);
        int textY = (int)(card.y + 60);
        DrawText("Name: Ali Ahmed", textX, textY, 16, BLACK);
        DrawText("Batch: BSCS-23", textX, textY + 25, 16, BLACK);
        DrawText("Roll No: 23K-1234", textX, textY + 50, 16, BLACK);
        DrawText("Point Bus: Bus 5", textX, textY + 75, 16, BLACK);
        DrawText("Route: Green Line", textX, textY + 100, 16, BLACK);

        for (int i = 0; i < 4; i++)
        {
            bool hovered = CheckCollisionPointRec(GetMousePosition(), buttons[i]);
            DrawRectangleRounded(buttons[i], 0.3f, 10, hovered ? buttonHover : buttonColor);
            DrawRectangleRoundedLines(buttons[i], 0.3f, 10, 1, Fade(BLUE, 0.3f));

            int textWidth = MeasureText(labels[i], 14);
            DrawText(labels[i], (int)(buttons[i].x + (buttons[i].width - textWidth) / 2), (int)(buttons[i].y + 10),14, textColor);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
