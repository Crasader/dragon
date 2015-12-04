package com.wonpee.battle;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.net.ConnectivityManager;

import com.wonpee.battle.log.XYLog;

public class GameManager {
	private static GameManager mInstance = null;

	public Context mContext = null;

	public static GameManager getInstance() {
		if (mInstance == null) {
			mInstance = new GameManager();
		}

		return mInstance;
	}

	private GameManager() {

	}

	public void setup(Context cxt) {
		mContext = cxt;
	}

	/**
	 * 检查当前网络
	 * @return 1 Wifi网络可用，2 3G网络可用  -1 无网络连接
	 */
	public int checkNetWork() {
		int ret = -1;

		final ConnectivityManager connMgr = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);

		final android.net.NetworkInfo wifi = connMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		final android.net.NetworkInfo mobile = connMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);

		if (wifi.isAvailable() && wifi.isConnected()) {
			ret = 1;
		}

		if (mobile.isAvailable() && mobile.isConnected()) {
			ret = 2;
		}

		return ret;
	}

	public void writeToClipboard(String data) {
		ClipboardManager clipboard = (ClipboardManager) mContext.getSystemService(Context.CLIPBOARD_SERVICE);

		ClipData textCd = ClipData.newPlainText(null, data);
		clipboard.setPrimaryClip(textCd);

		XYLog.d("write data : " + data);
		XYLog.d("get data from Clipboard : " + data);
	}

	public String readFromClipboard() {
		ClipboardManager clipboard = (ClipboardManager) mContext.getSystemService(Context.CLIPBOARD_SERVICE);

		String ret = "";
		if (clipboard.hasPrimaryClip()) {
			ret = clipboard.getPrimaryClip().getItemAt(0).getText().toString();
		}

		XYLog.d("get data from Clipboard : " + ret);

		return ret;
	}
}
