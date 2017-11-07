#include "EnemyNormal.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "Player.h"
#include "ManagerCriatures.h"
#include "j1Map.h"
#include "j1Astar.h"

EnemyNormal::EnemyNormal() : Criature()
{
	name.create("enemynormal");

	idle.PushBack({ 12,  16, 28, 30 });
	idle.PushBack({ 73,  18, 29, 29 });
	idle.PushBack({ 134, 15, 31, 28 });
	idle.PushBack({ 192, 16, 29, 29 });
	idle.PushBack({ 254, 16, 28, 30 });
	idle.PushBack({ 314, 16, 28, 30 });
	idle.speed = AnimationSpeed4;

	//Left---------------------------------------

	run_left.PushBack({ 16, 72, 26, 30});
	run_left.PushBack({ 71, 74, 29, 27});
	run_left.PushBack({ 133,74, 31, 25});
	run_left.PushBack({ 133,74, 27, 28});
	run_left.PushBack({ 195,71, 27, 29});
	run_left.PushBack({ 254,74, 25, 30});
	run_left.PushBack({ 314,71, 27, 29});
	run_left.PushBack({ 375,70, 25, 30});
	run_left.speed = AnimationSpeed4;

	run_right.PushBack({ 16, 194, 26, 30 });
	run_right.PushBack({ 78, 196, 29, 27 });
	run_right.PushBack({ 134,196, 31, 25 });
	run_right.PushBack({ 199,193, 27, 28 });
	run_right.PushBack({ 199,194, 27, 29 });
	run_right.PushBack({ 257,196, 25, 30 });
	run_right.PushBack({ 317,193, 27, 29 });
	run_right.PushBack({ 378,192, 25, 30 });
	run_right.speed = AnimationSpeed4;

	//Death--------------------------------------
	dead.PushBack({ 13, 131, 22, 30});
	dead.PushBack({ 73, 133, 28, 28});
	dead.PushBack({ 135,140, 29, 22});
	dead.PushBack({ 196,141, 28, 20});
	dead.speed = AnimationSpeedDead;
}



EnemyNormal::~EnemyNormal()
{
}

bool EnemyNormal::Awake()
{
	position.create(360, 354);
	velocity.create(0, 0);
	return true;
}

bool EnemyNormal::Start()
{
	graphics = App->tex->Load("textures/Snow_Man.png");
	current_animation = &idle;
	state = IDLE;
	astar = new j1Astar();
	path = astar->GenerateAstar(position, App->managerC->player->Getposition());
	collision_feet = App->collision->AddCollider({ (int)position.x, (int)position.y, 50, 50 }, COLLIDER_ENEMY_NORMAL, this);
	return true;
}

bool EnemyNormal::PreUpdate()
{
	return true;
}

bool EnemyNormal::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		state = RUN_LEFT;
	}
	else
	{
		state = IDLE;
	}


	MoveEnemy(dt);
	processPos();
	processGravity(dt);

	//Collision follow 
	collision_feet->SetPos(position.x, position.y - 25);
	return true;
}

void EnemyNormal::MoveEnemy(float dt)
{
	if (App->managerC->player->Getposition().DistanceTo(position) < 200)
	{
		//if()
	}
	else
	{
		LOG("NO!");
	}
}

void EnemyNormal::processPos()
{
	position.y = position.y + velocity.y;   // current velocity components.
}

void EnemyNormal::processGravity(float dt)
{
	if (velocity.y < 0)
	{
		if (App->map->MovementCost(position.x, position.y - OFFSET_Y, current_animation->frames[0].w, current_animation->frames[0].h, UP) == false && isFly)
		{
			velocity.y = 0;
			isInPlataform = true;
		}
	}
	if (App->map->MovementCost(position.x, position.y, current_animation->frames[0].w, current_animation->frames[0].h, DOWN) && state != FLY_LEFT && state != FLY_RIGHT)
	{
		velocity.y += ceil(Gravity * dt);
	}
	else if (isFly == false)
	{
		if (App->map->MovementCost(position.x, position.y + OFFSET_Y, current_animation->frames[0].w, current_animation->frames[0].h, DOWN) == false)
		{
			isInPlataform = true;
		}
		velocity.y = 0;
	}
	else
	{
		if (App->map->MovementCost(position.x, position.y, current_animation->frames[0].w, current_animation->frames[0].h, DOWN) == false)
		{
			isFly = false;
			velocity.y = 0;
			state = IDLE;
		}
		else
		{
			velocity.y = ceil(Gravity_LOW * dt);
		}
	}
}

void EnemyNormal::Draw()
{
	switch (state)
	{
	case IDLE:
	{
		current_animation = &idle;
		break;
	}
	case RUN_LEFT:
	{
		current_animation = &run_left;
		break;
	}
	case RUN_RIGHT:
	{
		current_animation = &run_right;
		break;
	}
	case DEAD:
	{
		current_animation = &dead;
		break;
	}
	}
	SDL_Rect r = current_animation->GetCurrentFrame();
	App->render->Blit(graphics, position.x / 2, position.y / 2 - 10, &r, 2);
}

bool EnemyNormal::PostUpdate()
{
	Draw();
	return true;
}

bool EnemyNormal::Load(pugi::xml_node &)
{
	return true;
}

bool EnemyNormal::Save(pugi::xml_node &) const
{
	return true;
}

int EnemyNormal::GetDirection() const
{
	return 0;
}

fPoint* EnemyNormal::Getposition() const
{
	return nullptr;
}

bool EnemyNormal::CleanUp()
{
	return true;
}
