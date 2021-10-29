#include "PlayerStruct.h"

void Player_TakeDamage(Player* _player, int _damages)
{
	_player->current_hp -= _damages;

	if (_player->current_hp < 0)
	{
		_player->current_hp = 0;
	}
}

void Player_ReceiveHeal(Player* _player, int _heal)
{
	_player->current_hp += _heal;

	if (_player->current_hp > _player->max_hp)
	{
		_player->current_hp = _player->max_hp;
	}
}
