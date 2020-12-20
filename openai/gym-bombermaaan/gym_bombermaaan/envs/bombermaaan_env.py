import os
import win32process
import win32ui
import win32gui
import win32api
import win32con
import time
import gym
import numpy as np
from gym import spaces
from PIL import Image, ImageOps

import logging
logger = logging.getLogger(__name__)

BOMBER_ICON_X = 79
BOMBER_ICON_Y = 30 + 5
BOMBER_ICON_W = 14
BOMBER_ICON_H = 14
BOMBER_OFFSET_X = 35

WHITE_BOMBER_RGB = (255, 248, 255)
BLACK_BOMBER_RGB = (2, 0, 1)
RED_BOMBER_RGB = (255, 0, 0)
BLUE_BOMBER_RGB = (0, 72, 255)
GREEN_BOMBER_RGB = (79, 183, 0)

MAX_BOMBERS = 5

class BombermaaanEnv(gym.Env):

    def __init__(self):

        self.done = False
        self.victory = False
        self.draw = False
        self.state = None

    def get_bombermaaan_window(self):
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
    
        time.sleep(3)

        self.hwnd = win32gui.FindWindowEx(None, None, None, self.get_bombermaaan_window())
        x0, y0, x1, y1 = win32gui.GetWindowRect(self.hwnd)

        x0 += 8
        y0 += 1
        x1 -= 8
        y1 -= 8
        
        self.window_area = (0, 0, x1 - x0, y1 - y0)
        self.score_area = (0, 30, x1 - x0, 56)        
        self.play_area = (0, 56, x1 - x0, y1 - y0)

        self.width = x1 - x0
        self.height = y1 - y0
                
        self.action_space = spaces.Discrete(6)
        self.observation_space = spaces.Box(low=0, high=255, shape=(self.height, self.width, 3), dtype=np.uint8)
                
        self.head_bbox = []
        for i in range(0, MAX_BOMBERS):
            b = (BOMBER_ICON_X + BOMBER_OFFSET_X * i, \
                 BOMBER_ICON_Y, \
                 BOMBER_ICON_X + BOMBER_OFFSET_X * i + BOMBER_ICON_W, \
                 BOMBER_ICON_Y + BOMBER_ICON_H)
            self.head_bbox.append(b)

    def grab_screenshot(self, box): 
    
        x0, y0, x1, y1 = win32gui.GetWindowRect(self.hwnd)
        x0 += 8
        y0 += 1
        x1 -= 8
        y1 -= 8
        w = x1 - x0
        h = y1 - y0
        wDC = win32gui.GetWindowDC(self.hwnd)
        dcObj = win32ui.CreateDCFromHandle(wDC)
        cDC = dcObj.CreateCompatibleDC()
        dataBitMap = win32ui.CreateBitmap()
        dataBitMap.CreateCompatibleBitmap(dcObj, w, h)
        cDC.SelectObject(dataBitMap)
        cDC.BitBlt((-8, -1), (w + 8, h + 1), dcObj, (0, 0), win32con.SRCCOPY)        
        bmpinfo = dataBitMap.GetInfo()
        bmpstr = dataBitMap.GetBitmapBits(True)
        
        img = Image.frombuffer('RGB', (bmpinfo['bmWidth'], bmpinfo['bmHeight']), bmpstr, 'raw', 'BGRX', 0, 1)    
        img = img.crop(box)
        
        # Free Resources
        dcObj.DeleteDC()
        cDC.DeleteDC()
        win32gui.ReleaseDC(self.hwnd, wDC)
        win32gui.DeleteObject(dataBitMap.GetHandle())
    
        return img
        
    def press(self, key, dt=0.5):
        win32api.SendMessage(self.hwnd, win32con.WM_KEYDOWN, key, 0)
        time.sleep(dt)
        win32api.SendMessage(self.hwnd, win32con.WM_KEYUP, key, 0)
    
    def output_bombers(self):
        for bomber in self.bombers:
            print(bomber)
    
    def reset(self):

        if self.done:
            if self.victory or self.draw:
                self.press(win32con.VK_RETURN, 0.1)
                time.sleep(0.5)
                self.press(win32con.VK_RETURN, 0.1)
                time.sleep(0.5)
            
            self.press(win32con.VK_ESCAPE, 0.1)
            time.sleep(0.5)
            self.press(win32con.VK_RETURN, 0.1)
            time.sleep(0.5)
            
        for _ in range(5):
            self.press(win32con.VK_RETURN, 0.5)

        while True:
            img = self.grab_screenshot(self.score_area)            
            r, g, b = img.getpixel((60, 8))

            if r == 132 and g == 132 and b == 0:
                break
            
            time.sleep(0.25)
        
        img = self.grab_screenshot(self.window_area)
        
        self.bombers = []
        for i in range(0, MAX_BOMBERS):
            icon = img.crop(self.head_bbox[i])
            rgb = icon.getpixel((5, 5))
            
            color = None
            if rgb == WHITE_BOMBER_RGB:
                color = 'white'
            if rgb == BLACK_BOMBER_RGB:
                color = 'black'
            if rgb == RED_BOMBER_RGB:
                color = 'red'
            if rgb == BLUE_BOMBER_RGB:
                color = 'blue'
            if rgb == GREEN_BOMBER_RGB:
                color = 'green'
        
            if color:
                self.bombers.append({'color': color, 'is_dead': False, 'icon': icon})
        
        #self.output_bombers()
        
        self.done = False
        self.victory = False
        self.draw = False
        
        state = np.array(self.grab_screenshot(self.play_area))
                
        return state
    
    def pause(self):
        self.press(0x50)

    def step(self, action):
        
        reward = 1.0
        
        if (action == 0):
            # Do nothing
            self.press(0x0) # dummy key
        elif (action == 1):
            # Go up
            self.press(win32con.VK_UP)
        elif (action == 2):
            # Go down
            self.press(win32con.VK_DOWN)
        elif (action == 3):
            # Go left
            self.press(win32con.VK_LEFT)
        elif (action == 4):
            # Go right
            self.press(win32con.VK_RIGHT)
        elif (action == 5):
            # Place bomb
            self.press(0x58)           
        elif (action == 6):
            # Detonate bomb
            self.press(0x5A)
                               
        state = np.array(self.grab_screenshot(self.play_area))
        
        if not self.done:
            if not self.victory and not self.draw:
                
                i = 0
                alive = 0
                
                img = self.grab_screenshot(self.window_area)
                
                for bomber in self.bombers:
                    icon = img.crop(self.head_bbox[i])
                    
                    if not bomber['is_dead']:
                        is_dead = (icon != bomber['icon'])  
                        
                        if is_dead and i > 0:
                            reward += 2.0
                        
                        bomber['is_dead'] = is_dead
                        
                    if not bomber['is_dead']:
                        alive = alive + 1
                    
                    i = i + 1
                    
                #self.output_bombers()
                
                # Check for death or win                                    
                if self.bombers[0]['is_dead']:
                    reward -= 5.0
                elif alive == 1:
                    self.victory = True
                    reward += 10.0
                
                if alive == 0:
                    self.draw = True
                    reward += 5.0
                    
        self.done = self.victory or self.draw or self.bombers[0]['is_dead']
                        
        return state, reward, self.done, {}
                
    def render(self, mode='human', close=False):
        
        img = None
        
        if mode == 'human':
            img = self.grab_screenshot(self.play_area)
        
        return img
