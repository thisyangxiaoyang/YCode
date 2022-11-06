import YanAPI as Yan
import time

ip_addr = "192.168.1.122"
Yan.yan_api_init(ip_addr)  # 初始化YanAPI
at_left = True  # 二阶段位置
box_num = None  # 保存物品放入位置


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


def get_col_rec(_time_out: int = 3) -> str:
    col_dt = {"no color detected": "没有检测到颜色", "pink": "粉", "red": "红", "green": "绿", "blue": "蓝", "yellow": "黄",
              "cyan": "青", "magenta": "洋红", "orange": "橙", "violet": "紫", "brown": "棕", "black": "黑", "white": "白",
              "gray": "灰"}
    Yan.start_color_recognition()
    if _time_out <= 0:
        time.sleep(3)
    else:
        time.sleep(_time_out)
    Yan.stop_color_recognition()
    col_res = Yan.get_visual_task_result("color", "color_detect")["data"]["color"][0]["name"]
    print("col_res: %s" % col_res)
    for i in col_dt:
        if i == col_res:
            print("get_col_rec: %s" % col_dt[i])
            tts("识别到" + col_dt[i] + "色物品")
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
                print("get_iat_col: %s " % i)
                tts(iat_res)
                return i
    return "error"


def qr_handle(time_out: int = 8) -> str:
    qr_res = Yan.sync_do_QR_code_recognition(time_out)
    print("qr_res: %s" % qr_res)
    if qr_res["code"] != 0 or qr_res["msg"] != "Success":  # 如果二维码识别失败
        return '2'
    qr_contents = qr_res["data"]["contents"]
    if 0 == len(qr_contents):
        return '2'
    else:
        return qr_contents[0]


def number1(left: bool = True):
    x = 0
    motion()
    while iat().find("开始") == -1:
        pass
    if left:
        motion("左走", repeat=2)
        x -= 2
        motion()
        motion("walk", "forward", repeat=6)
        motion()
        print(Yan.get_sensors_infrared())
        infrared = Yan.get_sensors_infrared_value()
        if infrared / 10.0 < 5.5:
            tts("检测到障碍物距离约为" + str(infrared) + "厘米")
            motion("右走", repeat=4)
            x += 4
            motion()
        motion("跨")
        motion("walk", "forward", repeat=6)
    else:
        motion("右走", repeat=2)
        x += 2
        motion()
        motion("walk", "forward", repeat=6)
        motion()
        infrared = Yan.get_sensors_infrared_value() / 10.0
        if infrared < 5.5:
            tts("检测到障碍物距离约为" + str(infrared) + "厘米")
            motion("左走", repeat=4)
            x -= 4
            motion()
        motion("跨")
        motion("walk", "forward", repeat=7)
    print(x)
    if x < 0:  # left
        motion("右走", repeat=2)
    elif x > 0:  # right
        motion("左走", repeat=1)
    tts("已到达指定位置")


def number2():
    global at_left
    global box_num
    motion()
    iat_col = get_iat_col()
    motion("识别")
    col_rec = get_col_rec()
    motion()
    if iat_col == col_rec:
        motion("抱")
    else:
        at_left = False
        motion("walk", "backward", repeat=2)
        motion("后抱")
        get_col_rec()
    motion("抱复位")
    box_num = qr_handle(-1)
    motion("右走", repeat=5)


def number3():
    qr_result = qr_handle(-1)
    if qr_result == box_num:
        if not at_left:
            motion("walk", "forward", repeat=2)
        motion("放")
    else:
        if at_left:
            motion("walk", "backward", repeat=2)
        motion("右看")
        qr_result = qr_handle(7)
        if qr_result == box_num:
            motion("右放")
        else:
            motion("后放")
    tts("物品搬运完成", False)
    motion()


if __name__ == "__main__":
    number1(True)
    number2()
    number3()
    pass
