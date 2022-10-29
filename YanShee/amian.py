import YanAPI as Yan

Yan.yan_api_init("192.168.1.132")
at_left = True


def tts(ts: str, sy: bool = False):
    if sy:
        Yan.sync_do_tts(ts)
    else:
        Yan.start_voice_tts(ts)


def iat() -> str:
    return Yan.sync_do_voice_iat_value()


def motion(name: str = 'reset', direction: str = '', speed: str = 'normal', repeat: int = 1, sy: bool = True) -> int:
    if sy:
        return int(Yan.sync_play_motion(name, direction, speed, repeat))
    else:
        return Yan.start_play_motion(name, direction, speed, repeat)["data"]["total_time"] // 1000


def number1():
    left = True
    x = 0
    y = 0
    motion(sy=False)
    while iat() != "开始":
        pass
    motion("左走", repeat=2)
    x -= 2
    motion("复位")
    motion("walk", "forward", repeat=7)
    infrared = Yan.get_sensors_infrared_value() / 10
    if infrared < 5.5:  # 如果前方有障碍物
        left = False
        motion()
        tts("检测到障碍物距离约为" + str(infrared) + "厘米")
        motion("右走", repeat=5)
        x += 5
        motion()
    motion("跨")
    if left:
        motion("右走")
        motion("复位")
    motion("walk", "forward", repeat=7)

    if x > 0:
        motion("左走", repeat=2)
    else:
        motion("右走", repeat=2)
    tts("到达指定位置")


def get_col_rec() -> str:
    col_dict = {"no color detected": "没有检测到颜色", "pink": "粉", "red": "红", "green": "绿", "blue": "蓝", "yellow": "黄",
                "cyan": "青", "magenta": "洋红", "orange": "橙", "violet": "紫", "brown": "棕", "black": "黑", "white": "白",
                "gray": "灰"}
    res = Yan.sync_do_color_recognition()["data"]["color"][0]["name"]
    for i in col_dict:
        if i == res:
            tts("识别到" + col_dict[i] + "色物品")
            print("col_rec: %s" % col_dict[i])
            return col_dict[i]
    return "get_col_rec error"


def get_iat_col() -> str:
    col_lt = ["粉", "红", "绿", "蓝", "黄", "青", "洋红", "橙", "紫", "棕", "黑", "白", "灰"]
    idx = -1
    while idx == -1:
        tts("搬运物体是什么")
        res = iat()
        for i in col_lt:
            idx = res.find(i)
            if idx != -1:
                tts(res)
                print("iat_col: %s" % res[idx])
                return res[idx]
    return "get_iat_col error"


def number2():
    motion()
    iat_col = get_iat_col()
    motion("识别")
    col_rec = get_col_rec()
    if iat_col == col_rec:
        motion("抱")
    else:
        motion("walk", "backward", speed="slow", repeat=2)
        motion("后抱")
        motion("抱识别")
        get_col_rec()
        global at_left
        at_left = False
    motion("右走", repeat=6)


def number3(_n: str):
    qr = Yan.sync_do_QR_code_recognition(0)["data"]["contents"][0]
    if qr == _n:
        if not at_left:
            motion("walk", "forward", repeat=2)
        motion("放")
    else:
        if at_left:
            motion("walk", "backward", speed="slow", repeat=2)

        motion("右看")
        qr = Yan.sync_do_QR_code_recognition(0)["data"]["contents"][0]
        if qr == _n:
            motion("右放")
        else:
            motion("后放")
    motion()
    tts("已到达指定位置")


number1()
number2()
number3('2')
