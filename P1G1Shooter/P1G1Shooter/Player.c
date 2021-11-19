#include "Player.h"
#include "Engine/Game.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Projectile.h"
#include "PowerupAimAssist.h"

void InitPlayer(Player** _player, GameScreenData* _gameScreen)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	memset(newPlayer, 0, sizeof(Player));

	*_player = newPlayer;

	ParamSection* playerSection = GetSection(_gameScreen->mParamsList, PLAYER_INIT_SECTION);

	if (playerSection)
	{
		ParamInt* playerHealth = (ParamInt*)GetParamInSection(playerSection, "Health");
		ParamInt* playerSpeed = (ParamInt*)GetParamInSection(playerSection, "Speed");

		Entity_Initialize((Entity*)newPlayer, TYPE_PLAYER, 5,
			WINDOW_HEIGHT / 2, (float)playerHealth->mValue, playerSpeed->mValue,
			&_gameScreen->mSprites[TYPE_PLAYER],
			Player_Update, Player_OnCollide, Player_Destroy);

		ParamFloat* playerMaxEnergy = (ParamFloat*)GetParamInSection(playerSection, "Energy_max");
		ParamFloat* playerReloadCooldown = (ParamFloat*)GetParamInSection(playerSection, "ReloadCooldown_max");
		ParamFloat* playerOverheatCooldown = (ParamFloat*)GetParamInSection(playerSection, "OverheatCooldown_max");
		ParamFloat* playerReloadGain = (ParamFloat*)GetParamInSection(playerSection, "ReloadGain");

		newPlayer->mChargeZone = malloc(sizeof(DisplayZone));
		InitDisplayZone(newPlayer->mChargeZone, 0, 0, 5, 2, 1);
		DrawBatteryInDisplayZone(newPlayer);

		newPlayer->mHealthZone = malloc(sizeof(DisplayZone));
		InitDisplayZone(newPlayer->mHealthZone, 0, 0, 4, 1, 1);
		DrawHealthInDisplayZone(newPlayer);

		newPlayer->mTouchedTime = 0;
		newPlayer->mEntity.mPosition_x = 5;
		newPlayer->mEntity.mPosition_y = WINDOW_HEIGHT / 2 - 5;
		newPlayer->mEntity.mEntityType = TYPE_PLAYER;
		newPlayer->mCurrentEnergy = playerMaxEnergy->mValue;
		newPlayer->mMaxEnergy = playerMaxEnergy->mValue;
		newPlayer->mCurrentReloadCooldown = 0.f;
		newPlayer->mMaxReloadCooldown = playerReloadCooldown->mValue;
		newPlayer->mReloadGain = playerReloadGain->mValue;
		newPlayer->mOverheatCooldown = 0.f;
		newPlayer->mMaxOverheatCooldown = playerOverheatCooldown->mValue;
		newPlayer->mShootAimAssistTimer = 0.f;
	}
}

void Player_Update(void* _player, Game* _game, GameScreenData* _gameScreen)
{
	Player* myPlayer = (Player*)_player;

	if (myPlayer->mTouchedTime > 0)
	{
		myPlayer->mTouchedTime -= _game->mGameDt;
	}

	if (myPlayer->mShootAimAssistTimer > 0)
	{
		myPlayer->mShootAimAssistTimer -= _game->mGameDt;
	}

	Player_UpdateMovement(myPlayer, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myPlayer->mEntity.mDisplayZone);

	if (KeyPressStart(*_game->mInputs, VK_SPACE))
	{
		Player_Shoot(myPlayer, _gameScreen, _game);
	}

	DrawBatteryInDisplayZone(myPlayer);
	DrawHealthInDisplayZone(myPlayer);
	FlushDisplayZone(_game->mDisplaySettings, myPlayer->mChargeZone);
	FlushDisplayZone(_game->mDisplaySettings, myPlayer->mHealthZone);
}

void Player_UpdateMovement(Player* _player, Game* _game)
{
	double 
		newpos_x = _player->mEntity.mPosition_x, 
		newpos_y = _player->mEntity.mPosition_y;

	if (IsKeyDown(*_game->mInputs, VK_UP) || IsKeyDown(*_game->mInputs, 'Z'))
	{
		newpos_y -= _game->mGameDt * _player->mEntity.mSpeed;
	}
	if (IsKeyDown(*_game->mInputs, VK_DOWN) || IsKeyDown(*_game->mInputs, 'S'))
	{
		newpos_y += _game->mGameDt * _player->mEntity.mSpeed;
	}
	
	ClampPlayerPos(_player, &newpos_x, &newpos_y);

	Entity_MoveTo(&_player->mEntity, newpos_x, newpos_y);
	UpdateBatteryDisplayZonePosition(_player);
	UpdateHealthDisplayZonePosition(_player);
}

