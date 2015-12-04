package com.wonpee.battle.log;

import com.tencent.mm.BuildConfig;

import android.util.Log;

public class XYLog {
	
	private static final String TAG = "DragonBattle";

	public static void v(String tag, String msg) {
		if (BuildConfig.DEBUG) {
			Log.v(tag, msg);
		}
	}
	
	public static void d(String tag, String msg) {
		if (BuildConfig.DEBUG) {
			Log.d(tag, msg);
		}
	}
	
	public static void w(String tag, String msg) {
		if (BuildConfig.DEBUG) {
			Log.w(tag, msg);
		}
	}
	
	public static void e(String tag, String msg) {
		if (BuildConfig.DEBUG) {
			Log.e(tag, msg);
		}
	}
	
	public static void d(String msg) {
		if (BuildConfig.DEBUG) {
			Log.d(TAG, msg);
		}
	}
	
	public static void w(String msg) {
		if (BuildConfig.DEBUG) {
			Log.w(TAG, msg);
		}
	}
	
	public static void e(String msg) {
		if (BuildConfig.DEBUG) {
			Log.e(TAG, msg);
		}
	}
	
}
