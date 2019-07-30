
#include "Player.h"

/**************************************************/
/*                Public Functions                */
/**************************************************/

Player::Player(Table* table, Cards* cardInfo)
{
    this->table = table;
    hand = new Hand(cardInfo);
}

Player::~Player()
{
    delete hand;
}