#!/usr/bin/env python3
import argparse
import cv2
import numpy as np
import serial
import time

PHASE_SPEED = 1
TOGGLE_S = 2.0


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--serial-path', default='/dev/ttyACM0')
    parser.add_argument('--camera-index', type=int, default=0)
    parser.add_argument('--position', type=int, default=30)
    parser.add_argument('--updown', action='store_true')
    parser.add_argument('--pixels2egu', type=float, default=0.1)
    return parser.parse_args()


class BoardManager(object):
    def connect(self, path):
        self.serial = serial.Serial(path, 115200, timeout=1)
        return self.serial.is_open

    def stop(self):
        return self.set_speed(0)

    def set_speed(self, val):
        return self.send_command(b's %d\n' % (val,))

    def set_increment(self, val):
        return self.send_command(b'i %d\n' % (val,))

    def send_command(self, command):
        self.serial.write(command)
        ans = self.serial.read_until(b'\n')
        return ans.startswith(b'OK')


def input_roi(video):
    success, img = video.read()
    bbox = cv2.selectROI("Tracking", img, False)
    cv2.destroyAllWindows()
    return img, bbox


def main():
    args = parse_args()
    pos = args.position
    video = cv2.VideoCapture(args.camera_index)
    if not video.isOpened():
        print("Failed to open camera")
        return

    board_manager = BoardManager()
    if not board_manager.connect(args.serial_path):
        print("Failed to connect to board")
        return

    board_manager.stop()

    # might be better to initialize ROI using the hough algorithm instead?
    img, bbox = input_roi(video)
    tracker = cv2.TrackerKCF_create()
    tracker.init(img, bbox)
    last_time = time.time()
    toggling_speed = 1
    while True:
        ret, frame = video.read()
        ok, bbox = tracker.update(frame)
        if ok:
            # Tracking success
            p1 = (int(bbox[0]), int(bbox[1]))
            p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
            mid_y = int(
                (frame.shape[0] - (bbox[1] + bbox[3]) / 2) * args.pixels2egu)
            cv2.rectangle(frame, p1, p2, (255, 0, 0), 2, 1)
            cv2.putText(
                frame, f'{mid_y}', (100, 80),
                cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 2)
            if mid_y == pos:
                board_manager.stop()
            elif mid_y > pos:
                board_manager.set_speed(PHASE_SPEED)
            elif mid_y < pos:
                board_manager.set_speed(-PHASE_SPEED)

        else:
            # Tracking failure
            cv2.putText(
                frame, "Tracking failure detected", (100, 80),
                cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 2)
            if args.updown:
                current_time = time.time()
                # make sample go up and down on tracking failure
                if current_time >= last_time + TOGGLE_S:
                    last_time = current_time
                    toggling_speed *= -1
                    board_manager.set_speed(toggling_speed)
            else:
                board_manager.stop()

        cv2.imshow('frame', frame)

        # press escape to exit
        if cv2.waitKey(1) & 0xff == 27:
            break

    board_manager.stop()
    video.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
