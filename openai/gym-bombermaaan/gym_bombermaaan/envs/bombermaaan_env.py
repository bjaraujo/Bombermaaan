import os
import win32process
import win32gui
import win32api
import win32con
import time
import gym
import numpy as np
from gym import spaces
from PIL import Image, ImageGrab 

import logging
logger = logging.getLogger(__name__)

BOMBERMAAAN_PATH = 'D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187\\Bombermaaan.exe'
BOMBERMAAAN_COMMAND = 'Bombermaaan.exe --use-appdata-dir'
BOMBERMAAAN_WORKDIR = 'D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187'

BOMBER_ICON_X = 79
BOMBER_ICON_Y = 30 + 5
BOMBER_ICON_W = 14
BOMBER_ICON_H = 14
BOMBER_OFFSET_X = 35

BOMBERS = 5

class BombermaaanEnv(gym.Env):

    def __init__(self):

        self.done = False
        self.victory = False            
        self.score = 0
        self.state = None

    def get_bombermaaan_window_title(self):
        def callback(handle, data):
            text = win32gui.GetWindowText(handle)
            if text.startswith('Bombermaaan'):
                titles.append(text)

        titles = []
        win32gui.EnumWindows(callback, None)
        return titles[0]
    
    def start(self, path, exe, args):
        self.processInfo = win32process.STARTUPINFO()
        win32process.CreateProcess(os.path.join(path, exe), exe + ' ' + args, None, None, 8, 8, None, path, self.processInfo)
    
        time.sleep(2)

        self.whnd = win32gui.FindWindowEx(None, None, None, self.get_bombermaaan_window_title())
        self.x0, self.y0, self.x1, self.y1 = win32gui.GetWindowRect(self.whnd)

        self.x0 += 8
        self.y0 += 1
        self.x1 -= 8
        self.y1 -= 8

        self.width = self.x1 - self.x0
        self.height = self.y1 - self.y0
        
        self.action_space = spaces.Discrete(6)
        self.observation_space = spaces.Box(low=0, high=255, shape=(self.height, self.width, 3), dtype=np.uint8)
        
        self.window_box = (self.x0, self.y0, self.x1, self.y1)
        
        self.head_bbox = []
        for i in range(0, BOMBERS):
            b = (self.x0 + BOMBER_ICON_X + BOMBER_OFFSET_X * i, \
                 self.y0 + BOMBER_ICON_Y, \
                 self.x0 + BOMBER_ICON_X + BOMBER_OFFSET_X * i + BOMBER_ICON_W, \
                 self.y0 + BOMBER_ICON_Y + BOMBER_ICON_H)
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
            
        self.done = False
        self.victory = False

        for _ in range(5):
            self.press(win32con.VK_RETURN)
            
        time.sleep(2)
            
        self.bomber_icon = []
        self.is_bomber_dead = []
        for i in range(0, BOMBERS):
            img = ImageGrab.grab(self.head_bbox[i])
            self.bomber_icon.append(img)
            self.is_bomber_dead.append(False)
        
        state = np.array(ImageGrab.grab(self.window_box))
        
        return state
    
    def pause(self):
        self.press(0x50)

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
            
        reward = 0.01
        state = np.array(ImageGrab.grab(self.window_box))

        if not self.done:
            if not self.victory:
                alive = 0
                for i in range(0, BOMBERS):
                    img = ImageGrab.grab(self.head_bbox[i])
                    
                    is_dead = (img != self.bomber_icon[i])                
                    if not is_dead:
                        alive = alive + 1                

                    self.is_bomber_dead[i] = is_dead
            
                # Check for win
                if not self.is_bomber_dead[0] and (alive == 1):
                    self.victory = True
                
                if self.is_bomber_dead[0]:
                    reward = -0.05
                    
        self.done = self.victory or self.is_bomber_dead[0]

        if self.victory:
            reward = 1.0
            
        if self.done:
            time.sleep(2)
                                    
        return state, reward, self.done, {}
                
    def render(self, mode='human', close=False):
        img = ImageGrab.grab(self.window_box)
        return img
   
        