void Player_OnCollide(Player* _current, Entity* _entity, Game* _game)
{
	if (_current->mTouchedTime <= 0)
	{
		switch (_entity->mEntityType)
		{
		case TYPE_OBSTACLE:
		case TYPE_ENEMY_PROJECTILE:
		case TYPE_ENEMY_SHOOTER:
		case TYPE_ENEMY_KAMIKAZE:
		case TYPE_ENEMY_BOSS:
			Player_TakeDamages(_current, _game);
			return;
		case TYPE_POWERUP_HEALTH:
			Entity_ReceiveHeal(_current, 1);
			Play_Sound("powerup_health", _game->mSoundManager);
			return;
		case TYPE_POWERUP_AIMASSIST:
			_current->mShootAimAssistTimer = POWERUP_AIMASSIST_DELAY;
			Play_Sound("powerup_health", _game->mSoundManager);
			return;
		default:
			return;
		}
	}
}

void ClampPlayerPos(Player* _player, double* _posX, double* _posY)
{
	if (*_posX < 0)
	{
		*_posX = 0;
	}
	if (*_posY < 0)
	{
		*_posY = 0;
	}

	if (*_posX > WINDOW_WIDTH - _player->mEntity.mDisplayZone.mSizeX)
	{
		*_posX = WINDOW_WIDTH - _player->mEntity.mDisplayZone.mSizeX;
	}
	if (*_posY > WINDOW_HEIGHT - _player->mEntity.mDisplayZone.mSizeY)
	{
		*_posY = WINDOW_HEIGHT - _player->mEntity.mDisplayZone.mSizeY;
	}
}

void Player_Shoot(Player* _player, GameScreenData* _data, Game* _game)
{
	if (_player->mOverheatCooldown <= 0)
	{
		
		if (_player->mShootAimAssistTimer > 0)
		{
			Player_Shoot_AimAssist(_player, _data, _game);
		}
		else
		{
			Player_Shoot_Standard(_player, _data, _game);
		}

		_player->mCurrentEnergy -= SHOOT_COST;
		_player->mCurrentReloadCooldown = _player->mMaxReloadCooldown;

		if (_player->mCurrentEnergy <= 0)
		{
			_player->mOverheatCooldown = _player->mMaxOverheatCooldown;
			_player->mCurrentEnergy = 0;
			Play_Sound("player_recharge", _game->mSoundManager);
		}
		else
		{
			Play_Sound("player_shoot.wav", _game->mSoundManager);
		}
	}
}

void Player_Shoot_Standard(Player* _player, GameScreenData* _gameScreen, Game* _gameStruct)
{
	Projectile* newProjectile;
	Proj_Initialize(&newProjectile, 40, 1, 1, 0,
		_player->mEntity.mPosition_x + 7,
		_player->mEntity.mPosition_y,
		TYPE_PLAYER_PROJECTILE,
		TYPE_PLAYER_PROJECTILE, _gameScreen,
		Projectile_Movement_Standard,
		Projectile_Update, PlayerProjectile_OnCollide, Projectile_Destroy);

	DVectorPushBack(_gameScreen->mAllEntities, &newProjectile);
}

void Player_Shoot_AimAssist(Player* _player, GameScreenData* _gameScreen, Game* _gameStruct)
{
	Projectile* newProjectile;

	Proj_Initialize(&newProjectile, 40, 1, 1, 0,
		_player->mEntity.mPosition_x + 7,
		_player->mEntity.mPosition_y,
		TYPE_PLAYER_PROJECTILE,
		TYPE_PLAYER_PROJECTILE + 1, _gameScreen,
		Projectile_Movement_AimAssist,
		Projectile_Update, PlayerProjectile_OnCollide, Projectile_Destroy);

	DVectorPushBack(_gameScreen->mAllEntities, &newProjectile);
}

void UpdateBatteryDisplayZonePosition(Player* _player)
{
	MoveDisplayZone(_player->mChargeZone, 0, (int)(_player->mEntity.mPosition_y) + 1);
}

void UpdateHealthDisplayZonePosition(Player* _player)
{
	MoveDisplayZone(_player->mHealthZone, 
		(int)_player->mEntity.mPosition_x + 5, 
		(int)_player->mEntity.mPosition_y + 3);
}

