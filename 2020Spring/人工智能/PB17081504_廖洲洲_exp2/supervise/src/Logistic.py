#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# @Time: 2020/6/23 9:53
# @Author: Liao Zhouzhou
# @File: Logistic.py
# @software: PyCharm
"""
import numpy as np


def sigmoid(x):
    """
    sigmoid函数
    :param x:输入的x值
    :return: 函数对应的sigmoid函数值
    """
    return 1.0 / (1 + np.exp(-x))


def gradAscent(train_set, train_label):
    """
    梯度上升算法求回归系数
    :param train_set: 训练集特征
    :param train_label:训练集标签
    :return weights:返回的回归系数矩阵
    """
    train_set = np.mat(train_set)
    train_label = np.mat(train_label).transpose()
    m, n = train_set.shape
    alpha = 0.01  # 移动步长，控制更新的弧度
    maxIter = 800  # 最大迭代次数
    weights = np.ones((n, 1))
    for k in range(maxIter):
        h = sigmoid(train_set * weights)
        error = train_label - h
        weights = weights + alpha * train_set.T * error
    return weights


def classifier(x, weights):
    """
    分类函数，根据权重和输入的x进行分类预测
    :param x: 特征向量
    :param weights: 权重向量
    :return label:预测的label
    """
    # print(float(x * weights))
    prob = sigmoid(float(x * weights))
    if prob > 0.5:
        return 1
    else:
        return 0


def Logisitic(train_set, train_label, test_set):
    """
    Logisitic算法主函数，根据训练集训练出的模型预测测试集的标签
    :param train_set:训练集特征(1,0)
    :param train_label:训练集标签
    :param test_set:测试集特征(1,0)
    :return:
    """
    weights = gradAscent(train_set, train_label)
    test_set = np.mat(test_set)
    m, n = test_set.shape
    predictLabel = []
    for i in range(m):
        predictLabel.append(classifier(test_set[i, :], weights))
    return predictLabel
