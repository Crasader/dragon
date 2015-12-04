package com.wonpee.battle;

import com.tencent.msdk.api.LoginRet;
import com.tencent.msdk.api.ShareRet;
import com.tencent.msdk.api.TokenRet;
import com.tencent.msdk.api.WGPlatformObserver;
import com.tencent.msdk.api.WakeupRet;
import com.tencent.msdk.consts.CallbackFlag;
import com.tencent.msdk.consts.TokenType;
import com.wonpee.battle.log.XYLog;

class MsdkCallback implements WGPlatformObserver { // 游戏需要根据自己的逻辑实现自己的MsdkCallback对象

	private final static String TAG = MsdkCallback.class.getSimpleName();

	public void OnLoginNotify(LoginRet ret) {
		XYLog.d(TAG, "apple....OnLoginNotify");
		switch (ret.flag) {
		case CallbackFlag.eFlag_Succ:
			// 登陆成功, 读取各种票据
			ThirdPlatform.getInstance().open_id = ret.open_id;
			ThirdPlatform.getInstance().userId = ret.user_id;
			ThirdPlatform.getInstance().pf = ret.pf;
			ThirdPlatform.getInstance().pf_key = ret.pf_key;

			String qqAccessToken = "";
			long qqAccessTokenExpire = 0;

			String qqPayToken = "";
			// long qqPayTokenExpire = 0;

			for (TokenRet tr : ret.token) {
				switch (tr.type) {
				case TokenType.eToken_QQ_Access: // 接收QQ accessToken
					qqAccessToken = tr.value;
					qqAccessTokenExpire = tr.expiration;
					break;
				case TokenType.eToken_QQ_Pay: // 接收QQ payToken
					qqPayToken = tr.value;
					// qqPayTokenExpire = tr.expiration;
					break;
				default:
					break;
				}
			}

			ThirdPlatform.getInstance().qqAccessToken = qqAccessToken;
			ThirdPlatform.getInstance().userKey = qqPayToken;

			XYLog.d("apple..Auto Login With Tencent ..." + qqAccessToken);

			ThirdPlatform.getInstance().cbkLoginWithQQ(ThirdPlatform.getInstance().open_id,
					qqAccessTokenExpire);

			break;
		case CallbackFlag.eFlag_WX_UserCancel:
		case CallbackFlag.eFlag_WX_NotInstall:
		case CallbackFlag.eFlag_WX_NotSupportApi:
		case CallbackFlag.eFlag_WX_LoginFail:
			// 登陆失败处理
			XYLog.d(TAG, "apple..." + ret.flag + "   " + ret.desc);
			break;
		default:
			XYLog.d(TAG, "apple..." + ret.desc);
			break;
		}
	}

	public void OnShareNotify(ShareRet ret) {
		XYLog.d(TAG, "apple...OnShareNotify called");
		switch (ret.flag) {
		case CallbackFlag.eFlag_Succ:
			// 分享成功
			ThirdPlatform.getInstance().platform = ret.platform;
			break;
		case CallbackFlag.eFlag_QQ_UserCancel:
		case CallbackFlag.eFlag_QQ_NetworkErr:
			// 分享失败处理
			XYLog.d(TAG, ret.desc);
			break;

		case CallbackFlag.eFlag_WX_UserCancel:
		case CallbackFlag.eFlag_WX_NotInstall:
		case CallbackFlag.eFlag_WX_NotSupportApi:
			// 分享失败处理
			XYLog.d(TAG, ret.desc);
			break;
		default:
			break;
		}
	}

	public void OnWakeupNotify(WakeupRet ret) {
		XYLog.d(TAG, "apple...OnWakeupNotify called");
		XYLog.d(TAG, ret.toString());
	}
}