/*
 * =====================================================================================
 *       Filename:  share_enums.h
 *    Description:  前后端以及后端不同server之间共用的常量
 *        Version:  1.0
 *        Created:  2013年12月20日 12时09分23秒
 *       Revision:  none
 *       Compiler:  gcc
 * =====================================================================================
 */

#ifndef  SHARE_ENUMS_H__
#define  SHARE_ENUMS_H__

// online发送过来的用户的行为
enum enum_user_action_t {
	e_os_user_online    = 1,    //上线(登录进入游戏)
	e_os_user_offline   = 2,    //下线(退出游戏)
	e_os_user_enter_map = 3,    //切换地图(进入地图)
	e_os_user_exit_map = 4, 	//切换地图(离开地图)
	e_os_user_move      = 5,    //在场景中移动
	e_os_user_in_map    = 10,   //地图中所有用户的信息
};

#endif  /*SHARE_ENUMS_H__*/
