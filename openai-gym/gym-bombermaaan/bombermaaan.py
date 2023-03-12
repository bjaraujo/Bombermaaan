import os
import time
import argparse
import atexit
import gym
import gym_bombermaaan
import numpy as np
import random
import utils
from pathlib import Path
import pickle
from DQAgent import DQAgent
from evaluation import evaluate
from Logger import Logger
import pygame

def exit_handler():
    global DQA
    DQA.quit()
                 
BOMBERMAAAN_PATH = 'E:\\Projects\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.2.5.2231'
BOMBERMAAAN_EXE = 'Bombermaaan.exe'

IMG_SIZE = (96, 84)
utils.IMG_SIZE = IMG_SIZE

learning_rate = 0.00025
dropout = 0
replay_memory_size = 1e6
minibatch_size = 32
discount_factor = 0.99
epsilon = 1
epsilon_decrease_rate = 9e-7
min_epsilon = 0.1
experiences = []
training_count = 0
episode = 0
frame_counter = 0

state_file = Path('data/bombermaaan.pickle')
if state_file.exists():
    pickle_in = open(state_file, 'rb')
    learning_rate = pickle.load(pickle_in)
    dropout = pickle.load(pickle_in)
    replay_memory_size = pickle.load(pickle_in)
    minibatch_size = pickle.load(pickle_in)
    discount_factor = pickle.load(pickle_in)
    epsilon = pickle.load(pickle_in)
    epsilon_decrease_rate = pickle.load(pickle_in)
    min_epsilon = pickle.load(pickle_in)
    experiences = pickle.load(pickle_in)
    training_count = pickle.load(pickle_in)
    episode = pickle.load(pickle_in)
    frame_counter = pickle.load(pickle_in)
    pickle_in.close()    

# I/O
parser = argparse.ArgumentParser()
parser.add_argument('-t', '--train', action='store_true',
                    help='train the agent')
parser.add_argument('-l', '--load', type=str, default=None,
                    help='load the neural network weights from the given path')
parser.add_argument('-v', '--video', action='store_true',
                    help='show video output')
parser.add_argument('-d', '--debug', action='store_true',
                    help='run in debug mode (no output files)')
parser.add_argument('--eval', action='store_true',
                    help='evaluate the agent')
parser.add_argument('-e', '--environment', type=str,
                    help='name of the OpenAI Gym environment to use '
                         '(default: bombermaaan-v0)',
                    default='bombermaaan-v0')
parser.add_argument('--minibatch-size', type=int, default=minibatch_size,
                    help='number of sample to train the DQN at each update')
parser.add_argument('--replay-memory-size', type=int, default=replay_memory_size,
                    help='number of samples stored in the replay memory')
parser.add_argument('--target-network-update-freq', type=int, default=10e3,
                    help='frequency (number of DQN updates) with which the '
                         'target DQN is updated')
parser.add_argument('--avg-val-computation-freq', type=int, default=50e3,
                    help='frequency (number of DQN updates) with which the '
                         'average reward and Q value are computed')
parser.add_argument('--discount-factor', type=float, default=discount_factor,
                    help='discount factor for the environment')
parser.add_argument('--update-freq', type=int, default=4,
                    help='frequency (number of steps) with which to train the '
                         'DQN')
parser.add_argument('--learning-rate', type=float, default=0.00025,
                    help='learning rate for optimizer')
parser.add_argument('--epsilon', type=float, default=epsilon,
                    help='initial exploration rate for the agent')
parser.add_argument('--min-epsilon', type=float, default=min_epsilon,
                    help='final exploration rate for the agent')
parser.add_argument('--epsilon-decrease', type=float, default=epsilon_decrease_rate,
                    help='rate at which to linearly decrease epsilon')
parser.add_argument('--replay-start-size', type=int, default=50e3,
                    help='minimum number of transitions (with fully random '
                         'policy) to store in the replay memory before '
                         'starting training')
