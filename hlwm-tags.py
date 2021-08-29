import subprocess
import serial

def update_tags(ser):
    proc = subprocess.run(
        ['herbstclient', 'tag_status'],
        check=True,
        capture_output=True,
        text=True,
    )
    status = [tag[0] for tag in proc.stdout.split('\t') if tag]
    command = f"hlwm.tagstatus {''.join(status)}\n"
    print(command)
    ser.write(command.encode('ascii'))


def hc_idle(ser):
    hooks = ['tag_changed', 'tag_flags']
    proc = subprocess.Popen(
        ['herbstclient', '--idle', '|'.join(hooks)],
        stdout=subprocess.PIPE,
    )
    while (line := proc.stdout.readline()):
        print(line.decode('utf-8'))
        update_tags(ser)
        
    


if __name__ == '__main__':
    ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
    update_tags(ser)
    hc_idle(ser)
