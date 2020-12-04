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

        self.whnd = win32gui.FindWindowEx(None, None, None, 'Bombermaaan 2.0.8.2182')
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
           
    def press(self, key):
        win32api.PostMessage(self.whnd, win32con.WM_KEYDOWN, key, 0)
        time.sleep(0.5)
        win32api.PostMessage(self.whnd, win32con.WM_KEYUP, key, 0)
    
    def reset(self):
        self.t = 0

        for _ in range(0, 5):
            self.press(win32con.VK_RETURN)
            
        time.sleep(2)
            
        self.alive_img = ImageGrab.grab(bbox =(self.x0 + 76, self.y0 + 32, self.x0 + 95, self.y0 + 52))
            
        return self.state
        
    def step(self, action):

        print(action)
        
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
            
        self.t = self.t + 0.01        
        ob = self.state
        reward = self.t       
        
        img = ImageGrab.grab(bbox =(self.x0 + 76, self.y0 + 32, self.x0 + 95, self.y0 + 52))
        done = img != self.alive_img
        
        if done:
            self.press(win32con.VK_ESCAPE)
            self.press(win32con.VK_RETURN)
            self.reset()
            
        print(done)
        
        return ob, reward, done, {}
                
    def render(self, mode='human', close=False):
        img = ImageGrab.grab(bbox =(self.x0, self.y0, self.x1, self.y1))
        return img
      
    