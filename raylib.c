#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUSES 10
#define ROWS 5
#define COLS 5
#define MAX_INPUT 50
#define SEATS_PER_BUS 24

typedef enum
{
    SCREEN_LOGIN,
    SCREEN_MAIN,
    SCREEN_ROUTE_MAP,
    SCREEN_SEAT_AVAILABILITY,
    SCREEN_BOOK_SEAT,
    SCREEN_TIME_SCHEDULE
} Screen;

typedef struct
{
    char name[50];
    char batch[20];
    char rollNo[20];
    char pointBus[20];
    char route[20];
} Student;

typedef struct
{
    char bus[20];
    int seats;
} BusInfo;

typedef struct
{
    bool booked;
    int seatNum;
    Rectangle rect;
} Seat;

Seat busSeats[MAX_BUSES][SEATS_PER_BUS];

Screen currentScreen = SCREEN_LOGIN;
Student currentStudent;

BusInfo busList[MAX_BUSES];
int busCount = 0;
char selectedBus[20] = "Green";

Rectangle buttons[4];
const char *labels[4] = {
    "View Route Map",
    "Check Seat Availability",
    "Book a Seat",
    "View Time Schedule"};

void DrawLoginScreen();
void DrawMainDashboard();
void DrawRouteMap();
void DrawSeatAvailability();
void DrawBookSeat();
void DrawTimeSchedule();
void LoadBuses();
void SaveBuses();
void SetupSeatRects();
int GetSeats(const char *busName);
void BookSeat(const char *busName);

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Student Point Management System");
    SetTargetFPS(60);

    Rectangle card = (Rectangle){screenWidth / 2.0f - 150, 100, 300, 420};
    for (int i = 0; i < 4; i++)
        buttons[i] = (Rectangle){card.x + 40, card.y + 150 + (i * 50), 220, 35};

    LoadBuses();
    SetupSeatRects();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color){0, 180, 255, 255});

        switch (currentScreen)
        {
        case SCREEN_LOGIN:
            DrawLoginScreen();
            break;
        case SCREEN_MAIN:
            DrawMainDashboard();
            break;
        case SCREEN_ROUTE_MAP:
            DrawRouteMap();
            break;
        case SCREEN_SEAT_AVAILABILITY:
            DrawSeatAvailability();
            break;
        case SCREEN_BOOK_SEAT:
            DrawBookSeat();
            break;
        case SCREEN_TIME_SCHEDULE:
            DrawTimeSchedule();
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// ===================== FILE HANDLING =====================
void LoadBuses()
{
    FILE *f = fopen("seats.txt", "r");
    busCount = 0;
    if (f)
    {
        while (!feof(f) && busCount < MAX_BUSES)
        {
            fscanf(f, "%s", busList[busCount].bus);
            busList[busCount].seats = 0;
            for (int i = 0; i < SEATS_PER_BUS; i++)
            {
                int status;
                fscanf(f, "%d", &status);
                busSeats[busCount][i].booked = (status == 1);
                busSeats[busCount][i].seatNum = i + 1;
                if (!busSeats[busCount][i].booked)
                    busList[busCount].seats++;
            }
            busCount++;
        }
        fclose(f);
    }
}

void SaveBuses()
{
    FILE *f = fopen("seats.txt", "w");
    if (f)
    {
        for (int b = 0; b < busCount; b++)
        {
            fprintf(f, "%s", busList[b].bus);
            for (int i = 0; i < SEATS_PER_BUS; i++)
            {
                fprintf(f, " %d", busSeats[b][i].booked ? 1 : 0);
            }
            fprintf(f, "\n");
        }
        fclose(f);
    }
}

int GetSeats(const char *busName)
{
    for (int i = 0; i < busCount; i++)
        if (strcmp(busList[i].bus, busName) == 0)
            return busList[i].seats;
    return 0;
}

void BookSeat(const char *busName)
{
    for (int i = 0; i < busCount; i++)
    {
        if (strcmp(busList[i].bus, busName) == 0 && busList[i].seats > 0)
        {
            busList[i].seats--;
            SaveBuses();
            break;
        }
    }
}

void SetupSeatRects()
{
    int startX = 100, startY = 100;
    int seatW = 50, seatH = 50, gapX = 10, gapY = 10;

    for (int b = 0; b < busCount; b++)
    {
        for (int i = 0; i < SEATS_PER_BUS; i++)
        {
            int row = i / 4;
            int col = i % 4;
            int x = startX + col * (seatW + gapX);
            if (col >= 2)
                x += 20;
            int y = startY + row * (seatH + gapY);
            busSeats[b][i].rect = (Rectangle){x, y, seatW, seatH};
        }
    }
}

// ===================== LOGIN SCREEN =====================
void DrawLoginScreen()
{
    DrawText("Enter Your Details", 250, 50, 28, DARKBLUE);

    static char name[MAX_INPUT] = "";
    static char rollNo[MAX_INPUT] = "";
    static char department[MAX_INPUT] = "";

    static int currentField = 0;
    static bool showError = false;

    const char *labels[] = {"Name:", "Roll No:", "Department:"};
    char *fields[] = {name, rollNo, department};
    int fieldCount = 3;

    for (int i = 0; i < fieldCount; i++)
    {
        DrawText(labels[i], 150, 150 + i * 60, 20, BLACK);

        Rectangle box = {300, 145 + i * 60, 300, 30};
        DrawRectangleRec(box, LIGHTGRAY);
        DrawRectangleLines(box.x, box.y, box.width, box.height, BLUE);

        DrawText(fields[i], box.x + 5, box.y + 5, 20, BLACK);

        if (currentField == i)
            DrawRectangleLinesEx(box, 2, RED);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, box))
                currentField = i;
        }
    }

    DrawText("Press TAB to change field", 150, 380, 18, GRAY);
    DrawText("Press ENTER to submit", 150, 400, 18, GRAY);

    int key = GetCharPressed();
    while (key > 0)
    {
        if (strlen(fields[currentField]) < MAX_INPUT - 1)
        {
            fields[currentField][strlen(fields[currentField])] = (char)key;
            fields[currentField][strlen(fields[currentField]) + 1] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        int len = strlen(fields[currentField]);
        if (len > 0)
            fields[currentField][len - 1] = '\0';
    }

    if (IsKeyPressed(KEY_TAB))
        currentField = (currentField + 1) % fieldCount;

    if (IsKeyPressed(KEY_ENTER))
    {
        bool allFilled = true;
        for (int i = 0; i < fieldCount; i++)
        {
            if (strlen(fields[i]) == 0)
            {
                allFilled = false;
                break;
            }
        }

        if (allFilled)
        {
            strcpy(currentStudent.name, name);
            strcpy(currentStudent.rollNo, rollNo);
            strcpy(currentStudent.batch, department);
            currentScreen = SCREEN_MAIN;
        }
        else
        {
            showError = true;
        }
    }
    if (showError)
    {
        DrawText("Please enter all fields!", 150, 480, 30, RED);
    }
}

