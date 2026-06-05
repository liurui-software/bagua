#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import sys

LR_BG_VER = "3.0"

TRIGRAMS = [
    "乾上乾下 - 乾卦",
    "坤上坤下 - 坤卦",
    "坎上震下 - 屯卦",
    "艮上坎下 - 蒙卦",
    "坎上乾下 - 需卦",
    "乾上坎下 - 讼卦",
    "坤上坎下 - 师卦",
    "坎上坤下 - 比卦",
    "巽上乾下 - 小畜卦",
    "乾上兑下 - 履卦",
    "坤上乾下 - 泰卦",
    "乾上坤下 - 否卦",
    "乾上离下 - 同人卦",
    "离上乾下 - 大有卦",
    "坤上艮下 - 谦卦",
    "震上坤下 - 豫卦",
    "兑上震下 - 随卦",
    "艮上巽下 - 蛊卦",
    "坤上兑下 - 临卦",
    "巽上坤下 - 观卦",
    "离上震下 - 噬嗑卦",
    "艮上离下 - 贲卦",
    "艮上坤下 - 剥卦",
    "坤上震下 - 复卦",
    "乾上震下 - 无妄卦",
    "艮上乾下 - 大畜卦",
    "艮上震下 - 颐卦",
    "兑上巽下 - 大过卦",
    "坎上坎下 - 坎卦",
    "离上离下 - 离卦",
    "兑上艮下 - 咸卦",
    "震上巽下 - 恒卦",
    "乾上艮下 - 遁卦",
    "震上乾下 - 大壮卦",
    "离上坤下 - 晋卦",
    "坤上离下 - 明夷卦",
    "巽上离下 - 家人卦",
    "离上兑下 - 睽卦",
    "坎上艮下 - 蹇卦",
    "震上坎下 - 解卦",
    "艮上兑下 - 损卦",
    "巽上震下 - 益卦",
    "兑上乾下 - 夬卦",
    "乾上巽下 - 姤卦",
    "兑上坤下 - 萃卦",
    "坤上巽下 - 升卦",
    "坎上兑下 - 困卦",
    "巽上坎下 - 井卦",
    "离上兑下 - 革卦",
    "离上巽下 - 鼎卦",
    "震上震下 - 震卦",
    "艮上艮下 - 艮卦",
    "巽上艮下 - 渐卦",
    "震上兑下 - 归妹卦",
    "震上离下 - 丰卦",
    "离上艮下 - 旅卦",
    "巽上巽下 - 巽卦",
    "兑上兑下 - 兑卦",
    "巽上坎下 - 涣卦",
    "坎上兑下 - 节卦",
    "巽上兑下 - 中孚卦",
    "震上艮下 - 小过卦",
    "坎上离下 - 既济卦",
    "离上坎下 - 未济卦"
]


def wait_for_any_key():
    """
    跨平台等待单个按键
    """
    try:
        import msvcrt
        msvcrt.getch()
    except ImportError:
        import tty
        import termios

        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)

        try:
            tty.setraw(fd)
            sys.stdin.read(1)
        finally:
            termios.tcsetattr(
                fd,
                termios.TCSADRAIN,
                old_settings
            )


def bg_name(bg):
    """
    三爻 -> 八卦名
    """

    table = {
        (1, 1, 1): "坤",
        (0, 1, 1): "震",
        (1, 0, 1): "坎",
        (0, 0, 1): "兑",
        (1, 1, 0): "艮",
        (0, 1, 0): "离",
        (1, 0, 0): "巽",
        (0, 0, 0): "乾",
    }

    return table[tuple(bg)]


def bg_str(bg):
    """
    六爻 -> 六十四卦名称
    """

    lower = bg_name(bg[0:3])
    upper = bg_name(bg[3:6])

    prefix = f"{upper}上{lower}下"

    for item in TRIGRAMS:
        if item.startswith(prefix):
            return item

    return prefix


def main():

    print("----------------------------")
    print(f"LR八卦生成器 (版本{LR_BG_VER})")
    print("----------------------------\n")

    bg0 = []
    seq = 0

    for i in range(6):
        print(f"请按键：第{i + 1}爻!")

        start = time.perf_counter_ns()

        wait_for_any_key()

        t = (time.perf_counter_ns() - start) // 1_000_000

        bg0.append(t % 2)

        seq += t

    change = seq % 7

    bg1 = bg0.copy()

    if change:
        bg1[change - 1] ^= 1

    bg0s = bg_str(bg0)
    bg1s = bg_str(bg1)

    print()
    print(f" {bg0s}")
    print(f" {bg1s}")
    print(f" (变{change})")

    print("\n按任意键退出...")
    wait_for_any_key()


if __name__ == "__main__":
    main()