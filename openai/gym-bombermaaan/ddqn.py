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
from keras.layers import Conv2D, Dense, Flatten
from keras.optimizers import Adam
from keras import backend as K

import tensorflow as tf

class DQNAgent:
    def __init__(self, state_size, action_size):
        self.state_size = state_size
        self.action_size = action_size
        self.memory = deque(maxlen=250)
        self.gamma = 0.9 # discount rate
        self.epsilon = 1.0 # exploration rate
        self.epsilon_min = 0.01
        self.epsilon_decay = 0.98
        self.learning_rate = 0.001
        self.model = self._build_model()
        self.target_model = self._build_model()
        self.update_target_model()

    '''Huber loss for Q Learning
    References: https://en.wikipedia.org/wiki/Huber_loss
                https://www.tensorflow.org/api_docs/python/tf/losses/huber_loss
    '''

    def _huber_loss(self, y_true, y_pred, clip_delta=1.0):
        error = y_true - y_pred
        cond  = K.abs(error) <= clip_delta

        squared_loss = 0.5 * K.square(error)
        quadratic_loss = 0.5 * K.square(clip_delta) + clip_delta * (K.abs(error) - clip_delta)

        return K.mean(tf.where(cond, squared_loss, quadratic_loss))

    def _build_model(self):
        # Neural Net for Deep-Q learning Model
        model = Sequential()
        
        model.add(Conv2D(64, 8, strides=4, activation="relu", input_shape=self.state_size))
        model.add(Conv2D(32, 4, strides=2, activation="relu"))
        model.add(Conv2D(32, 3, strides=1, activation="relu"))
         
        model.add(Flatten())
        
        model.add(Dense(512, activation="relu"))
        model.add(Dense(128, activation="relu"))
        model.add(Dense(32, activation="relu"))
        model.add(Dense(6, activation="linear"))
                
        model.compile(loss=self._huber_loss, optimizer=Adam(lr=self.learning_rate))
        return model

    def update_target_model(self):
        # copy weights from model to target_model
        self.target_model.set_weights(self.model.get_weights())

    def memorize(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def act(self, state):
        if np.random.rand() <= self.epsilon:
            return random.randrange(self.action_size)
        d1 = self.state_size[0]
        d2 = self.state_size[1]
        d3 = self.state_size[2]
        act_values = self.model.predict(state.reshape(1, d1, d2, d3))
        return np.argmax(act_values[0])  # returns action

    def replay(self, batch_size):
        minibatch = random.sample(self.memory, batch_size)
        d1 = self.state_size[0]
        d2 = self.state_size[1]
        d3 = self.state_size[2]
        for state, action, reward, next_state, done in minibatch:
            target = self.model.predict(state.reshape(1, d1, d2, d3))
            if done:
                target[0][action] = reward
            else:
                t = self.target_model.predict(next_state.reshape(1, d1, d2, d3))[0]
                target[0][action] = reward + self.gamma * np.amax(t)
                        
            self.model.fit(state.reshape(1, d1, d2, d3), target, epochs=1, verbose=0)
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
    
    batch_size = 64
    done = False
    
    num_episodes = 500
    steps = []
    
    plt.ion()
    plt.show()
    plt.xlabel('Episode')
    plt.ylabel('Score')
    
    data = []
    data.append(0)
    
    for e in range(num_episodes):
        state = env.reset()
        score = 0.0
        for time in range(250):
            #env.render()
            action = agent.act(state)
            next_state, reward, done, _ = env.step(action)
            score += reward
            steps.append((state, action, reward, next_state, done))            
            state = next_state
            if done:
                print('episode: {}/{}, score: {}, e: {:.2}'.format(e + 1, num_episodes, score, agent.epsilon))
                
                if env.victory:
                    print('Victory!')
                                        
                data.append(score)

                if e < 20:
                    plt.xticks(range(0, e + 2, 1))
                elif e < 200:
                    plt.xticks(range(0, e + 2, 10))
                elif e < 2000:
                    plt.xticks(range(0, e + 2, 100))
                
                plt.plot(data)
                plt.draw()
                plt.pause(0.01)
    
                env.pause()

                steps.reverse()
                for i in range(len(steps)):
                    state, action, reward, next_state, done = steps[i]
                    
                    if not env.victory and reward > 0.0 and i < 5:
                        # discount reward 
                        reward = -1
                        
                    agent.memorize(state, action, reward, next_state, done)
                
                agent.update_target_model()

                if len(agent.memory) > batch_size:
                    agent.replay(batch_size)

                break
        
        if e % 10 == 0:
            agent.save('bombermaaan.h5')
    
    agent.save('bombermaaan.h5')
    