const ConsoleColors ChargeColors[5] =
{
	BRIGHT_BLACK,
	RED,
	YELLOW,
	GREEN,
	BRIGHT_GREEN
};

void DrawBatteryInDisplayZone(Player* _player)
{
	DisplayCharacter* buffer = _player->mChargeZone->mBuffer;

	ConsoleColors edgeColor =  
		(_player->mOverheatCooldown > 0) ? 
		RED : 
		(DARKER | WHITE);

	int colorLevel = 4;
	
	colorLevel -=
		(_player->mCurrentEnergy < _player->mMaxEnergy) +
		(_player->mCurrentEnergy < (_player->mMaxEnergy / 3) * 2) +
		(_player->mCurrentEnergy < _player->mMaxEnergy / 3) +
		(_player->mCurrentEnergy <= 0);

	ConsoleColors 
		rightFill	= colorLevel >= 3 ? ChargeColors[colorLevel] : ChargeColors[0],
		middleFill	= colorLevel >= 2 ? ChargeColors[colorLevel] : ChargeColors[0],
		leftFill	= ChargeColors[colorLevel];

	buffer[0] = ENCODE_DISPLAY_CHARACTER(edgeColor, BLUE, 222, NO_FLAG);
	buffer[1] = ENCODE_DISPLAY_CHARACTER(edgeColor, leftFill, 223, NO_FLAG);
	buffer[2] = ENCODE_DISPLAY_CHARACTER(edgeColor, middleFill, 223, NO_FLAG);
	buffer[3] = ENCODE_DISPLAY_CHARACTER(edgeColor, rightFill, 223, NO_FLAG);
	buffer[4] = ENCODE_DISPLAY_CHARACTER(edgeColor, BLUE, 221, NO_FLAG);

	buffer[5] = ENCODE_DISPLAY_CHARACTER(edgeColor, BLUE, 222, NO_FLAG);
	buffer[6] = ENCODE_DISPLAY_CHARACTER(edgeColor, leftFill, 220, NO_FLAG);
	buffer[7] = ENCODE_DISPLAY_CHARACTER(edgeColor, middleFill, 220, NO_FLAG);
	buffer[8] = ENCODE_DISPLAY_CHARACTER(edgeColor, rightFill, 220, NO_FLAG);
	buffer[9] = ENCODE_DISPLAY_CHARACTER(edgeColor, BLUE, 221, NO_FLAG);
}

void DrawHealthInDisplayZone(Player* _player)
{
	DisplayCharacter* buffer = _player->mHealthZone->mBuffer;

	buffer[0] = ENCODE_DISPLAY_CHARACTER(RED, BACKGROUND, 3, NO_FLAG);
	if (_player->mEntity.mCurrentHealth > 1)
	{
		buffer[1] = ENCODE_DISPLAY_CHARACTER(RED, BACKGROUND, 3, NO_FLAG);

		if (_player->mEntity.mCurrentHealth > 2)
		{
			buffer[2] = ENCODE_DISPLAY_CHARACTER(RED, BACKGROUND, 3, NO_FLAG);
		}
		else
		{
			buffer[2] = ENCODE_DISPLAY_CHARACTER(FOREGROUND, BACKGROUND, 0, NO_CHARACTER);
		}
	}
	else
	{
		buffer[1] = ENCODE_DISPLAY_CHARACTER(FOREGROUND, BACKGROUND, 0, NO_CHARACTER);
		buffer[2] = ENCODE_DISPLAY_CHARACTER(FOREGROUND, BACKGROUND, 0, NO_CHARACTER);
	}
}

void Player_TakeDamages(Player* _player, Game* _game)
{
	Entity_TakeDamages(&_player->mEntity, 1);
	_player->mTouchedTime = HIT_TIME;
	if (_player->mEntity.mCurrentHealth > 0)
	{
		Play_Sound("player_enemyhit", _game->mSoundManager);
	}
	else
	{
		Play_Sound("player_die", _game->mSoundManager);
	}
}

void Player_Destroy(Player* _player)
{
	CloseDisplayZone(_player->mChargeZone);
	free(_player->mChargeZone);

	CloseDisplayZone(_player->mHealthZone);
	free(_player->mHealthZone);

	free(_player);
}

void PlayerProjectile_OnCollide(Projectile* _current, Entity* _entity, Game* game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_OBSTACLE:
	case TYPE_ENEMY_PROJECTILE:
	case TYPE_ENEMY_SHOOTER:
	case TYPE_ENEMY_KAMIKAZE:
	case TYPE_ENEMY_BOSS:
		Entity_Kill(_current);
		return;
	default:
		return;
	}
}
