//
//  Itemunitformula.cpp
//  DragonBattle
//
//  Created by zeng xiaolong on 13-7-22.
//
//

#include "Itemunitformula.h"

int getItemCount(std::vector<one_round_battle_prize_t> *prize_vec,int itemid)
{
    int yxb = 0;
    for (int i = 0; i<prize_vec->size(); i++)
    {
        one_round_battle_prize_t *prize = &prize_vec->at(i);
        for (int k = 0; k<prize->drop_vec.size(); k++)
        {
            item_unit_t *item = &prize->drop_vec.at(k);
            if (item->item_id == itemid)
            {
                yxb += item->item_cnt;
            }
        }
        for (int k = 0; k<prize->item_vec.size(); k++)
        {
            item_unit_t *item = &prize->item_vec.at(k);
            if (item->item_id == itemid)
            {
                yxb += item->item_cnt;
            }
        }
    }
    return yxb;
}

int getItemCount(std::vector<auto_battle_t> *prize_vec,int itemid)
{
    int item_cnt = 0;
    for (int i = 0; i<prize_vec->size(); i++)
    {
        auto_battle_t *prize = &prize_vec->at(i);
        for (int k = 0; k<prize->drop_vec.size(); k++)
        {
            item_unit_t *item = &prize->drop_vec.at(k);
            if (item->item_id == itemid)
            {
                item_cnt += item->item_cnt;
            }
        }
        for (int k = 0; k<prize->item_vec.size(); k++)
        {
            item_unit_t *item = &prize->item_vec.at(k);
            if (item->item_id == itemid)
            {
                item_cnt += item->item_cnt;
            }
        }
    }
    return item_cnt;
}