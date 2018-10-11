#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

class Collider;

struct Player
{
	iPoint position = { 0,0 };
	iPoint speed = { 0,0 };
	int playerSpeed = 0;
	int jumpStrength = 0;
	int gravity = 0;
	bool	alive	=	false;
	bool	jumping =	false;

	bool	facingLeft	=	false;
	
	Collider*		playerCollider		= nullptr;
	Collider*		playerNextFrameCol	= nullptr;

	iPoint colliderOffset = { 0,0 };

	SDL_Texture*	playerTexture		= nullptr;

	Animation* currentAnimation = nullptr;

	Animation idle;
	Animation run;
	Animation jump;

public:
};

class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node &config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	//bool PostUpdate(float dt); //needead to update the player position that was changed in update(float dt);

	// Called before quitting
	bool CleanUp();

	void LoadAnimations();
	void OnCollision(Collider* collider1, Collider* collider2);
	void CheckCollision();

private:

	void HorizontalInput();
	void VerticalInput();
	void UpdateColliders();
	void ApplyGravity();

public:

	Player* player1 = nullptr;

private:

	pugi::xml_node		animFinder;
	pugi::xml_document	storedAnims;
	
};

#endif // __j1PLAYER_H__