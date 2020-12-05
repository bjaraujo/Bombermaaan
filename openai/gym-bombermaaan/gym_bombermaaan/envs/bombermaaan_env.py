import win32process
import win32gui
import win32api
import win32con
import time
import gym
from gym import spaces
from PIL import Image, ImageGrab 

import logging
logger = logging.getLogger(__name__)

BOMBERMAAAN_PATH = 'C:\\Program Files (x86)\\Bombermaaan\\Bombermaaan.exe'
BOMBERMAAAN_COMMAND = 'Bombermaaan.exe --use-appdata-dir'
BOMBERMAAAN_WORKDIR = 'C:\\Program Files (x86)\\Bombermaaan'

BOMBER_HEAD_X = 79
BOMBER_HEAD_Y = 30 + 5
BOMBER_HEAD_W = 14
BOMBER_HEAD_H = 14
BOMBER_OFFSET_X = 35

BOMBERS = 5

class BombermaaanEnv(gym.Env):

    def __init__(self):

        startObj = win32process.STARTUPINFO()
        win32process.CreateProcess(BOMBERMAAAN_PATH, BOMBERMAAAN_COMMAND, None, None, 8, 8, None, BOMBERMAAAN_WORKDIR, startObj)

        time.sleep(2)

        self.whnd = win32gui.FindWindowEx(None, None, None, self.get_bombermaaan_window_title())
        self.x0, self.y0, self.x1, self.y1 = win32gui.GetWindowRect(self.whnd)

        self.x0 = self.x0 + 8
        self.y0 = self.y0 + 1
        self.x1 = self.x1 - 8
        self.y1 = self.y1 - 8

        self.width = self.x1 - self.x0
        self.height = self.y1 - self.y0
        
        self.action_space = spaces.Discrete(6)
        self.observation_space = spaces.Box(low=0, high=255, shape=(self.height, self.width, 3))
        
        self.head_bbox = []
        for i in range(0, BOMBERS):
            b = bbox =(self.x0 + BOMBER_HEAD_X + BOMBER_OFFSET_X * i, \
                       self.y0 + BOMBER_HEAD_Y, \
                       self.x0 + BOMBER_HEAD_X + BOMBER_OFFSET_X * i + BOMBER_HEAD_W, \
                       self.y0 + BOMBER_HEAD_Y + BOMBER_HEAD_H)
            self.head_bbox.append(b)

        #print(self.head_bbox)
            
        self.state = []

    def get_bombermaaan_window_title(self):
        def callback(handle, data):
            text = win32gui.GetWindowText(handle)
            if text.startswith('Bombermaaan'):
                titles.append(text)

        titles = []
        win32gui.EnumWindows(callback, None)
        return titles[0]
        
    def press(self, key):
        win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, key, 0)
        time.sleep(0.5)
        win32api.PostMessage(self.whnd, win32con.WM_KEYUP, key, 0)
    
    def reset(self):
        self.score = 0

        for _ in range(5):
            self.press(win32con.VK_RETURN)
            
        time.sleep(2)
            
        self.bomber_head = []
        self.bomber_dead = []
        for i in range(0, BOMBERS):
            img = ImageGrab.grab(self.head_bbox[i])
            self.bomber_head.append(img)
            self.bomber_dead.append(False)
            #img.save('head' + str(i) + '.png')
            
        return self.state
        
    def step(self, action):
        
        if (action == 0):
            self.press(win32con.VK_UP)
        elif (action == 1):
            self.press(win32con.VK_DOWN)
        elif (action == 2):
            self.press(win32con.VK_LEFT)
        elif (action == 3):
            self.press(win32con.VK_RIGHT)
        elif (action == 4):
            self.press(0x58)
        elif (action == 5):
            self.press(0x5A)
            
        self.score = self.score + 0.01        
        ob = self.state

        for i in range(0, BOMBERS):
            img = ImageGrab.grab(self.head_bbox[i])
            self.bomber_dead[i] = (img != self.bomber_head[i])
            if self.bomber_dead[i]:
                if i > 0:
                    self.score = self.score + 0.1
                else:
                    self.score = self.score - 0.5
            
        # White bomber dead?
        done = self.bomber_dead[0]

        reward = self.score      
                    
        return ob, reward, done, {}
                
    def render(self, mode='human', close=False):
        img = ImageGrab.grab(bbox =(self.x0, self.y0, self.x1, self.y1))
        return img
    
    def exit(self):
        self.press(win32con.VK_ESCAPE)
        self.press(win32con.VK_RETURN)
        