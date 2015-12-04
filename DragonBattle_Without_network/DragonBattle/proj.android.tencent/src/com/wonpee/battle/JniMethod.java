package com.wonpee.battle;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.ClipboardManager;
import android.content.Context;

import com.wonpee.battle.log.XYLog;

public class JniMethod {
	public static native int nativeTest();

	public static native void commandToNative(int command, String data);

	// Native call java codes
	public static int onNativeCommand(int command, String data) {
		switch (command) {
		case NativeCommand.CMD_LOGIN_WITH_QQ: {
			ThirdPlatform.getInstance().apiWGLoginWithQQ();
			break;
		}
		case NativeCommand.CMD_LOGIN_WITH_WX: {
			ThirdPlatform.getInstance().apiWGLoginWithWeixin();
			break;
		}
		case NativeCommand.CMD_LOGOUT: {
			ThirdPlatform.getInstance().apiWGLogout();
			break;
		}
		case NativeCommand.CMD_SHARE_WITH_QQ: {
			ThirdPlatform.getInstance().apiWGSendToQQ_hide_qzone();
			break;
		}
		case NativeCommand.CMD_SHARE_WITH_WX: {
			ThirdPlatform.getInstance().apiWGSendToQQ_auto_qzone();
			break;
		}
		case NativeCommand.CMD_UNIPAY_CBK: {
			ThirdPlatform.getInstance().confirmSaveGoods(data);
			break;
		}

		default: {
			break;
		}
		}
		return 1;
	}

	public static void onWriteToClipboard(String data) {
		XYLog.d("apple...onWriteToClipboard...." + data);
		// ClipboardManager cm = (ClipboardManager) ThirdPlatform.getInstance()
		// .getMainContext().getSystemService(Context.CLIPBOARD_SERVICE);
		// XYLog.d("apple...onWriteToClipboard...." + data);
		// //cm.setText(data);
		// XYLog.d("apple...onWriteToClipboard...." + data);
	}

	public static String onReadFromClipboard() {
		ClipboardManager cm = (ClipboardManager) ThirdPlatform.getInstance()
				.getMainContext().getSystemService(Context.CLIPBOARD_SERVICE);
		return cm.getText().toString();
	}

	public static String getUserInfoForPayment() {

		XYLog.d("apple...getUserInfoForPayment....");

		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("pf", ThirdPlatform.getInstance().pf);
			jsonObj.put("pfkey", ThirdPlatform.getInstance().pf_key);
			jsonObj.put("openid", ThirdPlatform.getInstance().open_id);
			jsonObj.put("openkey", ThirdPlatform.getInstance().qqAccessToken);
			jsonObj.put("pay_token", ThirdPlatform.getInstance().userKey);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		XYLog.d("apple...getUserInfoForPayment...." + jsonObj.toString());

		return jsonObj.toString();
	}
}
