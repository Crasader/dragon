package com.wonpee.battle;

import android.os.Handler;
import android.os.Message;
import android.os.RemoteException;
import android.widget.Toast;

import com.tencent.unipay.plugsdk.IUnipayServiceCallBack;
import com.wonpee.battle.log.XYLog;

public class UnipayServiceCallBack extends IUnipayServiceCallBack.Stub {

	private static String TAG = UnipayServiceCallBack.class.getSimpleName();

	private int retCode = -100;

	@Override
	public void UnipayCallBack(int resultCode, int payChannel, int payState,
			int providerState, int saveNum, String resultMsg, String extendInfo)
			throws RemoteException {
		// TODO Auto-generated method stub
		XYLog.d(TAG, "UnipayCallBack \n" + "\nresultCode = " + resultCode
				+ "\npayChannel = " + payChannel + "\npayState = " + payState
				+ "\nproviderState = " + providerState);

		retCode = resultCode;
		handler.sendEmptyMessage(0);
	}

	@Override
	public void UnipayNeedLogin() throws RemoteException {
		// TODO Auto-generated method stub
		XYLog.d(TAG, "UnipayNeedLogin");
	}

	Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			Toast.makeText(ThirdPlatform.getInstance().getMainContext(),
					"call back retCode=" + String.valueOf(retCode),
					Toast.LENGTH_SHORT).show();

			// notify native code.
			JniMethod.commandToNative(NativeCommand.CMD_UNIPAY_REFRESH, null);
		}
	};

}
