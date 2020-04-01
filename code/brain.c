#include "raylib.h"

#include "nuklear_compile_flags.h"
#include "nuklear.h"

#define COMMON_IMPLEMENTATION
#include "common.h"

#define MAX_WORD_LEN 64

typedef struct word
{
	char Text[MAX_WORD_LEN];
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

	i32 EnterTextBufferCount = 0;
	char EnterTextBuffer[MAX_WORD_LEN] = {0};
	struct nk_context* GuiContext = NuklearInit();

    while (!WindowShouldClose())
    {
		NuklearGatherInput(GuiContext);

		if(nk_begin(GuiContext, "mypocketbrain", nk_rect(0, 0, ScreenWidth, ScreenHeight), 0))
		{
			nk_layout_row_dynamic(GuiContext, 30, 1);
			if(nk_button_label(GuiContext, "Add Word"))
			{
				if(EnterTextBufferCount > 0)
				{
					ASSERT(WordCount < count_of(Words));
					word* W = Words + WordCount;
					for(u32 CharIndex = 0; CharIndex < MAX_WORD_LEN; ++CharIndex)
					{
						W->Text[CharIndex] = EnterTextBuffer[CharIndex];
						EnterTextBuffer[CharIndex] = 0;
					}
					++WordCount;
					EnterTextBufferCount = 0;
				}
			}
			nk_label(GuiContext, "New word :", NK_TEXT_LEFT);
			nk_edit_string(GuiContext, NK_EDIT_FIELD, &EnterTextBuffer[0], &EnterTextBufferCount, count_of(EnterTextBuffer), nk_filter_ascii);

			for(u32 WordIndex = 0; WordIndex < WordCount; ++WordIndex)
			{
				word* Word = Words + WordIndex;
				nk_label(GuiContext, Word->Text, NK_TEXT_LEFT);
			}

		}
		nk_end(GuiContext);

		{
			BeginDrawing();

			NuklearRender(GuiContext, ScreenWidth, ScreenHeight);

			EndDrawing();
		}
    }
	NuklearFree(GuiContext);

    CloseWindow();

    return 0;
}
