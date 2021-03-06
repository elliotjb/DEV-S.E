#include "UI_Button.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Audio.h"

//Constructors
UI_Button::UI_Button(const SDL_Rect & box, const Ui_img& tex_on, const Ui_img& tex_off, const Ui_img& tex_over) :Ui_Element(box, BUTTON), tex_on(tex_on), tex_off(tex_off), tex_over(tex_over), button_state(OFF), interactive(true) {}

UI_Button::UI_Button(const UI_Button* copy) : Ui_Element(copy->box, copy->ui_type), tex_on(copy->tex_on), tex_off(copy->tex_off), tex_over(copy->tex_over), button_state(OFF), interactive(true) {}

UI_Button::UI_Button() : Ui_Element({ 0,0,0,0 }, BUTTON), button_state(OFF), interactive(true) {}

//Destructor
UI_Button::~UI_Button()
{

}


// ==========================
void UI_Button::Draw(bool debug)const
{

	//This Draw
	int x = this->box.x;
	int y = this->box.y;

	if(debug)App->render->DrawQuad({ x, y, box.w, box.h }, 150, 250, 0);

	//This Draw
	switch (button_state) {

		case BUTTON_STATE::ON:		tex_on.DrawAt(x,y);			break;
		case BUTTON_STATE::OFF:		tex_off.DrawAt(x, y);		break;
		case BUTTON_STATE::OVER:	tex_over.DrawAt(x, y);		break;

	}

	//Childs Draw
	DrawChilds(debug);
}

bool UI_Button::Update()
{
	if (interactive)
	{
		HandleInput();
	}
	UpdateChilds();
	return true;
}

void UI_Button::SetInteractive(bool inter_)
{
	interactive = inter_;
}

void UI_Button::Change_State(BUTTON_STATE new_button_state)
{
	button_state = new_button_state;
}

void UI_Button::HandleInput()
{

	j1KeyState mouse_key_1 = App->input->GetMouseButtonDown(1);
	
	if (mouse_key_1 == KEY_IDLE && MouseIsIn() && App->gui->upper_element == this->layer ||
		mouse_key_1 == KEY_UP && MouseIsIn() && App->gui->upper_element == this->layer)
	{
		button_state = OVER;
	}
	else if (mouse_key_1 == KEY_DOWN && App->gui->upper_element == this->layer && MouseIsIn())
	{
		this->button_state = ON;
		App->audio->PlayFx(4);
		App->gui->ItemSelected = this;
	}
	else if (App->gui->ItemSelected == this && mouse_key_1 == KEY_REPEAT  && MouseIsIn())
	{
		App->audio->PlayFx(4);
		button_state = ON;
	}
	else
	{
		button_state = OFF;
	}

}

void UI_Button::SetTextures_button(BUTTON_STATE state_texture, Ui_img texture)
{
	if (state_texture == ON)
	{
		this->tex_on = texture;
	}
	else if (state_texture == OFF)
	{
		this->tex_off = texture;
	}
	else if (state_texture == OVER)
	{
		this->tex_over = texture;
	}
}
