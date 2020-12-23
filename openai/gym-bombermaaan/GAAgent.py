import numpy as np
from random import random, randrange, randint

MAX_ACTIONS = 500

class GAAgent:
    def __init__(self):

        # Parameters
        self.actions = np.random.randint(0, 6, size=MAX_ACTIONS)
        self.index = 0
        self.score = None
        self.score_total = 0.0
        self.nb_games = 0
    
    def reset(self):
        self.index = 0
        
    def get_action(self):
        action = self.actions[self.index % MAX_ACTIONS]
        self.index = self.index + 1
        return action

    def add_score(self, score):
        self.nb_games = self.nb_games + 1
        self.score_total += score
        self.score = self.score_total / self.nb_games
        
    def get_score(self):
        return self.score
        
    def mate_with(self, agent, mutation):
        new_agent = GAAgent()
        
        epsilon = 0.2 if self.score > agent.score else 0.8
            
        for i in range(MAX_ACTIONS):
            new_agent.actions[i] = self.actions[i] if random() > epsilon else agent.actions[i]
            
            if random() > mutation:
                new_agent.actions[i] = randint(0, 5)
            
        return new_agent
        
    