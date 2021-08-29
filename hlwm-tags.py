#!/usr/bin/python3

import subprocess
import glob
import serial


def update_tags():
    proc = subprocess.run(
        ['herbstclient', 'tag_status'],
        check=True,
        capture_output=True,
        text=True,
    )
    status = [tag[0] for tag in proc.stdout.split('\t') if tag]
    write_command(f"hlwm.tagstatus {''.join(status)}\n")


def write_command(command):
    print(command.strip())
    dev = find_device()
    if dev is None:
        return

    with serial.Serial(dev, 9600, timeout=1) as ser:
        ser.write(command.encode('ascii'))


def hc_idle():
    hooks = ['tag_changed', 'tag_flags']
    proc = subprocess.Popen(
        ['herbstclient', '--idle', '|'.join(hooks)],
        stdout=subprocess.PIPE,
    )
    while (line := proc.stdout.readline()):
        print(line.decode('utf-8'))
        update_tags()


def find_device():
    devices = glob.glob("/dev/serial/by-id/usb-Keyboardio_*")
    if len(devices) != 1:
        print(f"Didn't find suitable device: {devices}")
        return None
    return devices[0]


if __name__ == '__main__':
    update_tags()
    hc_idle()
