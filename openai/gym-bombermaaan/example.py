import os
import random
import gym
import gym_bombermaaan
import numpy as np

from keras.models import Sequential, load_model
from keras.layers import Conv2D, Dense, Flatten, Dropout
from keras import backend as backend

def gather_data(env):
    
    num_trials = 2
    sim_steps = 500
    min_score = 0
    trainingX, trainingY = [], []

    if os.path.exists('score.dat'):
        with open('score.dat', 'r') as file:
            text = file.readline()
            min_score = int(text)
            
    model = None
    if os.path.exists('bombermaaan.h5'):
        model = load_model('bombermaaan.h5')

    scores = []
    for trial in range(num_trials):
        print('========> Trial: {}'.format(trial + 1))
        observation = env.reset()
        reward = 0
        done = False
        training_sampleX, training_sampleY = [], []        
        score = 0        
        for _ in range(sim_steps):            
            # Action corresponds to the previous observation so record before step
            if (model and random.random() > 0.25):
                print('-- Neural network --')
                action = np.argmax(model.predict(observation.reshape(1, env.height, env.width, 3)))
            else:
                print('-- Random --')
                if random.random() > 0.9:
                    action = random.randint(4, 5)
                else:
                    action = random.randint(0, 3)
                      
            print('Action: {}'.format(action))

            one_hot_action = np.zeros(6)
            one_hot_action[action] = reward
            training_sampleX.append(observation)
            training_sampleY.append(one_hot_action)
            
            observation, reward, done, _ = env.step(action)
            
            score += reward
            
            print('Score = {}'.format(score))
            
            if done:
                break
                
        if score > min_score:
            scores.append(score)
            trainingX += training_sampleX
            trainingY += training_sampleY

    with open('score.dat', 'w') as file:
        file.write(str(min_score + 10) + '\n')

    trainingX, trainingY = np.array(trainingX), np.array(trainingY)
    
    average_score = np.mean(scores)
    median_score = np.median(scores)
    
    print('Average: {}'.format(average_score))
    print('Median: {}'.format(median_score))
            
    return trainingX, trainingY

def create_model(env):

    model = Sequential()
    model.add(Conv2D(256, (8, 8), strides=(4, 4), input_shape=(env.height, env.width, 3), activation="relu"))
    model.add(Conv2D(128, (4, 4), strides=(2, 2), input_shape=(env.height, env.width, 3), activation="relu"))
    model.add(Flatten())
    model.add(Dense(256, activation="relu"))
    model.add(Dense(32, activation="relu"))
    model.add(Dense(6, activation="linear"))
    model.compile(loss='mse', optimizer='adam')
    
    return model

def main():
    env = gym.make('bombermaaan-v0')

    env.start('D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187', 'Bombermaaan.exe', '')
    trainingX, trainingY = gather_data(env)   

    # Clear keras memory?
    #backend.clear_session()

    env.pause()

    model = create_model(env)
    model.fit(trainingX, trainingY, epochs=5)
    model.save('bombermaaan.h5')

    env.pause()

    scores = []
    num_trials = 10
    sim_steps = 500
    for trial in range(num_trials):
        print('========> Trial: {}'.format(trial + 1))
        observation = env.reset()
        score = 0
        for _ in range(sim_steps):
            print('-- Neural network --')
            action = np.argmax(model.predict(observation.reshape(1, env.height, env.width, 3)))
            print('Action: {}'.format(action))
            observation, reward, done, _ = env.step(action)
            
            score += reward
            
            print('Score = {}'.format(score))
            
            if done:
                break
        scores.append(score)
    env.end()

    env.close()

    average_score = np.mean(scores)
    print('Average: {}'.format(average_score))

if __name__ == "__main__":
    main()