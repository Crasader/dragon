package com.wonpee.battle;


public class JniMethod {
	public static native int nativeTest();

	public static native void commandToNative(int command, String data);

	public static int onNativeCommand(int command, String data) {
		switch (command) {
		case NativeCommand.CMD_LOGOUT: {
			break;
		}
		default: {
			break;
		}
		}
		return 1;
	}

	public static int onCheckNetWork() {
		return GameManager.getInstance().checkNetWork();
	}

	public static void onWriteToClipboard(String data) {
		GameManager.getInstance().writeToClipboard(data);
	}

	public static String onReadFromClipboard() {
		return GameManager.getInstance().readFromClipboard();
	}
}
