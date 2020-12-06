import os
import random
import gym
import gym_bombermaaan
import numpy as np

from keras.models import Sequential, load_model
from keras.layers import Flatten, Dense, Dropout

def gather_data(env):
    
    num_trials = 10
    sim_steps = 500
    min_score = 0.0
    trainingX, trainingY = [], []

    score_file_name = 'score.dat'
    if os.path.exists(score_file_name):
        with open(score_file_name, 'r') as file:
            text = file.readline()
            min_score = int(text)

    model = None
    model_file_name = 'bombermaaan.h5'
    file_exists = os.path.exists(model_file_name)
    if file_exists:
        model = load_model(model_file_name)

    scores = []
    for trial in range(num_trials):
        print('Trial: {}'.format(trial))
        observation = env.reset()
        training_sampleX, training_sampleY = [], []
        score = 0
        for _ in range(sim_steps):
            
            # action corresponds to the previous observation so record before step
            if (random.random() > 0.5 and file_exists):
                action = np.argmax(model.predict(observation.reshape(1, env.height, env.width, 3)))
            else:
                action = env.action_space.sample()
                
            one_hot_action = np.zeros(6)
            one_hot_action[action] = 1
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

    trainingX, trainingY = np.array(trainingX), np.array(trainingY)
    
    average_score = np.mean(scores)
    median_score = np.median(scores)

    with open(score_file_name, 'w') as the_file:
        the_file.write(str(int(average_score)) + '\n')
    
    print('Average: {}'.format(average_score))
    print('Median: {}'.format(median_score))
    
    return trainingX, trainingY

def create_model(env):
    model = Sequential()
    model.add(Dense(32, input_shape=(env.height, env.width, 3), activation="relu"))
    model.add(Flatten())
    model.add(Dense(16, activation="relu"))
    model.add(Dense(6, activation="linear"))

    model.compile(
        loss='mse',
        optimizer='adam',
        metrics=['accuracy'])
    
    return model

def main():
    env = gym.make('bombermaaan-v0')

    env.start('D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187', 'Bombermaaan.exe', '')
    trainingX, trainingY = gather_data(env)   

    env.pause()

    model = create_model(env)
    model.fit(trainingX, trainingY, epochs=5)
    model.save('bombermaaan.h5')

    env.pause()

    scores = []
    num_trials = 50
    sim_steps = 500
    for trial in range(num_trials):
        print('Trial: {}'.format(trial))
        observation = env.reset()
        score = 0
        for step in range(sim_steps):
            print('Step: {}'.format(step))
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