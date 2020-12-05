import gym
import gym_bombermaaan
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout

def gather_data(env):
    num_trials = 1
    sim_steps = 500
    min_score = 0.001
    trainingX, trainingY = [], []

    scores = []
    for trial in range(num_trials):
        print('Trial: {}'.format(trial))
        observation = env.reset()
        training_sampleX, training_sampleY = [], []
        score = 0
        for _ in range(sim_steps):
            # action corresponds to the previous observation so record before step
            action = env.action_space.sample()
            one_hot_action = np.zeros(6)
            one_hot_action[action] = 1
            training_sampleX.append(observation)
            training_sampleY.append(one_hot_action)
            
            observation, reward, done, _ = env.step(action)
            score += reward
            print('score = ' + str(score))
            if done:
                break
        if score > min_score:
            scores.append(score)
            trainingX += training_sampleX
            trainingY += training_sampleY

    trainingX, trainingY = np.array(trainingX), np.array(trainingY)
    print('Average: {}'.format(np.mean(scores)))
    print('Median: {}'.format(np.median(scores)))
    return trainingX, trainingY

def create_model(w, h, s):
    model = Sequential()
    model.add(Dense(128, input_shape=(h, w, 3), activation="relu"))
    model.add(Dense(256, activation="relu"))
    model.add(Dense(512, activation="relu"))
    model.add(Dense(256, activation="relu"))
    model.add(Dense(128, activation="relu"))
    model.add(Dense(s,   activation="linear"))

    model.compile(
        loss='mse',
        optimizer='adam',
        metrics=['accuracy'])
    
    return model

def main():
    env = gym.make("bombermaaan-v0")
    env.start('D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187', 'Bombermaaan.exe', '')

    trainingX, trainingY = gather_data(env)

    print(env.width)

    print(trainingX.shape)
    print(trainingY.shape)

    model = create_model(env.width, env.height, 6)
    model.fit(trainingX, trainingY, epochs=5)
    
    scores = []
    num_trials = 50
    sim_steps = 500
    for trial in range(num_trials):
        print('Trial: {}'.format(trial))
        observation = env.reset()
        score = 0
        for step in range(sim_steps):
            print('Step: {}'.format(step))
            action = np.argmax(model.predict(observation))
            observation, reward, done, _ = env.step(action)
            score += reward
            if done:
                break
        scores.append(score)

    print(np.mean(scores))

    env.close()

if __name__ == "__main__":
    main()