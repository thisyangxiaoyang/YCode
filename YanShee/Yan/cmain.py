import YanAPI as Yan

at_left = True
Yan.yan_api_init("192.168.1.143")
x = 0


def motion(name: str = "reset", direction: str = "", speed: str = "normal", repeat: int = 1, sy: bool = True) -> any:
    if sy:
        return Yan.sync_play_motion(name, direction, speed, repeat)
    else:
        return Yan.start_play_motion(name, direction, speed, repeat)["data"]["total_time"] // 1000


def tts(ts: str, sy: bool = True):
    if sy:
        Yan.sync_do_tts(ts)
    else:
        Yan.start_voice_tts(ts)


def iat() -> str:
    return Yan.sync_do_voice_iat_value()


def get_col_rec() -> str:
    col_dt = {"no color detected": "没有检测到颜色", "pink": "粉", "red": "红", "green": "绿", "blue": "蓝", "yellow": "黄",
              "cyan": "青", "magenta": "洋红", "orange": "橙", "violet": "紫", "brown": "棕", "black": "黑", "white": "白",
              "gray": "灰"}
    col_rec = Yan.sync_do_color_recognition()["data"]["color"][0]["name"]
    print("col_rec: %s" % col_rec)
    for i in col_dt:
        if i == col_rec:
            print("get_col_rec: %s" % col_dt[i])
            tts("识别到" + col_dt[i] + "色物品")
            return col_dt[i]
    return "get_col_rec: error"


def get_iat_col() -> str:
    col_lt = ["粉", "红", "绿", "蓝", "黄", "青", "洋红", "橙", "紫", "棕", "黑", "白", "灰"]
    idx = -1
    while idx == -1:
        tts("搬运物体是什么")
        iat_res = Yan.sync_do_voice_iat_value()
        print("iat_res: %s" % iat_res)
        for i in col_lt:
            idx = iat_res.find(i)
            if idx != -1:
                print("get_iat_col: %s" % i)
                tts(iat_res, False)
                return i
    return "get_iat_col: error"


def number1(ts: bool):
    global x
    motion()
    while iat() != "开始":
        pass
    if ts:
        motion("左走", repeat=2)
        x -= 2
        motion("复位")
        motion("walk", "forward", repeat=7)
        motion()
        infrared = Yan.get_sensors_infrared_value() / 10.0
        if infrared < 5.5:
            tts("检测到障碍物距离约为" + str(infrared) + "厘米")
            motion("右走", repeat=4)
            x += 4
    else:
        motion("右走", repeat=2)
        x += 2
        motion()
        motion("walk", "forward", repeat=7)
        motion()
        infrared = Yan.get_sensors_infrared_value() / 10.0
        if infrared < 5.5:
            tts("检测到障碍物距离约为" + str(infrared) + "厘米")
            motion("左走", repeat=4)
            x -= 4
    motion()
    motion("跨")
    if ts:
        motion("walk", "forward", repeat=7)
    else:
        motion("walk", "forward", repeat=7)

    motion()
    if x > 0:
        motion("左走", repeat=x)
    elif x < 0:
        motion("右走", repeat=-x + 1)
    x = 0
    tts("已到达指定位置", False)


def number2():
    motion()
    iat_col = get_iat_col()
    motion("识别")
    col_rec = get_col_rec()
    if iat_col == col_rec:
        motion("抱")
    else:
        motion()
        global at_left
        at_left = False
        motion("walk", "backward", "very slow", repeat=2)
        motion("后抱")
        motion("抱识别")
        get_col_rec()
    motion("右走", repeat=7)


def number3():
    n = 'nul'
    qr = Yan.sync_do_QR_code_recognition(-1)["data"]["contents"][0]
    if qr == '1':
        if not at_left:
            motion("walk", "forward", repeat=2)
        motion("放")
    else:
        if at_left:
            motion("walk", "backward", "very slow",  repeat=2)
        motion("右看")
        qr = Yan.sync_do_QR_code_recognition(-1)["data"]["contents"][0]
        if qr == '2':
            motion("右放")
        else:
            motion("后放")
    tts("物品搬运完成")
    motion()


if __name__ == "__main__":
    number1(True)
    number2()
    number3()
