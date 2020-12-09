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

def train(env, model):
    
    model = None

    if os.path.exists('bombermaaan.h5'):
        model = load_model('bombermaaan.h5')

    n = 5
    scores = [0.0] * n
    samples_x = [None] * n
    samples_y = [None] * n
    
    eps = 1.0
    
    for trial in range(20):
        print('========> Trial: {}'.format(trial + 1))
                
        reward = 0.0
        sample_x = []
        sample_y = []
        cur_score = 0.0
        last_action = 0
        done = False
        
        observation = env.reset()
                    
        for _ in range(500):            
            # Action corresponds to the previous observation so record before step
            if (model and random.random() > eps):
                print('-- Neural network --')
                action = np.argmax(model.predict(observation.reshape(1, env.height, env.width, 3)))
            else:
                print('-- Random --')
                if random.random() > 0.98:
                    action = random.randint(4, 5)
                else:
                    action = random.randint(0, 3)
            
            # Override these actions
            if last_action == 4 and action >= 4:
                action = random.randint(0, 3)
                
            last_action = action

            print('Action: {}'.format(action))
            
            sample_x.append(observation)
            
            observation, reward, done, _ = env.step(action)

            one_hot_action = np.zeros(6)            
            one_hot_action[action] = reward

            # If reward is negative takeaway previous reward as well
            if reward < 0 and len(sample_y) > 0:
                i = np.argmax(sample_y[-1])
                sample_y[-1][i] = -1.0
                    
            sample_y.append(one_hot_action)           
            
            cur_score += reward
            
            if env.victory:
                cur_score += 50.0
            
            print('Current score = {:.1f}'.format(cur_score))
            
            if done:
                break
                    
        if eps > 0.0:
            eps -= 0.2
        
        i = np.argmin(scores)
        
        scores[i] = cur_score
        samples_x[i] = sample_x
        samples_y[i] = sample_y
    
    print('Highest scores')
    print(scores)
    
    env.pause()
    
    training_x = []
    for i in range(0, len(samples_x)):
        if samples_x[i]:
            training_x += samples_x[i]

    training_y = []
    for i in range(0, len(samples_y)):
        if samples_y[i]:
            training_y += samples_y[i]
   
    training_x = np.array(training_x)
    training_y = np.array(training_y)

    if not model:
        model = create_model(env)

    model.fit(training_x, training_y, epochs=6)
    model.save('bombermaaan.h5')

    env.pause()          

def main():

    env = gym.make('bombermaaan-v0')
    env.start('F:\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187', 'Bombermaaan.exe', '')

    model = create_model(env)
    train(env, model)

    env.close()

if __name__ == "__main__":
    main()