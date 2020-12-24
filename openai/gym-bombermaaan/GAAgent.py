import numpy as np
from random import random, randrange, randint

MAX_ACTIONS = 500

class GAAgent:
    def __init__(self):

        # Parameters
        self.actions = np.random.randint(0, 6, size=MAX_ACTIONS)
        self.rewards = np.zeros(MAX_ACTIONS)
        self.index = 0
        self.score = None
        self.score_total = 0.0
        self.nb_games = 0
    
    def reset(self):
        self.rewards = np.zeros(MAX_ACTIONS)
        self.index = 0
        
    def get_action(self):
        action = self.actions[self.index % MAX_ACTIONS]
        return action

    def set_reward(self, reward):
        self.rewards[self.index % MAX_ACTIONS] = reward

    def step(self):
        self.index = self.index + 1

    def add_score(self, score):
        self.nb_games = self.nb_games + 1
        self.score_total += score
        self.score = self.score_total / self.nb_games
        
    def get_score(self):
        return self.score

    def enhance(self):
        for i in range(MAX_ACTIONS):
            if self.rewards[i] < 0 and i > 1:
                self.actions[i] = randint(0, 5)
                self.actions[i-1] = randint(0, 5)
                self.actions[i-2] = randint(0, 5)
            
    def replicate(self, new_agent, agent1, agent2):
        for i in range(MAX_ACTIONS):
            if agent1.rewards[i] > 0:
                new_agent.actions[i] = agent1.actions[i] if random() > 0.2 else agent2.actions[i]
            else:
                if i > 1:
                    new_agent.actions[i] = agent2.actions[i]
                    new_agent.actions[i-1] = agent2.actions[i-1]
                    new_agent.actions[i-2] = agent2.actions[i-2]        
    
    def mutate(self, agent, mutation):
        for i in range(MAX_ACTIONS):
            if random() > mutation:
                agent.actions[i] = randint(0, 5)        
        
    def mate_with(self, agent, mutation):
        new_agent = GAAgent()
        
        if self.score > agent.score:
            self.replicate(new_agent, self, agent)
        else:
            self.replicate(new_agent, agent, self)
        
        self.mutate(new_agent, mutation)
        
        return new_agent
    
    def save(self, file_path):
        with open(file_path, 'w') as f:
            f.write(str(self.score) + '\n')
            f.write(','.join(np.char.mod('%d', self.actions)) + '\n')

    def load(self, file_path):
        with open(file_path, 'r') as f:
            score = f.readline()
            i = 0
            for a in f.readline().strip().split(','):
                self.actions[i] = int(a)
                i = i + 1
