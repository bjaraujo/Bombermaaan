import time
import datetime
import numpy as np
from DQNetwork import DQNetwork
from random import random, randrange, randint, choice


class DQAgent:
    def __init__(self,
                 actions,
                 network_input_shape,
                 replay_memory_size=1024,
                 minibatch_size=32,
                 learning_rate=0.00025,
                 discount_factor=0.9,
                 dropout_prob=0.1,
                 epsilon=1,
                 epsilon_decrease_rate=0.99,
                 min_epsilon=0.1,
                 load_path=None,
                 logger=None):

        self.start_time = time.time()
        
        # Parameters
        self.network_input_shape = network_input_shape  # Shape of the DQN input
        self.actions = actions  # Size of the discrete action space
        self.learning_rate = learning_rate  # Learning rate for the DQN
        self.dropout_prob = dropout_prob  # Dropout probability of the DQN
        self.load_path = load_path  # Path from which to load the DQN's weights
        self.replay_memory_size = replay_memory_size  # Size of replay memory
        self.minibatch_size = minibatch_size  # Size of a DQN minibatch
        self.discount_factor = discount_factor  # Discount factor of the MDP
        self.epsilon = epsilon  # Probability of taking a random action
        self.epsilon_decrease_rate = epsilon_decrease_rate  # See update_epsilon
        self.min_epsilon = min_epsilon  # Minimum value for epsilon
        self.logger = logger

        # Replay memory
        self.experiences = []
        self.training_count = 0

        # Instantiate the deep Q-networks
        # Main DQN
        self.DQN = DQNetwork(
            self.actions,
            self.network_input_shape,
            learning_rate=self.learning_rate,
            discount_factor=self.discount_factor,
            minibatch_size=self.minibatch_size,
            dropout_prob=self.dropout_prob,
            load_path=self.load_path,
            logger=self.logger
        )

        # Target DQN used to generate targets
        self.DQN_target = DQNetwork(
            self.actions,
            self.network_input_shape,
            learning_rate=self.learning_rate,
            discount_factor=self.discount_factor,
            minibatch_size=self.minibatch_size,
            dropout_prob=self.dropout_prob,
            load_path=self.load_path,
            logger=self.logger
        )
        # Reset target DQN
        self.DQN_target.model.set_weights(self.DQN.model.get_weights())
        
    def get_action(self, state, testing=False, force_random=False):
        '''
        Polls DQN for Q-values. Returns argmax(Q) with probability 1-epsilon
        during training, 0.95 during testing.
        :param state: a state that can be passed as input to DQN
        :param testing: whether to use the current epsilon or the constant 0.05
        :param force_random: whether to sample a random action regardless of
            parameters
        :return: the index of (action associated to) the highest Q-value 
        '''
        is_random = (random() < (self.epsilon if not testing else 0.05))
        if force_random or is_random:
            return randint(0, self.actions - 1)
        else:
            q_values = self.DQN.predict(state)
            return np.argmax(q_values)

    def get_max_q(self, state):
        '''
        Returns the maximum Q value predicted on the given state.
        :param state: a state that can be passed as input to DQN
        :return: an action index corresponding to the maximum Q-value in the 
            given state
        '''
        q_values = self.DQN.predict(state)
        idxs = np.argwhere(q_values == np.max(q_values)).ravel()
        return np.random.choice(idxs)

    def get_random_state(self):
        '''
        Samples a random state from the replay memory.
        :return: the sampled state
        '''
        return self.experiences[randrange(0, len(self.experiences))]['source']

    def add_experience(self, source, action, reward, dest, final):
        '''
        Add a SARS' tuple to the experience replay.
        :param source: source state
        :param action: action index
        :param reward: reward associated to the transition
        :param dest: destination state
        :param final: whether the state is absorbing
        '''
        # Remove older transitions if the replay memory is full
        if len(self.experiences) >= self.replay_memory_size:
            self.experiences.pop(0)
        # Add a tuple (source, action, reward, dest, final) to replay memory
        self.experiences.append({'source': source,
                                 'action': action,
                                 'reward': reward,
                                 'dest': dest,
                                 'final': final})

        # Periodically log how many samples we've gathered so far
        if (len(self.experiences) % 100 == 0) and (len(self.experiences) < self.replay_memory_size) and (self.logger is not None):
            self.logger.log('Gathered %d samples of %d' % (len(self.experiences), self.replay_memory_size))
            t = (time.time() - self.start_time) * (self.replay_memory_size - len(self.experiences)) / (len(self.experiences) + 1)            
            self.logger.log('Estimated time left to gather all samples: %s' % (str(datetime.timedelta(seconds=t))))
            

    def sample_batch(self):
        '''
        Samples self.minibatch_size random transitions from the replay memory
        and returns them as a batch.
        :return: a batch of SARS' tuples
        '''
        batch = []
        for i in range(self.minibatch_size):
            batch.append(self.experiences[randrange(0, len(self.experiences))])
        return np.asarray(batch)

    def train(self):
        '''
        Trains the DQN on a minibatch of transitions.
        '''
        self.training_count += 1
        print('Training session #%d - epsilon: %f' % (self.training_count, self.epsilon))
        batch = self.sample_batch()
        self.DQN.train(batch, self.DQN_target)  # Train the DQN

    def update_epsilon(self):
        '''
        Decreases the probability of picking a random action, to improve
        exploitation.
        '''
        if self.epsilon - self.epsilon_decrease_rate > self.min_epsilon:
            self.epsilon -= self.epsilon_decrease_rate
        else:
            self.epsilon = self.min_epsilon

    def reset_target_network(self):
        '''
        Updates the target DQN with the current weights of the main DQN.
        '''
        if self.logger is not None:
            self.logger.log('Updating target network...')
        self.DQN_target.model.set_weights(self.DQN.model.get_weights())
                    
    def quit(self):
        '''
        Saves the DQN and the target DQN to file.
        '''
        if self.logger is not None:
            self.logger.log('Quitting...')
        self.DQN.save(append='_DQN')
        self.DQN_target.save(append='_DQN_target')
