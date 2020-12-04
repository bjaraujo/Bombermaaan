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

class BombermaaanEnv(gym.Env):

    def __init__(self):

        startObj = win32process.STARTUPINFO()
        win32process.CreateProcess('C:\\Program Files (x86)\\Bombermaaan\\Bombermaaan.exe', 'Bombermaaan.exe --use-appdata-dir', None, None, 8, 8, None, 'C:\\Program Files (x86)\\Bombermaaan', startObj)

        time.sleep(2)

        x0 = 0
        y0 = 0
        x1 = 0
        y1 = 0

        self.whnd = win32gui.FindWindowEx(None, None, None, 'Bombermaaan 2.0.8.2182')
        if not (self.whnd == 0):
            x0, y0, x1, y1 = win32gui.GetWindowRect(self.whnd)

        self.x0 = x0 + 8
        self.y0 = y0 + 1
        self.x1 = x1 - 8
        self.y1 = y1 - 8

        self.width = self.x1 - self.x0
        self.height = self.y1 - self.y0
        
        self.action_space = spaces.Discrete(6)
        self.observation_space = spaces.Box(low=0, high=255, shape=(self.height, self.width, 3))
               
        self.state = []
        
    def reset(self):
        self.t = 0
        for _ in range(0, 5):
            win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, win32con.VK_RETURN, 0)
        return self.state
        
    def step(self, action):

        print(action)
        
        if (action == 0):
            win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, win32con.VK_UP, 0)
            time.sleep(0.5)
            win32api.PostMessage(self.whnd, win32con.WM_KEYUP, win32con.VK_UP, 0)
        elif (action == 1):
            win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, win32con.VK_DOWN, 0)
            time.sleep(0.5)
            win32api.PostMessage(self.whnd, win32con.WM_KEYUP, win32con.VK_DOWN, 0)
        elif (action == 2):
            win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, win32con.VK_LEFT, 0)
            time.sleep(0.5)
            win32api.PostMessage(self.whnd, win32con.WM_KEYUP, win32con.VK_LEFT, 0)
        elif (action == 3):
            win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, win32con.VK_RIGHT, 0)
            time.sleep(0.5)
            win32api.PostMessage(self.whnd, win32con.WM_KEYUP, win32con.VK_RIGHT, 0)
        elif (action == 4):
            win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, 0x58, 0)
            time.sleep(0.5)
            win32api.PostMessage(self.whnd, win32con.WM_KEYUP, 0x58, 0)
        elif (action == 5):
            win32api.PostMessage(self.whnd, win32con.WM_CHAR, 0x5A, 0)
            time.sleep(0.5)
            win32api.PostMessage(self.whnd, win32con.WM_KEYUP, 0x5A, 0)
            
        self.t = self.t + 0.01        
        ob = self.state
        reward = self.t
        done = False
        return ob, reward, done, {}
                
    def render(self, mode='human', close=False):
        img = ImageGrab.grab(bbox =(self.x0, self.y0, self.x1, self.y1))
        return img
      
    