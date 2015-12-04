/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.wonpee.battle;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationSet;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.tencent.msdk.api.WGPlatform;
import com.tencent.msdk.tools.Logger;
import com.tencent.tmgp.gedou.R;

public class DragonBattleActivity extends Cocos2dxActivity {

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
			this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}

		ThirdPlatform.getInstance().initApi(this);

		addLoadingAni();
	}

	private void addLoadingAni() {
		ViewGroup.LayoutParams framelayout_params = new ViewGroup.LayoutParams(
				ViewGroup.LayoutParams.MATCH_PARENT,
				ViewGroup.LayoutParams.MATCH_PARENT);

		final LinearLayout bgLayout = new LinearLayout(this);
		bgLayout.setBackgroundColor(Color.BLACK);
		bgLayout.setLayoutParams(framelayout_params);
		mFramelayout.addView(bgLayout);

		final ImageView loadingImg = new ImageView(this);
		loadingImg.setImageDrawable(this.getResources().getDrawable(
				R.drawable.loading));
		loadingImg.setLayoutParams(framelayout_params);
		mFramelayout.addView(loadingImg);

		AlphaAnimation alphaAnimation = new AlphaAnimation(1, 0);
		alphaAnimation.setDuration(3000);
		alphaAnimation.setAnimationListener(new AnimationListener() {

			@Override
			public void onAnimationEnd(Animation animation) {
				mFramelayout.removeView(loadingImg);
				mFramelayout.removeView(bgLayout);
			}

			@Override
			public void onAnimationRepeat(Animation animation) {

			}

			@Override
			public void onAnimationStart(Animation animation) {

			}

		});

		AnimationSet animationSet = new AnimationSet(true);
		animationSet.setInterpolator(new AccelerateInterpolator());
		animationSet.addAnimation(alphaAnimation);
		loadingImg.startAnimation(alphaAnimation);
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// TestGame should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	@Override
	public void onStart() {
		ThirdPlatform.getInstance().bindUnipayService(this);
		super.onStart();
	}

	@Override
	public void onStop() {
		ThirdPlatform.getInstance().unbindUnipayService();
		super.onStart();
	}

	@Override
	public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
		switch (pKeyCode) {
		case KeyEvent.KEYCODE_BACK:
			// case KeyEvent.KEYCODE_MENU:
			showExitDialog("", "");
			// int native_test = JniMethod.nativeTest();
			// Log.d("KeyDown", String.format("apple......%d", native_test));

			return true;
		default:
			return super.onKeyDown(pKeyCode, pKeyEvent);
		}
	}

	@Override
	protected void onNewIntent(Intent intent) {
		Logger.d("called");
		super.onNewIntent(intent);
		WGPlatform.handleCallback(intent); // 必须要调用这一句
	}

	// TODO 要实现onActivityResult并且调用WGPlatform.onActivityResult将手Q登陆返回的数据交由MSDK处理
	// START
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		Logger.d("called");
		WGPlatform.onActivityResult(requestCode, resultCode, data); // 必须调用这一句
	}

	static {
		System.loadLibrary("game");
		System.loadLibrary("NativeRQD");
	}

}
