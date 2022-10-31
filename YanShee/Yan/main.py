from YanAPI import *

yan_api_init("192.168.1.110")
at_left = True


def motion(name: str = "reset", direction: str = "", speed: str = "normal", repeat: int = 1) -> int:
    return start_play_motion(name, direction, speed, repeat)["data"]["total_time"] // 1000


def smotion(name: str = "reset", direction: str = "", speed: str = "normal", repeat: int = 1) -> bool:
    return sync_play_motion(name, direction, speed, repeat)


def tts(_ts: str, _sync: bool = False):
    if _sync:
        sync_do_tts(_ts)
    else:
        start_voice_tts(_ts)


def iat() -> str:
    return sync_do_voice_iat_value()


def number1():
    x = 0
    y = 0
    # map_len = 13
    motion()
    while iat() != "开始执行任务":
        pass
    smotion("左走", repeat=2)
    x -= 2
    smotion()
    smotion("walk", "forward", repeat=6)
    y += 6
    infrared = get_sensors_infrared_value() / 10.0
    if infrared < 5.0:
        tts("检测到障碍物距离约为" + str(int(infrared)) + "厘米")
        smotion("右走", repeat=4)
        smotion()
    smotion("跨")
    smotion("walk", "forward", repeat=6)
    if x < 0:
        smotion("右走", repeat=-x)
    elif x > 0:
        smotion("左走", repeat=x)
    x = 0
    tts("已到达指定位置", False)


def get_col_rec() -> str:
    col_dict = {"no color detected": "没有检测到颜色", "pink": "粉", "red": "红", "green": "绿", "blue": "蓝",
                "yellow": "黄", "cyan": "青", "magenta": "洋红", "orange": "橙", "violet": "紫", "brown": "棕",
                "black": "黑", "white": "白", "gray": "灰"}
    col_res = sync_do_color_recognition()["data"]["color"][0]["name"]
    print("col_res: %s" % col_res)
    for i in col_dict:
        if i == col_res:
            print("col_dict[i]: %s" % col_dict[i])
            tts("识别到" + col_dict[i] + "色物品", False)
            return col_dict[i]
    return "无"


def get_iat_col() -> str:
    col_list = ["粉", "红", "绿", "蓝", "黄", "青", "洋红", "橙", "紫", "棕", "黑", "白", "灰"]
    idx = -1
    while idx == -1:
        iat_res = sync_do_voice_iat_value()
        print("iat_res: %s" % iat_res)
        for i in col_list:
            idx = iat_res.find(i)
            print("idx: %s" % idx)
            if idx != -1:
                print("iat_res[i]: %s" % iat_res[i])
                tts(iat_res, False)
                return iat_res[i]
    return "无"


def number2():
    smotion()
    tts("搬运物体是什么")
    iat_col = get_iat_col()
    smotion("识别")
    col_rec = get_col_rec()
    if iat_col == col_rec:
        smotion("抱")
    else:
        smotion("walk", "backward", repeat=2)
        smotion("后抱")
        smotion("抱识别")
        get_col_rec()
        global at_left
        at_left = False
    smotion("右走", repeat=4)


def number3(_n: str):
    qr_num = sync_do_QR_code_recognition(0)["data"]["contents"][0]
    if qr_num == _n:
        if not at_left:
            smotion("walk", "forward", repeat=2)
        smotion("放")
    else:
        if at_left:
            smotion("walk", "backward", repeat=2)
        smotion("右看")
        qr_num = sync_do_QR_code_recognition(0)["data"]["contents"][0]
        if qr_num == _n:
            smotion("右放")
        else:
            smotion("后放")
    tts("物品搬运完成")
    motion()


if __name__ == "__main__":
    print("test1")
    number1()
    number2()
    number3('1')
