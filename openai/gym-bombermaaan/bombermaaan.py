import os
import argparse
import atexit
import gym
import gym_bombermaaan
import numpy as np
import random
import utils
from DQAgent import DQAgent
from evaluation import evaluate
from Logger import Logger

def exit_handler():
    global DQA
    DQA.quit()

IMG_SIZE = (96, 84)
utils.IMG_SIZE = IMG_SIZE

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
parser.add_argument('--minibatch-size', type=int, default=32,
                    help='number of sample to train the DQN at each update')
parser.add_argument('--replay-memory-size', type=int, default=1e6,
                    help='number of samples stored in the replay memory')
parser.add_argument('--target-network-update-freq', type=int, default=10e3,
                    help='frequency (number of DQN updates) with which the '
                         'target DQN is updated')
parser.add_argument('--avg-val-computation-freq', type=int, default=50e3,
                    help='frequency (number of DQN updates) with which the '
                         'average reward and Q value are computed')
parser.add_argument('--discount-factor', type=float, default=0.99,
                    help='discount factor for the environment')
parser.add_argument('--update-freq', type=int, default=4,
                    help='frequency (number of steps) with which to train the '
                         'DQN')
parser.add_argument('--learning-rate', type=float, default=0.00025,
                    help='learning rate for optimizer')
parser.add_argument('--epsilon', type=float, default=1,
                    help='initial exploration rate for the agent')
parser.add_argument('--min-epsilon', type=float, default=0.1,
                    help='final exploration rate for the agent')
parser.add_argument('--epsilon-decrease', type=float, default=9e-7,
                    help='rate at which to linearly decrease epsilon')
parser.add_argument('--replay-start-size', type=int, default=50e3,
                    help='minimum number of transitions (with fully random '
                         'policy) to store in the replay memory before '
                         'starting training')
parser.add_argument('--initial-random-actions', type=int, default=30,
                    help='number of random actions to be performed by the agent'
                         ' at the beginning of each episode')
parser.add_argument('--dropout', type=float, default=0.,
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
env.start('E:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.4.2204', 'Bombermaaan.exe', '')

network_input_shape = (4, 84, 96)  # Dimension ordering: 'th' (channels first)
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

# Set counters
episode = 0
frame_counter = 0

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
                env.render()

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

            # Train the agent
            if t % args.update_freq == 0 and len(DQA.experiences) >= args.replay_start_size:
                DQA.train()
                # Every C DQN updates, update DQN_target
                if DQA.training_count % args.target_network_update_freq == 0 and DQA.training_count >= args.target_network_update_freq:
                    DQA.reset_target_network()
                # Log the mean score and mean Q values of test states
                if DQA.training_count % args.avg_val_computation_freq == 0 and DQA.training_count >= args.avg_val_computation_freq:
                    logger.to_csv(test_csv,
                                  [np.mean(test_scores), np.mean(test_mean_q)])
                    del test_scores[:]
                    del test_mean_q[:]

            # Linear epsilon annealing
            if len(DQA.experiences) >= args.replay_start_size:
                DQA.update_epsilon()

            # Update the current state and score
            current_state = next_state
            score += reward

            # Log episode data in the training csv
            if done or t == args.max_episode_length - 1:
                env.pause()
                logger.to_csv(training_csv, [t, score])
                logger.log("Length: %d; Score: %d\n" % (t + 1, score))
                break

            t += 1

            # Evaluate the agent's performance
            if frame_counter % args.test_freq == 0:
                t_evaluation, score_evaluation = evaluate(DQA, args, logger)
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

        episode += 1

if args.eval:
    logger.log(evaluate(DQA, args, logger))
