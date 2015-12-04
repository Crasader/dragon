package com.wonpee.battle;

public class NativeCommand {
	/* native call back cmd */
	public static final int CMD_LOGIN_WITH_QQ = 1;
	public static final int CMD_LOGIN_WITH_WX = 2;
	public static final int CMD_LOGOUT = 3;

	public static final int CMD_SHARE_WITH_QQ = 4;
	public static final int CMD_SHARE_WITH_WX = 5;

	public static final int CMD_UNIPAY_CBK = 10;

	/* notify native code cmd */
	public static final int CMD_UNIPAY_REFRESH = 101;

}
