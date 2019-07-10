# --------------------------------------------------------
# Camera sample code for Tegra X2/X1
#
# This program could capture and display video from
# IP CAM, USB webcam, or the Tegra onboard camera.
# Refer to the following blog post for how to set up
# and run the code:
#   https://jkjung-avt.github.io/tx2-camera-with-python/
#
# Written by JK Jung <jkjung13@gmail.com>
# --------------------------------------------------------

import sys
import argparse
import cv2
from datetime import datetime
import time


WINDOW_NAME = 'Insight your Eyes .. JW BlackBox'
#VIDEO_FILE_PATH = 'D:/VIDEO_TEST/SAVEVIDEO.avi'
#VIDEO_FILE_PATH = '/home/nvidia/test/SAVEVIDEO.avi'
VIDEO_FILE_PATH = ""
VIDEO_REC_TIME = 0
VIDEO_REC_TIMER = 0


def parse_args():
    # Parse input arguments
    desc = 'Capture and display live camera video on Jetson TX2/TX1'
    parser = argparse.ArgumentParser(description=desc)
    parser.add_argument('--rtsp', dest='use_rtsp',
                        help='use IP CAM (remember to also set --uri)',
                        action='store_true')
    parser.add_argument('--uri', dest='rtsp_uri',
                        help='RTSP URI, e.g. rtsp://192.168.1.64:554',
                        default=None, type=str)
    parser.add_argument('--latency', dest='rtsp_latency',
                        help='latency in ms for RTSP [200]',
                        default=200, type=int)
    parser.add_argument('--usb', dest='use_usb',
                        help='use USB webcam (remember to also set --vid)',
                        action='store_true')
    parser.add_argument('--vid', dest='video_dev',
                        help='device # of USB webcam (/dev/video?) [1]',
                        default=1, type=int)
    parser.add_argument('--width', dest='image_width',
                        help='image width [1920]',
                        default=1920, type=int)
    parser.add_argument('--height', dest='image_height',
                        help='image height [1080]',
                        default=1080, type=int)
    parser.add_argument('--path', dest='save_path',
                        help='AVI VIDEO File Save Path',
                        default='/home/nvidia/test/', type=str)
    parser.add_argument('--rectime', dest='rec_time',
                        help='AVI VIDEO REC time Default 30 second',
                        default='10', type=int)
    args = parser.parse_args()
    return args


def open_cam_rtsp(uri, width, height, latency):
#    gst_str = ('rtspsrc location={} latency={} ! '
#               'rtph264depay ! h264parse ! omxh264dec ! '
#               'nvvidconv ! '
#               'video/x-raw, width=(int){}, height=(int){}, '
#               'format=(string)BGRx ! '
#               'videoconvert ! appsink').format(uri, latency, width, height)
    gst_str = ('nvarguscamerasrc ! '
               'video/x-raw(memory:NVMM), '
               'width=640, height=480, '
               'format=NV12, framerate=30/1 ! '
               'nvvidconv ! '
               'nvegltransform ! nveglglessink -e')

    return cv2.VideoCapture(gst_str, cv2.CAP_GSTREAMER)


def open_cam_usb(dev, width, height):
    # We want to set width and height here, otherwise we could just do:
    #     return cv2.VideoCapture(dev)
    gst_str = ('v4l2src device=/dev/video{} ! '
               'video/x-raw, width=(int){}, height=(int){} ! '
               'videoconvert ! appsink').format(dev, width, height)
    return cv2.VideoCapture(gst_str, cv2.CAP_GSTREAMER)


def open_cam_onboard(width, height):
    # On versions of L4T prior to 28.1, add 'flip-method=2' into gst_str
    gst_str = ('nvarguscamerasrc ! '
               'video/x-raw(memory:NVMM), '
               'width=640, height=480, '
               'format=NV12, framerate=30/1 ! '
               'nvvidconv ! '
               'video/x-raw, width=(int){}, height=(int){}, '
               'format=(string)BGRx ! '
               'videoconvert ! appsink').format(width, height)

    return cv2.VideoCapture(gst_str, cv2.CAP_GSTREAMER)

def save_cam_onboard(width, height):
    fourcc = cv2.VideoWriter_fourcc(*'DIVX')
    return cv2.VideoWriter(VIDEO_FILE_PATH, fourcc, 25.0, (width, height))


