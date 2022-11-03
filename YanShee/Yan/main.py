import YanAPI as Yan

Yan.yan_api_init("192.168.1.128")
at_left = True
x = 0


def motion(name: str = "复位", direction: str = "", speed: str = "normal", repeat: int = 1, sy: bool = True) -> any:
    if sy:
        return Yan.sync_play_motion(name, direction, speed, repeat)
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
    col_res = Yan.sync_do_color_recognition()["data"]["color"][0]["name"]
    print("col_res: %s" % col_res)
    for i in col_dt:
        if i == col_res:
            print("get_col_rec: %s" % col_dt[i])
            tts("识别到" + col_dt[i] + "色物品", sy=False)
            return col_dt[i]
    return "error"


def get_iat_col() -> str:
    col_lt = ["粉", "红", "绿", "蓝", "黄", "青", "洋红", "橙", "紫", "棕", "黑", "白", "灰"]
    idx = -1
    while idx == -1:
        tts("搬运物体是什么")
        iat_res = iat()
        print("iat_res: %s" % iat_res)
        for i in col_lt:
            idx = iat_res.find(i)
            if idx != -1:
                tts(iat_res)
                print("get_iat_res: %s" % i)
                return i
    return "error"


def number1(test: bool = True):
    global x
    motion()
    while iat() != "开始":
        pass
    if test:
        motion("左走", repeat=2)
        x -= 2
        motion()
        motion("walk", "forward", repeat=6)
        infrared = Yan.get_sensors_infrared_value() / 10.0
        if infrared < 5.5:
            tts("检测到障碍物距离约为" + str(infrared) + "厘米")
            motion("右走", repeat=4)
            x += 4
        motion("跨")
        motion("walk", "forward", repeat=8)
    else:
        motion("右走", repeat=2)
        x += 2
        motion()
        motion("walk", "forward", repeat=6)
        infrared = Yan.get_sensors_infrared_value() / 10.0
        if infrared < 5.5:
            tts("检测到障碍物距离约为" + str(infrared) + "厘米")
            motion("左走", repeat=4)
            x -= 4
        motion("跨")
        motion("walk", "forward", repeat=7)
    if x > 0:
        motion("左走", repeat=x)
    elif x < 0:
        motion("右走", repeat=-x)
    motion(sy=False)
    tts("已到达指定位置")


def number2():
    motion()
    iat_col = get_iat_col()
    motion("识别")
    col_rec = get_col_rec()
    if iat_col == col_rec:
        motion("抱")
    else:
        global at_left
        at_left = False
        motion("walk", "backward", speed="slow", repeat=2)
        motion("后抱")
        motion("抱识别")
        get_col_rec()
    motion("右走", repeat=6)


def number3():
    n = '2'
    qr = Yan.sync_do_QR_code_recognition(-1)["data"]["contents"][0]
    if qr == n:
        if not at_left:
            motion("walk", "forward", repeat=2)
        motion("放")
    else:
        if at_left:
            motion("walk", "backward", repeat=2)
        motion("右看")
        qr = Yan.sync_do_QR_code_recognition(-1)["data"]["contents"][0]
        if qr == n:
            motion("右放")
        else:
            motion("后放")
    tts("物品搬运成功", False)
    motion()


if __name__ == "__main__":
    number1(True)
    number2()
    number3()