parser.add_argument('--initial-random-actions', type=int, default=30,
                    help='number of random actions to be performed by the agent'
                         ' at the beginning of each episode')
parser.add_argument('--dropout', type=float, default=dropout,
                    help='dropout rate for the DQN')
parser.add_argument('--max-episodes', type=int, default=np.inf,
                    help='maximum number of episodes that the agent can '
                         'experience before quitting')
parser.add_argument('--max-episode-length', type=int, default=np.inf,
                    help='maximum number of steps in an episode')
parser.add_argument('--max-frames-number', type=int, default=50e6,
                    help='maximum number of frames during the whole algorithm')
parser.add_argument('--test-freq', type=int, default=250000,
                    help='frequency (number of frames) with which to test the '
                         'agent\'s performance')
parser.add_argument('--validation-frames', type=int, default=135000,
                    help='number of frames to test the model like in table 3 of'
                         ' the paper')
parser.add_argument('--test-states', type=int, default=30,
                    help='number of states on which to compute the average Q '
                         'value')
args = parser.parse_args()

if args.debug:
    print('####################################################' \
          'WARNING: debug flag is set, output will not be saved' \
          '####################################################')

logger = Logger(debug=args.debug, append=args.environment)

# Variables
test_scores = []
test_mean_q = []
test_states = []

# Setup
env = gym.make(args.environment)
env.start(BOMBERMAAAN_PATH, BOMBERMAAAN_EXE, '')

network_input_shape = (4, IMG_SIZE[1], IMG_SIZE[0])  # Dimension ordering: 'th' (channels first)
DQA = DQAgent(env.action_space.n,
              network_input_shape,
              replay_memory_size=args.replay_memory_size,
              minibatch_size=args.minibatch_size,
              learning_rate=args.learning_rate,
              discount_factor=args.discount_factor,
              dropout_prob=args.dropout,
              epsilon=args.epsilon,
              epsilon_decrease_rate=args.epsilon_decrease,
              min_epsilon=args.min_epsilon,
              load_path=args.load,
              logger=logger)

atexit.register(exit_handler)  # Make sure to always save the model when exiting

# Initial logging
logger.log({
    'Action space': env.action_space.n,
    'Observation space': env.observation_space.shape
})
logger.log(vars(args))
training_csv = 'training_info.csv'
eval_csv = 'evaluation_info.csv'
test_csv = 'test_score_mean_q_info.csv'
logger.to_csv(training_csv, 'length,score')
logger.to_csv(eval_csv, 'length,score')
logger.to_csv(test_csv, 'avg_score,avg_Q')

train_agent = False
evaluate_agent = False

# Restore state
model_file = Path('data/bombermaaan.h5')
if model_file.exists():
    DQA.DQN.model.load_weights(model_file)

model_file = Path('data/bombermaaan_target.h5')
if model_file.exists():
    DQA.DQN_target.model.load_weights(model_file)

DQA.experiences = experiences
DQA.training_count = training_count

if args.video:
    pygame.init()
    surface= pygame.display.set_mode((IMG_SIZE[0], IMG_SIZE[1]))
    pygame.display.set_caption('Image')

