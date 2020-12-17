# -*- coding: utf-8 -*-
#!/usr/bin/python3

import os
import time
import random
import gym
import gym_bombermaaan
import numpy as np
import matplotlib.pyplot as plt
from collections import deque
from keras.models import Sequential
from keras.layers import Conv2D, Dense, Dropout, Flatten
from keras.optimizers import Adam
from keras import backend as K

class DQNAgent:
    def __init__(self, state_size, action_size):
        self.state_size = state_size
        self.action_size = action_size
        self.memory = deque(maxlen=1024)
        self.gamma = 0.9 # discount rate
        self.epsilon = 1.0 # exploration rate
        self.epsilon_min = 0.01
        self.epsilon_decay = 0.96 # decay rate
        self.model = self._build_model()

    def _build_model(self):
        # Neural Net for Deep-Q learning Model
        model = Sequential()
        
        model.add(Conv2D(256, 3, strides=1, activation='relu', input_shape=self.state_size))
        model.add(Conv2D(128, 3, strides=2, activation='relu'))
        model.add(Conv2D(64, 3, strides=3, activation='relu'))
        model.add(Flatten())
        model.add(Dense(512, activation='relu'))
        model.add(Dense(128, activation='relu'))        
        model.add(Dense(32, activation='relu'))
        model.add(Dense(6, activation='linear'))
                
        model.compile(loss='mse', optimizer='adam')
        
        return model

    def random_act(self):
        if np.random.rand() < 0.95:
            action = np.random.randint(0, 5)
        else:
            action = 5

        return action
        
    def act(self, state):
        action = 0
        
        if np.random.rand() < self.epsilon:
            action = self.random_act()
        
        else:
            d1 = self.state_size[0]
            d2 = self.state_size[1]
            d3 = self.state_size[2]
            action_values = self.model.predict(state.reshape(1, d1, d2, d3))
            action = np.argmax(action_values[0])  # returns action
            
            print(action_values)
            
            if (action == 0):
                print('Do nothing!')
            elif (action == 1):
                print('Go up!')
            elif (action == 2):
                print('Go down!')
            elif (action == 3):
                print('Go left!')
            elif (action == 4):
                print('Go right!')
            elif (action == 5):
                print('Place bomb!')
            elif (action == 6):
                print('Detonate bomb!')
            
        return action

    def memorize(self, state, action, reward):
        self.memory.append((state, action, reward))

    def train(self):
        
        states = []
        actions = []
        
        for state, action, reward in self.memory:
            action_values = np.zeros(6)            
            action_values[action] = reward
            states.append(state)
            actions.append(action_values)
            
        states = np.array(states)
        actions = np.array(actions)
    
        self.model.fit(states, actions, epochs=1, verbose=1)
        
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

    def load(self, name):
        self.model.load_weights(name)

    def save(self, name):
        self.model.save_weights(name)


if __name__ == '__main__':
    env = gym.make('bombermaaan-v0')
    env.start('E:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2192', 'Bombermaaan.exe', '')
    
    state_size = env.observation_space.shape    
    action_size = env.action_space.n
    agent = DQNAgent(state_size, action_size)
    
    if os.path.exists('bombermaaan.h5'):
        agent.load('bombermaaan.h5')
        agent.epsilon = 0.5
    
    done = False
    
    num_episodes = 1000
    
    plt.ion()
    plt.show()
    plt.xlabel('Episode')
    plt.ylabel('Score')
    
    score = 0
    scores = []    
    scores.append(score)
        
    for e in range(num_episodes):
        
        state = env.reset()
        start = time.time()
        
        history = []
        
        for t in range(1000):
            
            #img = env.render(mode = 'ai')
            #img.save('play' + str(t) + '.png')
            
            if e % 5 == 0:
                action = agent.random_act()
            else:
                action = agent.act(state)
            
            next_state, reward, done, _ = env.step(action)  

            history.append((state, action, reward))
                        
            state = next_state

            if done:

                end = time.time()
                
                score = end - start
                
                if env.victory:
                    print('Victory!')
                    score += 100.0
                elif env.draw:
                    print('Draw!')
                    score += 50.0

                scores.append(score)
                
                env.pause()
                
                if e < 20:
                    plt.xticks(range(0, e + 2, 1))
                elif e < 200:
                    plt.xticks(range(0, e + 2, 10))
                elif e < 2000:
                    plt.xticks(range(0, e + 2, 100))
                
                plt.plot(scores)
                plt.draw()
                plt.pause(0.01)

                print('episode: {}/{}, score: {:.1f}, t: {}, e: {:.2f}'.format(e + 1, num_episodes, score, t, agent.epsilon))

                if score > np.mean(scores) or len(scores) < 10:
                    for state, action, reward in history:                    
                        agent.memorize(state, action, reward)                

                    agent.train()
                    
                break
        
        if (e + 1) % 10 == 0:
            agent.save('bombermaaan.h5')
    