// ===================== DASHBOARD =====================
void DrawMainDashboard()
{
    Rectangle card = (Rectangle){400 - 150, 100, 300, 370};
    Color cardColor = (Color){255, 255, 255, 230};
    Color textColor = (Color){0, 80, 180, 255};
    Color buttonColor = (Color){255, 255, 255, 255};
    Color buttonHover = (Color){220, 220, 255, 255};

    DrawRectangleRounded(card, 0.1f, 10, cardColor);
    DrawRectangleRoundedLines(card, 0.1f, 10, 2, Fade(BLUE, 0.3f));

    DrawText("Student Dashboard", (int)(card.x + 40), (int)(card.y + 20), 20, textColor);

    int textY = (int)(card.y + 60);
    DrawText(TextFormat("Name: %s", currentStudent.name), (int)(card.x + 20), textY, 16, BLACK);
    DrawText(TextFormat("Department: %s", currentStudent.batch), (int)(card.x + 20), textY + 25, 16, BLACK);
    DrawText(TextFormat("Roll No: %s", currentStudent.rollNo), (int)(card.x + 20), textY + 50, 16, BLACK);

    for (int i = 0; i < 4; i++)
    {
        bool hovered = CheckCollisionPointRec(GetMousePosition(), buttons[i]);
        DrawRectangleRounded(buttons[i], 0.3f, 10, hovered ? buttonHover : buttonColor);
        DrawRectangleRoundedLines(buttons[i], 0.3f, 10, 1, Fade(BLUE, 0.3f));

        int textWidth = MeasureText(labels[i], 14);
        DrawText(labels[i], (int)(buttons[i].x + (buttons[i].width - textWidth) / 2),
                 (int)(buttons[i].y + 10), 14, textColor);

        if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            switch (i)
            {
            case 0:
                currentScreen = SCREEN_ROUTE_MAP;
                break;
            case 1:
                currentScreen = SCREEN_SEAT_AVAILABILITY;
                break;
            case 2:
                currentScreen = SCREEN_BOOK_SEAT;
                break;
            case 3:
                currentScreen = SCREEN_TIME_SCHEDULE;
                break;
            }
        }
    }
}

