#!/usr/bin/env python
import argparse
import curses
import serial

MOD_KEY_UP = (91, 65)
MOD_KEY_DOWN = (91, 66)
MOD_ESCAPE = (-1, -1)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        'port',
        help='serial port name, e.g. COM7 (in windows), '
             '/dev/ttyUSB0 (in linux)')
    parser.add_argument(
        '--step', type=int, default=1,
        help='how much the phase speed changes when you press an arrow key')
    return parser.parse_args()


class CursesApp(object):
    def __init__(self, com, step):
        self.com = com
        self.step = step

    def run(self):
        self.screen = curses.initscr()
        curses.noecho()
        curses.cbreak()
        curses.curs_set(0)
        self.main_loop()

    def send_speed_command(self, speed):
        self.com.write(f's {speed}\n'.encode())

    def main_loop(self):
        self.screen.addstr(0, 0, 'Press arrows to change speed')
        speed = 0
        while True:
            ch = self.screen.getch()
            if ch == 27:  # it's a modkey
                self.screen.nodelay(True)
                mod_pair = (self.screen.getch(), self.screen.getch())
                self.screen.nodelay(False)
                if mod_pair == MOD_KEY_UP:
                    speed = 0 if speed > 0 else (speed - self.step)
                    self.send_speed_command(speed)
                elif mod_pair == MOD_KEY_DOWN:
                    speed = 0 if speed < 0 else (speed + self.step)
                    self.send_speed_command(speed)
                elif mod_pair == MOD_ESCAPE:
                    break

            self.screen.clear()
            self.screen.addstr(0, 0, 'Phase change speed:')
            self.screen.addstr(1, 0, f'{speed} ticks per interrupt')
            self.screen.refresh()

        curses.endwin()


def main():
    args = parse_args()
    com = serial.Serial(args.port, baudrate=115200)
    app = CursesApp(com, args.step)
    app.run()
    com.close()


if __name__ == '__main__':
    main()
