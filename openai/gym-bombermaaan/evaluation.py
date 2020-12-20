from __future__ import print_function
import gym
import numpy as np
import utils

max_mean_score = 0


def evaluate(DQA, args, logger, env):
    global max_mean_score

    evaluation_csv = 'evaluation.csv'
    logger.to_csv(evaluation_csv, 'length,score')
    
    scores = list()
    frame_counter = 0

    while frame_counter < args.validation_frames:
        remaining_random_actions = args.initial_random_actions
        obs = utils.preprocess_observation(env.reset())

        frame_counter += 1
        # Initialize the first state with the same 4 images
        current_state = np.array([obs, obs, obs, obs])
        t = 0
        episode = 0
        score = 0

        # Start episode
        while True:
            # Render the game if video output is not suppressed
            if args.video:
                env.render()

            action = DQA.get_action(np.asarray([current_state]),
                                    testing=True,
                                    force_random=remaining_random_actions > 0)
            obs, reward, done, info = env.step(action)
            obs = utils.preprocess_observation(obs)
            current_state = utils.get_next_state(current_state, obs)

            if remaining_random_actions > 0:
                remaining_random_actions -= 1

            score += reward
            t += 1
            frame_counter += 1

            # End episode
            if done or t > args.max_episode_length:
                episode += 1
                print('Episode %d end\n---------------\nFrame counter: %d\n' % 
                      (episode, frame_counter))
                print('Length: %d\n, Score: %f\n\n' % (t, score))
                # Save episode data in the evaluation csv
                logger.to_csv(evaluation_csv, [t, score])
                break
                
        scores.append([t, score])

    scores = np.asarray(scores)
    max_indices = np.argwhere(scores[:, 1] == np.max(scores[:, 1])).ravel()
    max_idx = np.random.choice(max_indices)

    # Save best model
    if max_mean_score < np.mean(scores):
        max_mean_score = np.mean(scores)
        DQA.DQN.save(append='_best')

    return scores[max_idx, :].ravel()