void DrawRouteMap()
{
    DrawText("Route Map", 50, 20, 28, BLACK);

    int cx = 400;
    int cy = 300;

    DrawCircle(cx, cy, 40, DARKBLUE);
    DrawText("FAST", cx - MeasureText("FAST", 20) / 2, cy - 10, 20, WHITE);

    int greenStops[5] = {cx, cx + 50, cx + 100, cx + 150, cx + 200};

    int blueStops[5] = {cx, cx - 50, cx - 100, cx - 150, cx - 200};

    int redStops[5] = {cy, cy - 40, cy - 80, cy - 120, cy - 160};

    int yellowStops[5] = {cy, cy + 40, cy + 80, cy + 120, cy + 160};

    static int greenIndex = 0;
    static int blueIndex = 0;
    static int redIndex = 0;
    static int yellowIndex = 0;

    DrawLine(cx, cy, cx + 250, cy, GREEN);
    DrawText("Green Route ->", cx + 260, cy - 8, 16, GREEN);

    DrawLine(cx, cy, cx - 250, cy, BLUE);
    DrawText("<- Blue Route",
             cx - 250 - MeasureText("← Blue Route", 16) - 10,
             cy - 8, 16, BLUE);

    DrawLine(cx, cy, cx, cy - 200, RED);
    DrawText("Red Route ^", cx - MeasureText("Red Route ↑", 16) / 2,
             cy - 220, 16, RED);

    DrawLine(cx, cy, cx, cy + 200, YELLOW);
    DrawText("Yellow Route",
             cx - MeasureText("Yellow Route ↓", 16) / 2,
             cy + 210, 16, YELLOW);

    for (int i = 1; i <= 4; i++)
        DrawCircle(cx + i * 50, cy, 12, Fade(GREEN, 0.5f));
    for (int i = 1; i <= 4; i++)
        DrawCircle(cx - i * 50, cy, 12, Fade(BLUE, 0.5f));
    for (int i = 1; i <= 4; i++)
        DrawCircle(cx, cy - i * 40, 12, Fade(RED, 0.5f));
    for (int i = 1; i <= 4; i++)
        DrawCircle(cx, cy + i * 40, 12, Fade(YELLOW, 0.5f));

    if (IsKeyPressed(KEY_RIGHT) && greenIndex < 4)
        greenIndex++;
    if (IsKeyPressed(KEY_LEFT) && greenIndex > 0)
        greenIndex--;

    if (IsKeyPressed(KEY_D) && blueIndex < 4)
        blueIndex++;
    if (IsKeyPressed(KEY_A) && blueIndex > 0)
        blueIndex--;

    if (IsKeyPressed(KEY_W) && yellowIndex > 0)
        yellowIndex--;
    if (IsKeyPressed(KEY_S) && yellowIndex < 4)
        yellowIndex++;

    if (IsKeyPressed(KEY_UP) && redIndex < 4)
        redIndex++;
    if (IsKeyPressed(KEY_DOWN) && redIndex > 0)
        redIndex--;

    DrawRectangle(greenStops[greenIndex] - 10, cy - 10, 20, 20, GREEN);

    DrawRectangle(blueStops[blueIndex] - 10, cy - 10, 20, 20, BLUE);

    DrawRectangle(cx - 10, redStops[redIndex] - 10, 20, 20, RED);

    DrawRectangle(cx - 10, yellowStops[yellowIndex] - 10, 20, 20, YELLOW);

    DrawText("Controls to see routes:", 20, 440, 18, BLACK);
    DrawText("Green Bus: LEFT / RIGHT", 20, 460, 16, GREEN);
    DrawText("Blue Bus: D / A", 20, 480, 16, BLUE);
    DrawText("Red Bus: UP / DOWN", 20, 500, 16, RED);
    DrawText("Yellow Bus: W / S", 20, 520, 16, YELLOW);

    DrawText("Press BACKSPACE to return", 20, 550, 20, GRAY);
    if (IsKeyPressed(KEY_BACKSPACE))
        currentScreen = SCREEN_MAIN;
}

