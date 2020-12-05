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

BOMBERMAAAN_PATH = 'D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187\\Bombermaaan.exe'
BOMBERMAAAN_COMMAND = 'Bombermaaan.exe --use-appdata-dir'
BOMBERMAAAN_WORKDIR = 'D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187'

BOMBER_HEAD_X = 79
BOMBER_HEAD_Y = 30 + 5
BOMBER_HEAD_W = 14
BOMBER_HEAD_H = 14
BOMBER_OFFSET_X = 35

BOMBERS = 5

class BombermaaanEnv(gym.Env):

    def __init__(self):

        self.done = False
        self.victory = False            
        self.score = 0
        self.state = []

    def get_bombermaaan_window_title(self):
        def callback(handle, data):
            text = win32gui.GetWindowText(handle)
            if text.startswith('Bombermaaan'):
                titles.append(text)

        titles = []
        win32gui.EnumWindows(callback, None)
        return titles[0]
    
    def start(self, path, exe, args):
        startObj = win32process.STARTUPINFO()
        win32process.CreateProcess(path + '\\' + exe, exe + ' ' + args, None, None, 8, 8, None, path, startObj)
    
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

    def press(self, key):
        win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, key, 0)
        time.sleep(0.5)
        win32api.PostMessage(self.whnd, win32con.WM_KEYUP, key, 0)
    
    def reset(self):

        if self.done:
            if self.victory:
                self.press(win32con.VK_RETURN)
                self.press(win32con.VK_RETURN)
            self.press(win32con.VK_ESCAPE)
            self.press(win32con.VK_RETURN)
            
        self.victory = False
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

        if not self.victory:
            alive = 0
            for i in range(0, BOMBERS):
                img = ImageGrab.grab(self.head_bbox[i])
                img.save('head' + str(i) + '.png')
                
                is_dead = (img != self.bomber_head[i])                
                if not is_dead:
                    alive = alive + 1                

                self.bomber_dead[i] = is_dead
        
            # Check for win
            if not self.bomber_dead[0] and (alive == 1):
                self.victory = True
                self.score = self.score + 10.0
            
            if self.bomber_dead[0]:
                self.score = self.score - 5.0
                
        self.done = self.victory or self.bomber_dead[0]

        if self.done:
            time.sleep(2)
                    
        return ob, self.score, self.done, {}
                
    def render(self, mode='human', close=False):
        img = ImageGrab.grab(bbox =(self.x0, self.y0, self.x1, self.y1))
        return img
   
        