#ifndef JNI_H
#define JNI_H

#define CMD_LOGIN_WITH_QQ  	1
#define CMD_LOGIN_WITH_WX  	2
#define CMD_LOGOUT  		3
#define CMD_SHARE_WITH_QQ   4
#define CMD_SHARE_WITH_WX   5
#define CMD_UNIPAY_CBK      10

/* notify native code cmd */
#define CMD_UNIPAY_REFRESH  101

#define CMD_LOGIN_WITH_QQ_NOTE      "CMD_LOGIN_WITH_QQ_NOTE"
#define CMD_LOGIN_WITH_WX_NOTE      "CMD_LOGIN_WITH_WX_NOTE"
#define CMD_LOGOUT_NOTE             "CMD_LOGOUT_NOTE"

#define CMD_SHARE_WITH_QQ_NOTE      "CMD_SHARE_WITH_QQ_NOTE"
#define CMD_SHARE_WITH_WX_NOTE      "CMD_SHARE_WITH_WX_NOTE"

extern "C" {
/**
 * @brief notifyToJava 在UI线程中向JavaCode通知
 * @param command      命令号
 * @param data         数据 建议使用json
 * @return
 */
int notifyToJava(int command, const char *data);
void writeToClipboard(const char* data);
const char* readFromClipboard();

const char* getUserInfoForPayment();


}
#endif // JNI_H