// ===================== SEAT AVAILABILITY =====================
void DrawSeatAvailability()
{
    DrawText("Seat Availability", 50, 20, 20, BLACK);

    const char *busDisplay[MAX_BUSES] = {"Green Bus", "Blue Bus", "Red Bus", "Yellow Bus"};
    const char *busInternal[MAX_BUSES] = {"Green", "Blue", "Red", "Yellow"};
    Rectangle busButtons[MAX_BUSES] = {{50, 50, 100, 30}, {160, 50, 100, 30}, {270, 50, 100, 30}, {380, 50, 100, 30}};

    for (int i = 0; i < 4; i++)
    {
        bool hovered = CheckCollisionPointRec(GetMousePosition(), busButtons[i]);
        DrawRectangleRounded(busButtons[i], 0.2f, 10, hovered ? BLUE : WHITE);
        DrawRectangleRoundedLines(busButtons[i], 0.2f, 10, 1, BLUE);
        int tw = MeasureText(busDisplay[i], 14);
        DrawText(busDisplay[i], busButtons[i].x + (busButtons[i].width - tw) / 2, busButtons[i].y + 8, 14, BLACK);

        if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            strcpy(selectedBus, busInternal[i]);
    }

    if (strlen(selectedBus) == 0)
        strcpy(selectedBus, "Green");

    int bIndex = -1;
    for (int i = 0; i < busCount; i++)
        if (strcmp(busList[i].bus, selectedBus) == 0)
            bIndex = i;

    if (bIndex != -1)
    {
        for (int i = 0; i < SEATS_PER_BUS; i++)
        {
            Color c = busSeats[bIndex][i].booked ? RED : GREEN;
            DrawRectangleRec(busSeats[bIndex][i].rect, c);
            DrawText(TextFormat("%d", busSeats[bIndex][i].seatNum),
                     busSeats[bIndex][i].rect.x + 15,
                     busSeats[bIndex][i].rect.y + 15,
                     20,
                     BLACK);
        }
    }

    DrawText("GREEN=available, RED=booked", 50, 500, 16, GRAY);
    DrawText("Press BACKSPACE to return", 50, 520, 16, GRAY);
    if (IsKeyPressed(KEY_BACKSPACE))
        currentScreen = SCREEN_MAIN;
}

// ===================== BOOK SEAT =====================
void DrawBookSeat()
{
    DrawText("Book a Seat", 50, 20, 20, BLACK);
    const char *busDisplay[MAX_BUSES] = {"Green Bus", "Blue Bus", "Red Bus", "Yellow Bus"};
    const char *busInternal[MAX_BUSES] = {"Green", "Blue", "Red", "Yellow"};
    Rectangle busButtons[MAX_BUSES] = {{50, 50, 100, 30}, {160, 50, 100, 30}, {270, 50, 100, 30}, {380, 50, 100, 30}};
    for (int i = 0; i < 4; i++)
    {
        bool hovered = CheckCollisionPointRec(GetMousePosition(), busButtons[i]);
        DrawRectangleRounded(busButtons[i], 0.2f, 10, hovered ? BLUE : WHITE);
        DrawRectangleRoundedLines(busButtons[i], 0.2f, 10, 1, BLUE);
        int tw = MeasureText(busDisplay[i], 14);
        DrawText(busDisplay[i], busButtons[i].x + (busButtons[i].width - tw) / 2, busButtons[i].y + 8, 14, BLACK);
        if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            strcpy(selectedBus, busInternal[i]);
    }
    if (strlen(selectedBus) == 0)
        strcpy(selectedBus, "Green");
    int bIndex = -1;
    for (int i = 0; i < busCount; i++)
        if (strcmp(busList[i].bus, selectedBus) == 0)
            bIndex = i;
    if (bIndex != -1)
    {
        for (int i = 0; i < SEATS_PER_BUS; i++)
        {
            Color c = busSeats[bIndex][i].booked ? RED : GREEN;
            DrawRectangleRec(busSeats[bIndex][i].rect, c);
            DrawText(TextFormat("%d", busSeats[bIndex][i].seatNum), busSeats[bIndex][i].rect.x + 15, busSeats[bIndex][i].rect.y + 15, 20, BLACK);
            if (!busSeats[bIndex][i].booked && CheckCollisionPointRec(GetMousePosition(), busSeats[bIndex][i].rect))
            {
                DrawRectangleLinesEx(busSeats[bIndex][i].rect, 3, YELLOW);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    busSeats[bIndex][i].booked = true;
                    busList[bIndex].seats--;
                    SaveBuses();
                }
            }
        }
    }
    DrawText("Click GREEN seat to book", 50, 500, 16, GRAY);
    DrawText("Press BACKSPACE to return", 50, 520, 16, GRAY);
    if (IsKeyPressed(KEY_BACKSPACE))
        currentScreen = SCREEN_MAIN;
}

