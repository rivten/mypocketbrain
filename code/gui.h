
static const int GuiFontSize = 16;
static struct nk_context GuiContext_ = {0};
static struct nk_user_font GuiNKFont_ = {0};
static Font GuiRLFont_ = {0};
static struct nk_colorf NuklearColor = {0};

float GetTextWidth(nk_handle handle, float h, const char* t, int len)
{
	return MeasureTextEx(GuiRLFont_, TextSubtext(t, 0, len), GuiFontSize, 0.0f).x;
}

static struct nk_context* NuklearInit()
{
	GuiNKFont_.height = GuiFontSize;
	GuiNKFont_.width = GetTextWidth;
	GuiNKFont_.userdata = nk_handle_ptr(0);
	nk_init_default(&GuiContext_, &GuiNKFont_);
	GuiRLFont_ = LoadFontEx("../data/LiberationMono-Regular.ttf", GuiFontSize, 0, 95);

	return &GuiContext_;
}

static void NuklearFree(struct nk_context* Context)
{
	UnloadFont(GuiRLFont_);
	nk_free(Context);
}

static enum nk_buttons RaylibButtonToNuklearButton(MouseButton RaylibButton)
{
	switch(RaylibButton)
	{
		case MOUSE_LEFT_BUTTON:
		{
			return(NK_BUTTON_LEFT);
		} break;
		case MOUSE_RIGHT_BUTTON:
		{
			return(NK_BUTTON_RIGHT);
		} break;
		case MOUSE_MIDDLE_BUTTON:
		{
			return(NK_BUTTON_MIDDLE);
		} break;
		default:
		{
			ASSERT(false);
		}
	}
	return(NK_BUTTON_MAX);
}

static void NuklearGatherInput(struct nk_context* Context)
{
	nk_input_begin(Context);
	int MouseX = GetMouseX();
	int MouseY = GetMouseY();
	nk_input_motion(Context, MouseX, MouseY);
	MouseButton Buttons[] = {MOUSE_LEFT_BUTTON, MOUSE_RIGHT_BUTTON, MOUSE_MIDDLE_BUTTON};
	for(u32 ButtonIndex = 0; ButtonIndex < count_of(Buttons); ++ButtonIndex)
	{
		MouseButton Button = Buttons[ButtonIndex];
		nk_input_button(Context, RaylibButtonToNuklearButton(Button), MouseX, MouseY, IsMouseButtonDown(Button));
	}
	nk_input_scroll(Context, cast(struct nk_vec2){0, GetMouseWheelMove()});
	for(int Key = KEY_A; Key <= KEY_Z; ++Key)
	{
		if(IsKeyPressed(Key))
		{
			nk_input_char(Context, Key - KEY_A + 'a');
		}
	}
	nk_input_key(Context, NK_KEY_SHIFT, IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT));
	nk_input_key(Context, NK_KEY_CTRL, IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL));
	nk_input_key(Context, NK_KEY_DEL, IsKeyPressed(KEY_DELETE));
	nk_input_key(Context, NK_KEY_ENTER, IsKeyPressed(KEY_ENTER));
	nk_input_key(Context, NK_KEY_TAB, IsKeyPressed(KEY_TAB));
	nk_input_key(Context, NK_KEY_BACKSPACE, IsKeyPressed(KEY_BACKSPACE));
	nk_input_key(Context, NK_KEY_UP, IsKeyPressed(KEY_UP));
	nk_input_key(Context, NK_KEY_DOWN, IsKeyPressed(KEY_DOWN));
	nk_input_key(Context, NK_KEY_LEFT, IsKeyPressed(KEY_LEFT));
	nk_input_key(Context, NK_KEY_RIGHT, IsKeyPressed(KEY_RIGHT));
	nk_input_end(Context);
}

static Color NuklearColorToRaylibColor(struct nk_color color)
{
    Color rc;
    rc.a = color.a;
    rc.r = color.r;
    rc.g = color.g;
    rc.b = color.b;
    return rc;
}

