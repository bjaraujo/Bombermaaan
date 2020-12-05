import gym
import gym_bombermaaan

env = gym.make('bombermaaan-v0')

env.start('D:\\Programming\\Bombermaaan\\releases\\msvc16-win32\\Bombermaaan_2.1.2.2187', 'Bombermaaan.exe', '')
env.reset()

for _ in range(1000):
    
    env.render()
    
    action = env.action_space.sample()
    ob, reward, done, _ = env.step(action) # take action
    
    print(done)
    print(reward)
    print(env.bomber_dead)
       
    if done:
        env.reset()
        
env.close()