// ===================== TIME SCHEDULE =====================
void DrawTimeSchedule()
{
    DrawText("Bus Time Schedules", 50, 20, 24, BLACK);

    const char *busDisplay[MAX_BUSES] = {"Green Bus", "Blue Bus", "Red Bus", "Yellow Bus"};
    const char *busInternal[MAX_BUSES] = {"Green", "Blue", "Red", "Yellow"};
    Rectangle busButtons[MAX_BUSES] = {
        {50, 60, 100, 30},
        {160, 60, 100, 30},
        {270, 60, 100, 30},
        {380, 60, 100, 30}};

    static char selectedBus[20] = "Green";

    for (int i = 0; i < 4; i++)
    {
        bool hovered = CheckCollisionPointRec(GetMousePosition(), busButtons[i]);
        DrawRectangleRounded(busButtons[i], 0.2f, 10, hovered ? BLUE : WHITE);
        DrawRectangleRoundedLines(busButtons[i], 0.2f, 10, 1, BLUE);
        int tw = MeasureText(busDisplay[i], 14);
        DrawText(busDisplay[i], busButtons[i].x + (busButtons[i].width - tw) / 2,
                 busButtons[i].y + 5, 14, BLACK);

        if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            strcpy(selectedBus, busInternal[i]);
    }

    char morning[256] = "";
    char evening[256] = "";
    FILE *file = fopen("schedules.txt", "r");
    if (file)
    {
        char line[256];
        while (fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = 0;
            if (strcmp(line, TextFormat("%s:", selectedBus)) == 0)
            {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                strcpy(morning, strchr(line, ':') + 1);

                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                strcpy(evening, strchr(line, ':') + 1);

                break;
            }
        }
        fclose(file);
    }
    else
    {
        DrawText("Error: Could not open schedules.txt", 50, 120, 20, RED);
        return;
    }

    int stopsX[5] = {100, 250, 400, 550, 700};
    int yMorning = 200;
    int yEvening = 350;

    DrawText("Morning(-->):", 100, yMorning - 40, 20, BLACK);
    DrawLine(100, yMorning, 700, yMorning, DARKGRAY);

    char *token = strtok(morning, ",");
    int index = 0;
    while (token && index < 5)
    {
        DrawCircle(stopsX[index], yMorning, 15, BLUE);
        DrawText(TextFormat("%d", index + 1), stopsX[index] - 6, yMorning - 7, 12, WHITE);
        DrawText(TextFormat("%s", token), stopsX[index] - 40, yMorning + 20, 14, BLACK);

        token = strtok(NULL, ",");
        index++;
    }

    DrawCircle(stopsX[4] + 40, yMorning, 40, BLUE);
    DrawText("FAST->", stopsX[4] + 20, yMorning - 7, 14, WHITE);

    DrawText("Evening(<--):", 100, yEvening - 40, 20, BLACK);
    DrawLine(100, yEvening, 700, yEvening, DARKGRAY);

    token = strtok(evening, ",");
    index = 0;
    while (token && index < 5)
    {
        DrawCircle(stopsX[index], yEvening, 15, RED);
        DrawText(TextFormat("%d", index + 1), stopsX[index] - 6, yEvening - 7, 12, WHITE);
        DrawText(TextFormat("%s", token), stopsX[index] - 40, yEvening + 20, 14, BLACK);

        token = strtok(NULL, ",");
        index++;
    }
    DrawCircle(stopsX[4] + 40, yEvening, 40, RED);
    DrawText("<- FAST", stopsX[4] + 20, yEvening - 7, 14, WHITE);

    DrawText("Press BACKSPACE to return", 50, 520, 16, GRAY);
    if (IsKeyPressed(KEY_BACKSPACE))
        currentScreen = SCREEN_MAIN;
}