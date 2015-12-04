//
//  Itemunitformula.h
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-22.
//
//

#ifndef __DragonBattle__Itemunitformula__
#define __DragonBattle__Itemunitformula__

#ifdef WONPEE_SERVER
#include "../player.hpp"
#else
#include "online_cli_proto_cmd.h"
#endif

int getItemCount(std::vector<one_round_battle_prize_t> *prize_vec,int itemid);
int getItemCount(std::vector<auto_battle_t> *prize_vec,int itemid);

#endif /* defined(__DragonBattle__Itemunitformula__) */
