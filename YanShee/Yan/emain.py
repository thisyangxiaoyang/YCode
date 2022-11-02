import YanAPI as Yan
import time as t

Yan.yan_api_init("192.168.1.131")
at_left = True
x = 0
box_num = None


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
    Yan.start_color_recognition()
    t.sleep(2.5)
    col_rec = Yan.get_visual_task_result("color", "color_detect")["data"]["color"][0]["name"]
    Yan.stop_color_recognition()
    print("col_rec: %s" % col_rec)
    for i in col_dt:
        if i == col_rec:
            tts("识别到" + col_dt[i] + "色物品")
            print("get_col_rec: %s" % col_dt[i])
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
                print("get_iat_col: %s" % i)
                tts(iat_res, False)
                return i
    return "error"


def number1(left: bool = True):
    global x
    motion()
    while iat().find("开始") == -1:
        pass
    if left:
        motion("左走", repeat=2)
        x -= 2
        motion()
        motion("walk", "forward", repeat=7)
        infrared = Yan.get_sensors_infrared_value() / 10.0
        if infrared < 5.5:
            tts("识别到障碍物距离约为" + str(infrared) + "厘米", False)
            motion("右走", repeat=4)
            x += 4
        motion()
        motion("跨")
        motion("walk", "forward", repeat=7)
    else:
        motion("右走", repeat=3)
        x += 3
        motion()
        motion("walk", "forward", repeat=7)
        infrared = Yan.get_sensors_infrared_value() / 10.0
        if infrared < 5.5:
            tts("识别到障碍物距离约为" + str(infrared) + "厘米", False)
            motion("左走", repeat=4)
            x -= 4
        motion()
        motion("跨")
        motion("walk", "forward", repeat=9)
    motion()
    if x > 0:
        # motion("左走", repeat=x - x)
        pass
    elif x < 0:
        motion("右走", repeat=-x)
    x = 0
    motion("walk", direction="forward", repeat=1)
    tts("已到达指定位置")


def number2():
    motion()

    iat_col = get_iat_col()
    motion("识别")
    col_rec = get_col_rec()
    motion("复位")
    if iat_col == col_rec:
        motion("抱")
    else:
        global at_left
        at_left = False
        motion("walk", "backward", repeat=2)
        motion("后抱")
        get_col_rec()
    motion("抱复位")
    global box_num
    box_num = Yan.sync_do_QR_code_recognition(-1)["data"]["contents"][0]
    print("box_num: %s" % box_num)
    motion("右走", repeat=7)


def number3():
    qr = Yan.sync_do_QR_code_recognition(10)["data"]["contents"][0]
    print("qr: %s" % qr)
    if qr == box_num:
        if not at_left:
            motion("walk", "forward", repeat=2)
        motion("放")
    else:
        if at_left:
            motion("walk", "backward", repeat=2)
        motion("右看")
        qr = Yan.sync_do_QR_code_recognition(10)["data"]["contents"][0]
        print("qr: %s" % qr)
        if qr == box_num:
            motion("右放")
        else:
            motion("后放")


if __name__ == "__main__":
    # number1(True)
    number2()
    number3()
