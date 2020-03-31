#include "raylib.h"

#include "gb.h"

#include "nuklear_compile_flags.h"
#include "nuklear.h"

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
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "mypocketbrain");

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
				GB_ASSERT(WordCount < gb_count_of(Words));
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
				GB_ASSERT(EnterTextBufferCount < gb_count_of(EnterTextBuffer)); 
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
			GB_ASSERT(EnterTextBufferCount < gb_count_of(EnterTextBuffer));
			EnterTextBuffer[EnterTextBufferCount] = 0;
		}
		if(IsKeyPressed(KEY_SPACE))
		{
			GB_ASSERT(EnterTextBufferCount < gb_count_of(EnterTextBuffer));
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

			NuklearRender(GuiContext);

			EndDrawing();
		}
    }
	NuklearFree(GuiContext);

    CloseWindow();

    return 0;
}
