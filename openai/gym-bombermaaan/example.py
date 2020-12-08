import os
import random
import gym
import gym_bombermaaan
import numpy as np

from keras.models import Sequential, load_model
from keras.layers import Conv2D, Dense, Flatten, Dropout
from keras import backend as backend

def create_model(env):

    model = Sequential()
    
    model.add(Conv2D(32, 8, strides=4, activation="relu", input_shape=(env.height, env.width, 3)))
    model.add(Conv2D(64, 4, strides=2, activation="relu"))
    model.add(Conv2D(64, 3, strides=1, activation="relu"))
     
    model.add(Flatten())
    
    model.add(Dense(512, activation="relu"))
    model.add(Dense(6, activation="linear"))

    model.compile(loss='mse', optimizer='adam')
    
    return model

def train(env, model, eps):
    
    num_trials = 20
    sim_steps = 500
    n = 0

    training_x = []
    training_y = []

    model = None

    if os.path.exists('bombermaaan.h5'):
        model = load_model('bombermaaan.h5')

    min_score = 0.0
    
    if os.path.exists('score.dat'):
        with open('score.dat', 'r') as file:
            text = file.readline()
            min_score = float(text)
        
    for trial in range(num_trials):
        print('========> Trial: {}'.format(trial + 1))
        
        observation = env.reset()
        score = 0.0          
        reward = 0.0
        sample_x = []
        sample_y = []
        done = False
        
        for _ in range(sim_steps):            
            # Action corresponds to the previous observation so record before step
            if (model and random.random() > (1.0 - eps)):
                print('-- Neural network --')
                action = np.argmax(model.predict(observation.reshape(1, env.height, env.width, 3)))
            else:
                print('-- Random --')
                if random.random() > 0.98:
                    action = random.randint(4, 5)
                else:
                    action = random.randint(0, 3)
            
            print('Action: {}'.format(action))

            sample_x.append(observation)
            
            observation, reward, done, _ = env.step(action)

            one_hot_action = np.zeros(6)            
            one_hot_action[action] = reward

            # If reward is negative takeaway previous reward as well
            if reward < 0:
                i = np.argmax(sample_y[-1])
                sample_y[-1][i] = -1.0
                    
            sample_y.append(one_hot_action)           
            
            score += reward
            
            print('Score = {:.1f}'.format(score))
            
            if done:
                break
            
        if score > min_score:
            min_score = score
            training_x += sample_x
            training_y += sample_y
            n = n + 1
             
    env.pause()
    
    if n > 0:
        training_x = np.array(training_x)
        training_y = np.array(training_y)

        if not model:
            model = create_model(env)

        model.fit(training_x, training_y, epochs=5)
        model.save('bombermaaan.h5')

    env.pause()          

def main():

    env = gym.make('bombermaaan-v0')
    env.start('D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187', 'Bombermaaan.exe', '')

    model = create_model(env)
    train(env, model, 0.95)

    env.close()

if __name__ == "__main__":
    main()