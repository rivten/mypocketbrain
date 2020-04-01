#include "raylib.h"

#include "nuklear_compile_flags.h"
#include "nuklear.h"

#define COMMON_IMPLEMENTATION
#include "common.h"

#define MAX_WORD_LEN 64

typedef struct word
{
	char Text[MAX_WORD_LEN];
	u32 PosX;
	u32 PosY;
} word;

#include "gui.h"

int main(void)
{
    const int ScreenWidth = 800;
    const int ScreenHeight = 450;

    InitWindow(ScreenWidth, ScreenHeight, "mypocketbrain");
	SetConfigFlags(FLAG_WINDOW_RESIZABLE|FLAG_VSYNC_HINT);

    SetTargetFPS(60);

	u32 WordCount = 0;
	word Words[16] = {0};

	u32 FontSize = GuiFontSize;
	const char* IntroText = "Enter word : ";

	u32 EnterTextBufferCount = 0;
	char EnterTextBuffer[MAX_WORD_LEN] = {0};
	struct nk_context* GuiContext = NuklearInit();

    while (!WindowShouldClose())
    {
		NuklearGatherInput(GuiContext);
		if(IsKeyPressed(KEY_ENTER))
		{
			if(EnterTextBufferCount > 0)
			{
				ASSERT(WordCount < count_of(Words));
				word* W = Words + WordCount;
				W->PosX = 10;
				W->PosY = FontSize * (WordCount + 2);
				for(u32 CharIndex = 0; CharIndex < MAX_WORD_LEN; ++CharIndex)
				{
					W->Text[CharIndex] = EnterTextBuffer[CharIndex];
					EnterTextBuffer[CharIndex] = 0;
				}
				++WordCount;
				EnterTextBufferCount = 0;
			}
		}

		for(int Key = KEY_A; Key <= KEY_Z; ++Key)
		{
			if(IsKeyPressed(Key))
			{
				ASSERT(EnterTextBufferCount < count_of(EnterTextBuffer)); 
				EnterTextBuffer[EnterTextBufferCount] = Key - KEY_A + 'a';
				++EnterTextBufferCount;
			}
		}
		if(IsKeyPressed(KEY_BACKSPACE))
		{
			if(EnterTextBufferCount > 0)
			{
				--EnterTextBufferCount;
			}
			ASSERT(EnterTextBufferCount < count_of(EnterTextBuffer));
			EnterTextBuffer[EnterTextBufferCount] = 0;
		}
		if(IsKeyPressed(KEY_SPACE))
		{
			ASSERT(EnterTextBufferCount < count_of(EnterTextBuffer));
			EnterTextBuffer[EnterTextBufferCount] = ' ';
			++EnterTextBufferCount;
		}

		NuklearDisplay(GuiContext);

		{
			BeginDrawing();

			ClearBackground(RAYWHITE);
			int IntroTextSize = MeasureTextEx(GuiRLFont_, IntroText, FontSize, 1.0f).x;
			DrawTextEx(GuiRLFont_, IntroText, cast(Vector2) {10, 10}, FontSize, 1.0f, BLACK);
			DrawTextEx(GuiRLFont_, EnterTextBuffer, cast(Vector2) {10 + IntroTextSize, 10}, FontSize, 1.0f, BLACK);
			for(u32 WordIndex = 0; WordIndex < WordCount; ++WordIndex)
			{
				word* Word = Words + WordIndex;
				DrawTextEx(GuiRLFont_, Word->Text, cast(Vector2) {Word->PosX, Word->PosY}, FontSize, 1.0f, BLACK);
			}

			NuklearRender(GuiContext, ScreenWidth, ScreenHeight);

			EndDrawing();
		}
    }
	NuklearFree(GuiContext);

    CloseWindow();

    return 0;
}
