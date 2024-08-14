from tqdm import tqdm
import numpy as np
import torch
import collections
import random

class ReplayBuffer:
    """
    经验回放池
    """

    def __init__(self, capacity):
        """
        初始化经验回放池
        使用deque

        capacity：经验回放池容量
        """
        self.buffer = collections.deque(maxlen=capacity)

    def add(self, state, action, reward, next_state, done):
        """
        添加经验
        state, action, reward, next_state, done
        状态，动作，奖励，下一状态，结束标识
        """
        self.buffer.append((state, action, reward, next_state, done))

    def sample(self, batch_size):
        """
        从经验回放池采样一个批次
        batch_size：采样批次大小
        """

        # 采样
        transitions = random.sample(self.buffer, batch_size)

        # 按照元素打包
        state, action, reward, next_state, done = zip(*transitions)
        return np.array(state), action, reward, np.array(next_state), done

    def size(self):
        """
        获取经验回放池当前大小
        """
        return len(self.buffer)

def moving_average(a, window_size):
    """
    移动窗口平均
    """
    # Calculate the cumulative sum of the input array
    cumulative_sum = np.cumsum(np.insert(a, 0, 0))
    print("np.insert(a, 0, 0): ", np.insert(a, 0, 0))
    print("cumulative_sum: ", cumulative_sum)

    # Calculate the middle part of the moving average
    print("cumulative_sum[window_size:]: ", cumulative_sum[window_size:])
    print("cumulative_sum[:-window_size]: ", cumulative_sum[:-window_size])
    middle = (cumulative_sum[window_size:] - cumulative_sum[:-window_size]) / window_size
    print("middle: ", middle)

    # Calculate the beginning and ending parts of the moving average
    r = np.arange(1, window_size-1, 2)
    print("r: ", r)
    begin = np.cumsum(a[:window_size-1])[::2] / r
    print("begin: ", begin)
    end = (np.cumsum(a[:-window_size:-1])[::2] / r)[::-1]
    print("end: ", end)

    # Concatenate the beginning, middle, and ending parts
    return np.concatenate((begin, middle, end))

def train_on_policy_agent(env, agent, num_episodes):
    """
    训练在线策略的智能体
    行为策略与目标策略保持一致，不能使用经验回放
    env：环境
    agent：智能体
    num_episodes：训练轮数
    """
    # 记录奖励
    return_list = []

    # 用tqdm显示训练进度，将所有的训练轮数分为10个批次
    for i in range(10):
        with tqdm(total=int(num_episodes/10), desc='Iteration %d' % i) as pbar:
            # 训练一轮
            for i_episode in range(int(num_episodes/10)):
                # 当前轮次奖励
                episode_return = 0

                # 当前轮次状态，动作，奖励，下一状态，结束标识
                transition_dict = {'states': [], 'actions': [], 'next_states': [], 'rewards': [], 'dones': []}

                # 重置环境
                state = env.reset()
                done = False

                # 开始状态动作序列
                while not done:
                    # 智能体选择动作
                    action = agent.take_action(state)

                    # 环境执行动作
                    next_state, reward, done, _ = env.step(action)

                    # 记录当前轮次状态，动作，奖励，下一状态，结束标识
                    transition_dict['states'].append(state)
                    transition_dict['actions'].append(action)
                    transition_dict['next_states'].append(next_state)
                    transition_dict['rewards'].append(reward)
                    transition_dict['dones'].append(done)

                    # 更新状态
                    state = next_state

                    # 累计奖励
                    episode_return += reward

                # 记录当前轮次奖励
                return_list.append(episode_return)

                # 更新智能体的状态动作价值函数
                agent.update(transition_dict)

                # 打印当前轮次奖励
                if (i_episode+1) % 10 == 0:
                    pbar.set_postfix({'episode': '%d' % (num_episodes/10 * i + i_episode+1), 'return': '%.3f' % np.mean(return_list[-10:])})
                pbar.update(1)
    return return_list

def train_off_policy_agent(env, agent, num_episodes, replay_buffer, minimal_size, batch_size):
    """
    训练离线策略的智能体
    行为策略与目标策略不一致，可以使用经验回放
    env：环境
    agent：智能体
    num_episodes：训练轮数
    replay_buffer：经验
    minimal_size：经验回放池最小容量
    batch_size：采样批次大小
    """

    # 记录奖励
    return_list = []

    # 用tqdm显示训练进度，将所有的训练轮数分为10个批次
    for i in range(10):
        with tqdm(total=int(num_episodes/10), desc='Iteration %d' % i) as pbar:
            # 训练一轮
            for i_episode in range(int(num_episodes/10)):

                #  初始化当前轮次奖励
                episode_return = 0

                # 初始化环境
                state = env.reset()
                done = False

                # 开始状态动作序列
                while not done:

                    # 智能体选择动作
                    action = agent.take_action(state)

                    # 环境执行动作
                    next_state, reward, done, _ = env.step(action)

                    # 将 (s, a, r, s') 储存到经验回放池
                    replay_buffer.add(state, action, reward, next_state, done)

                    # 更新状态
                    state = next_state

                    # 累计奖励
                    episode_return += reward

                    # 通过经验回放数据更新智能体的状态动作价值函数
                    if replay_buffer.size() > minimal_size:
                        # 采样批次
                        b_s, b_a, b_r, b_ns, b_d = replay_buffer.sample(batch_size)
                        transition_dict = {'states': b_s, 'actions': b_a, 'next_states': b_ns, 'rewards': b_r, 'dones': b_d}

                        # 更新智能体的状态动作价值函数
                        agent.update(transition_dict)

                # 记录当前轮次奖励
                return_list.append(episode_return)

                # 打印当前轮次奖励
                if (i_episode+1) % 10 == 0:
                    pbar.set_postfix({'episode': '%d' % (num_episodes/10 * i + i_episode+1), 'return': '%.3f' % np.mean(return_list[-10:])})
                pbar.update(1)
    return return_list


def compute_advantage(gamma, lmbda, td_delta):
    """
    The Generalized Advantage Estimation (GAE) method is a technique used in reinforcement learning to estimate the advantage function,
    which is the difference between the expected return from taking a certain action and the expected return from following the current policy.
    The advantage function is used in policy gradient methods, such as the Proximal Policy Optimization (PPO) algorithm,
    to estimate the gradient of the policy with respect to the policy parameters.

    gamma: the discount factor
    lmbda: the GAE lambda parameter
    td_delta: the temporal difference errors
    """

    # This line converts the input td_delta tensor
    # (which likely contains the temporal difference errors) to a NumPy array,
    # detaching it from the computational graph if it was part of one.
    td_delta = td_delta.detach().numpy()

    # This initializes an empty list to store the computed advantages
    advantage_list = []

    # This initializes the current advantage value to 0.0
    advantage = 0.0

    # This loop iterates over the temporal difference errors in reverse order (from the last to the first).
    for delta in td_delta[::-1]:
        # This line computes the current advantage using the Generalized Advantage Estimation (GAE) formula
        # gamma is the discount factor
        # lmbda is the GAE lambda parameter
        # delta is the current temporal difference error
        advantage = gamma * lmbda * advantage + delta
        advantage_list.append(advantage)

    # This line reverses the order of the advantage_list to match the original order of the td_delta input.
    advantage_list.reverse()
    return torch.tensor(advantage_list, dtype=torch.float)



if __name__ == "__main__":
    data = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    window_size = 3
    res = moving_average(data, window_size)
    print(res)
