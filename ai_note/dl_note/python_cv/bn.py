import numpy as np


def bn_train(x, gamma, beta, bn_param):
    """
    x                   : 输入数据，shape(B，L)
    gamma               : 缩放因子 gamma
    beta                : 平移因子
    bn_param            : batchnorm需要的参数
        eps             : 接近0的数，防止分母出现0
        momentum        : 动量参数，一般为0.9， 0.99， 0.999
        running_mean    : 滑动平均的方式计算新的均值，训练时计算，为测试数据做准备
        running_var     : 滑动平均的方式计算新的方差，训练时计算，为测试数据做准备
    """
    running_mean = bn_param['running_mean']     # shape = [B]
    running_var = bn_param['running_var']       # shape = [B]
    momentum = bn_param['momentum']
    eps = bn_param['eps']

    x_mean = x.mean(axis=0)     # x的均值
    x_var = x.var(axis=0)       # x的方差
    x_normalized = (x-x_mean)/np.sqrt(x_var + eps)  # 归一化
    results = gamma * x_normalized + beta           # 缩放平移

    running_mean = momentum * running_mean + (1 - momentum) * x_mean
    running_var = momentum * running_var + (1 - momentum) * x_var

    bn_param['running_mean'] = running_mean
    bn_param['running_var'] = running_var

    return results, bn_param


def bn_inference(x, gamma, beta, bn_param):
    """
    x                   : 输入数据，shape(B，L)
    gamma               : 缩放因子 gamma
    beta                : 平移因子
    bn_param            : batchnorm需要的参数
        eps             : 接近0的数，防止分母出现0
        momentum        : 动量参数，一般为0.9， 0.99， 0.999
        running_mean    : 滑动平均的方式计算新的均值，训练时计算，为测试数据做准备
        running_var     : 滑动平均的方式计算新的方差，训练时计算，为测试数据做准备
    """

    running_mean = bn_param['running_mean']
    running_var = bn_param['running_var']
    eps = bn_param['eps']
    results = 0

    x_normalized = (x - running_mean) / np.sqrt(running_var + eps)
    results = gamma * x_normalized + beta