def open_window(width, height):
    cv2.namedWindow(WINDOW_NAME, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(WINDOW_NAME, width, height)
    cv2.moveWindow(WINDOW_NAME, 0, 0)
    cv2.setWindowTitle(WINDOW_NAME, 'Camera Demo for Jetson TX2/TX1')


def read_cam(cap, out):
    show_help = True
    full_scrn = False
    save_rec = False
    # help_text = '"Esc" Proc Exit And Save Vedio File.., "C/c" REC Continue, "P/p" REC Pause, "F" FullScreen, "S" Moment Save Video'
    help_text = '"Esc" Proc Exit And Save Vedio File.., "F" FullScreen, "S" Moment Save Video'
    font = cv2.FONT_HERSHEY_PLAIN

    # Timer setting #
    global VIDEO_REC_TIME
    global VIDEO_REC_TIMER
    VIDEO_REC_TIMER = time.time() + int(VIDEO_REC_TIME)

    while True:
        if cv2.getWindowProperty(WINDOW_NAME, 0) < 0:
            # Check to see if the user has closed the window
            # If yes, terminate the program
            break
        _, img = cap.read() # grab the next image frame from camera
        if show_help:
            cv2.putText(img, help_text, (11, 20), font,
                        1.0, (32, 32, 32), 4, cv2.LINE_AA)
            cv2.putText(img, help_text, (10, 20), font,
                        1.0, (240, 240, 240), 1, cv2.LINE_AA)
        # REC save_rec is True is Rec VIDEO
        # if save_rec:
        #     out.write(img)

        out.write(img)
        cv2.imshow(WINDOW_NAME, img)
        key = cv2.waitKey(10)

        # Timer Check#
        if time.time() > VIDEO_REC_TIMER:
            break

        if key == 27: # ESC key: quit program
            break
        elif key == ord('H') or key == ord('h'): # toggle help message
            show_help = not show_help
        elif key == ord('F') or key == ord('f'): # toggle fullscreen
            full_scrn = not full_scrn
            if full_scrn:
                cv2.setWindowProperty(WINDOW_NAME, cv2.WND_PROP_FULLSCREEN,
                                      cv2.WINDOW_FULLSCREEN)
            else:
                cv2.setWindowProperty(WINDOW_NAME, cv2.WND_PROP_FULLSCREEN,
                                      cv2.WINDOW_NORMAL)
        # elif key == ord('C') or key == ord('c'): # REC Continue
        #     save_rec = true
        #     # save coding
        # elif key == ord('P') or key == ord('p'): # REC Pause
        #     save_rec = false


def getCurrentTime(typeFlag):
    """
    # get time YYMMDDhhmmss_hhmmss
    #   typeFlag is 'D','d' : YYMMDD
    #   typeFlag is 'T','t' : HHMMSS
    #   typeFlag is 'A','a' : YYMMDD_HHMMSS
    """
    now = datetime.now()
    year = now.year
    month = now.month
    hour = now.hour
    min = now.minute
    sec = now.second

    if month < 10:  # month 0->00
        month = "0" + str(month)
    day = now.day
    if day < 10:  # day 0->00
        day = "0" + str(day)
    if hour < 10:  # hour 0->00
        if len(hour) is 1:
            hour = "0" + str(hour)
    if min < 10:  # min 0->00
        if len(min) is 1:
            min = "0" + str(min)
    if sec < 10:  # sec 0->00
        if len(sec) is 1:
            sec = "0" + str(sec)

    y = str(year) + "" + str(month) + "" + str(day)
    d = str(year) + "" + str(month) + "" + str(day)
    t = str(hour) + "" + str(min) + "" + str(sec)
    a = str(year) + "" + str(month) + "" + str(day) + "_" + str(hour) + "" + str(min) + "" + str(sec)

    if typeFlag is 'D' or typeFlag is 'd':
        return d
    elif typeFlag is 'T' or typeFlag is 't':
        return t
    elif typeFlag is 'A' or typeFlag is 'a':
        return a
    else:
        print('TypeFlag is Not Supported!!')


def main():
    args = parse_args()
    print('Called with args:')
    print(args)
    print('OpenCV version: {}'.format(cv2.__version__))

    # REC Time Argument
    recTime = args.rec_time
    global VIDEO_REC_TIME
    VIDEO_REC_TIME = int(recTime)

    # .avi save_path setting
    timePath = getCurrentTime('a')
    if VIDEO_FILE_PATH in "":
        global VIDEO_FILE_PATH
        VIDEO_FILE_PATH = args.save_path + "" + timePath + ".avi"

    print("VIDEO_FILE_PATH :: {}".format(VIDEO_FILE_PATH))

    if args.use_rtsp:
	print("call use_rtsp")
        cap = open_cam_rtsp(args.rtsp_uri,
                            args.image_width,
                            args.image_height,
                            args.rtsp_latency)
    elif args.use_usb:
	print("call use_usb")
        cap = open_cam_usb(args.video_dev,
                           args.image_width,
                           args.image_height)
    else: # by default, use the Jetson onboard camera
	print("call default, use the Jetson onboard camera")
        cap = open_cam_onboard(args.image_width,
                               args.image_height)
        rec = save_cam_onboard(args.image_width,
                               args.image_height)

    if not cap.isOpened():
	print("Failed to open camera")
        sys.exit('Failed to open camera!')

    # Create Window
    open_window(args.image_width, args.image_height)

    # showVideo And REC
    read_cam(cap, rec)

    cap.release()
    rec.release()
    cv2.destroyAllWindows()


# start method
if __name__ == '__main__':
   main()