if args.train:
    # Main loop
    while episode < args.max_episodes:
        # Start episode
        logger.log("Episode %d" % episode)
        score = 0

        # Observe reward and initialize first state
        obs = utils.preprocess_observation(env.reset())

        # Initialize the first state with the same 4 images
        current_state = np.array([obs, obs, obs, obs])

        # Main episode loop
        t = 0
        frame_counter += 1
                
        while t < args.max_episode_length:
            # Stop the episode if it takes too long
            if frame_counter > args.max_frames_number:
                DQA.quit()

            # Render the game
            if args.video:
                img = utils.rescale_image(env.render()).convert("RGB")
                pyg_img = pygame.image.fromstring(img.tobytes(), img.size, img.mode)
                surface.fill((255,255,255)) 
                surface.blit(pyg_img, (0, 0)) 
                pygame.display.update()  
            
            # Select an action using the DQA
            action = DQA.get_action(np.asarray([current_state]))                      
            
            # Observe reward and next state
            obs, reward, done, info = env.step(action)
            obs = utils.preprocess_observation(obs)
            next_state = utils.get_next_state(current_state, obs)

            frame_counter += 1

            # Store transition in replay memory
            clipped_reward = np.clip(reward, -1, 1)  # Clip the reward
            DQA.add_experience(np.asarray([current_state]),
                               action,
                               clipped_reward,
                               np.asarray([next_state]),
                               done)

            # Update the current state and score
            current_state = next_state
            score += reward
            
            if t % args.update_freq == 0:
                train_agent = True

            if frame_counter % args.test_freq == 0:
                evaluate_agent = True

            if done or t == args.max_episode_length - 1:
                env.pause()
                                    
                # Train the agent
                if train_agent and len(DQA.experiences) >= args.replay_start_size:
                    train_agent = False
                    DQA.train()
                    # Every C DQN updates, update DQN_target
                    if DQA.training_count % args.target_network_update_freq == 0 and DQA.training_count >= args.target_network_update_freq:
                        DQA.reset_target_network()
                    # Log the mean score and mean Q values of test states
                    if DQA.training_count % args.avg_val_computation_freq == 0 and DQA.training_count >= args.avg_val_computation_freq:
                        logger.to_csv(test_csv, [np.mean(test_scores), np.mean(test_mean_q)])
                        del test_scores[:]
                        del test_mean_q[:]

                # Linear epsilon annealing
                if len(DQA.experiences) >= args.replay_start_size:
                    DQA.update_epsilon()

                # Log episode data in the training csv
                logger.to_csv(training_csv, [t, score])
                logger.log("Length: %d; Score: %.1f\n" % (t + 1, score))

                # Evaluate the agent's performance
                if evaluate_agent:
                    evaluate_agent = False
                    t_evaluation, score_evaluation = evaluate(DQA, args, logger, env)
                    # Log evaluation data
                    logger.to_csv(eval_csv, [t_evaluation, score_evaluation])

                # Hold out a set of test states to monitor the mean Q value
                if len(test_states) < args.test_states:
                    # Generate test states
                    for _ in range(random.randint(1, 5)):
                        test_states.append(DQA.get_random_state())
                else:
                    # Update scores and mean Q values
                    test_scores.append(score)
                    test_q_values = [DQA.get_max_q(state) for state in test_states]
                    test_mean_q.append(np.mean(test_q_values))

                break
                
            t += 1

        episode += 1

        if episode % 10 == 0:
            data_dir = Path('data')
            if not os.path.exists('data'):
                os.mkdir(data_dir)
                
            DQA.DQN.model.save_weights('data/bombermaaan.h5')
            DQA.DQN_target.model.save_weights('data/bombermaaan_target.h5')
            
            pickle_out = open('data/bombermaaan.pickle', 'wb')
            pickle.dump(DQA.learning_rate, pickle_out)
            pickle.dump(DQA.dropout_prob, pickle_out)
            pickle.dump(DQA.replay_memory_size, pickle_out)
            pickle.dump(DQA.minibatch_size, pickle_out)
            pickle.dump(DQA.discount_factor, pickle_out)
            pickle.dump(DQA.epsilon, pickle_out)
            pickle.dump(DQA.epsilon_decrease_rate, pickle_out)
            pickle.dump(DQA.min_epsilon, pickle_out)
            pickle.dump(DQA.experiences, pickle_out)
            pickle.dump(DQA.training_count, pickle_out)
            pickle.dump(episode, pickle_out)
            pickle.dump(frame_counter, pickle_out)
            pickle_out.close()    

if args.eval:
    logger.log(evaluate(DQA, args, logger))
