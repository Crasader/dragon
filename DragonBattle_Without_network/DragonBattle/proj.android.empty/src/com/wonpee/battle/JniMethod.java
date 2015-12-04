package com.wonpee.battle;

public class JniMethod {
	public static native int nativeTest();
	public static native void commandToNative(int command, String data);
	
	
	public static int onNativeCommand(int command, String data)
	{
		return 1;
	}
}