static void NuklearRender(struct nk_context* Context, u32 ScreenWidth, u32 ScreenHeight)
{
	BeginScissorMode(0, 0, ScreenWidth, ScreenHeight);
	const struct nk_command* Command = 0;
	nk_foreach(Command, Context)
	{
		Color color;
		switch (Command->type) 
		{
			case NK_COMMAND_NOP: 
			{
			}break;

			case NK_COMMAND_SCISSOR:
			{
				EndScissorMode();
				const struct nk_command_scissor *s = (const struct nk_command_scissor *)Command;
				BeginScissorMode(s->x, s->y, s->w, s->h);
			} break;

			case NK_COMMAND_LINE:
			{
				const struct nk_command_line *l = (const struct nk_command_line *)Command;
				color = NuklearColorToRaylibColor(l->color);
				DrawLine(l->begin.x, l->begin.y, l->end.x, l->end.y, color);
			} break;

			case NK_COMMAND_RECT:
			{
				const struct nk_command_rect *r = (const struct nk_command_rect *)Command;
				color = NuklearColorToRaylibColor(r->color);
				DrawRectangleLines(r->x, r->y, r->w, r->h, color);
			} break;

			case NK_COMMAND_RECT_FILLED:
			{
				const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)Command;
				color = NuklearColorToRaylibColor(r->color);
				DrawRectangle(r->x, r->y, r->w, r->h, color);
			} break;

			case NK_COMMAND_CIRCLE:
			{
				const struct nk_command_circle *c = (const struct nk_command_circle *)Command;
				color = NuklearColorToRaylibColor(c->color);
				DrawCircleLines(c->x + 0.5f * c->w, c->y + 0.5f * c->h, 0.5f * c->w, color);
			} break;

			case NK_COMMAND_CIRCLE_FILLED:
			{
				const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)Command;
				color = NuklearColorToRaylibColor(c->color);
				DrawCircle(c->x + 0.5f * c->w, c->y + 0.5f * c->h, 0.5f * c->w, color);
			} break;

			case NK_COMMAND_TRIANGLE:
			{
				const struct nk_command_triangle *t = (const struct nk_command_triangle*)Command;
				color = NuklearColorToRaylibColor(t->color);
				DrawTriangleLines((Vector2){t->a.x, t->a.y}, (Vector2){t->c.x, t->c.y}, (Vector2){t->b.x, t->b.y}, color);
			} break;

			case NK_COMMAND_TRIANGLE_FILLED:
			{
				const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled*)Command;
				color = NuklearColorToRaylibColor(t->color);
				DrawTriangle((Vector2){t->a.x, t->a.y}, (Vector2){t->c.x, t->c.y}, (Vector2){t->b.x, t->b.y}, color);
			} break;

			case NK_COMMAND_POLYGON:
			{
				printf("NK_COMMAND_POLYGON\n");
#if 0
				const struct nk_command_polygon *p = (const struct nk_command_polygon*)Command;
				color = NuklearColorToRaylibColor(p->color);
				Vector2 points[p->point_count];

				for (int i = 0; i < p->point_count; i++) {
					points[i] = (Vector2){p->points[i].x, p->points[i].y};
				}

				DrawPolyEx(points, p->point_count, color);
#endif
			} break;

			case NK_COMMAND_POLYGON_FILLED:
			{
				printf("NK_COMMAND_POLYGON_FILLED\n");
#if 0
				const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled*)Command;
				color = NuklearColorToRaylibColor(p->color);
				Vector2 points[p->point_count];

				for (int i = 0; i < p->point_count; i++) {
					points[i] = (Vector2){p->points[i].x, p->points[i].y};
				}

				DrawPolyEx(points, p->point_count, color);
#endif
			} break;

			case NK_COMMAND_TEXT:
			{
				const struct nk_command_text *t = (const struct nk_command_text*)Command;
				color = NuklearColorToRaylibColor(t->foreground);
				DrawTextEx(GuiRLFont_, (const char*)t->string, cast(Vector2) {t->x, t->y}, GuiFontSize, 0.0f, color);
#if 0
				// NOTE(hugo): Debugging text rects
				Vector2 TextRect = MeasureTextEx(GuiRLFont_, (const char*)t->string, GuiFontSize, 0.0f);
				DrawRectangleLines(t->x, t->y, TextRect.x, TextRect.y, RED);
#endif
			} break;
			case NK_COMMAND_CURVE: 
			{
				printf("NK_COMMAND_CURVE\n");
			} break;
			case NK_COMMAND_RECT_MULTI_COLOR: 
			{
				const struct nk_command_rect_multi_color* r = (const struct nk_command_rect_multi_color *)Command;
				// NOTE(hugo): It just works ! Don't touch it :)
				Color col1 = NuklearColorToRaylibColor(r->left);
				Color col2 = NuklearColorToRaylibColor(r->bottom);
				Color col3 = NuklearColorToRaylibColor(r->right);
				Color col4 = NuklearColorToRaylibColor(r->top);
				DrawRectangleGradientEx(cast(Rectangle) {r->x, r->y, r->w, r->h}, col1, col2, col3, col4);
			} break;
			case NK_COMMAND_ARC: 
			{
				printf("NK_COMMAND_ARC\n");
			} break;
			case NK_COMMAND_ARC_FILLED: 
			{
				printf("NK_COMMAND_ARC_FILLED\n");
			} break;
			case NK_COMMAND_POLYLINE: 
			{
				printf("NK_COMMAND_POLYLINE\n");
			} break;
			case NK_COMMAND_IMAGE: 
			{
				printf("NK_COMMAND_IMAGE\n");
			} break;
			case NK_COMMAND_CUSTOM: 
			{
				printf("NK_COMMAND_CUSTOM\n");
			} break;
			default:
			{
				ASSERT(false);
			} break;
		}
	}
	nk_clear(Context);
	EndScissorMode();
}


