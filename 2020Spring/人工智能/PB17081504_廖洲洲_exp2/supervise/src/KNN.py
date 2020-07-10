#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# @Time: 2020/6/20 22:22
# @Author: Liao Zhouzhou
# @File: KNN.py
# @software: PyCharm
"""

import numpy as np
from scipy import stats


def KNN(train_set, train_label, test_set, k_neighbors):
    """
    输入训练集数据和测试集的属性及k值，输出预测标签集
    :param train_set:numpy数组类型，训练集的属性
    :param train_label:numpy数组类型，训练集的标签
    :param test_set:numpy数组类型，测试集的属性
    :param k_neighbors:整型，k个邻居
    :return predict_label:numpy数组类型，预测的标签
    """
    predict_label = []
    for test_instance in test_set:
        # 对测试集的每个样本进行预测
        distance = [euc_dis(test_instance, train_instance) for train_instance in train_set]
        # 对欧式距离进行排序，得到数组值从小到大的索引值
        neighbors = np.argsort(distance)
        k_near_neighbors = neighbors[:k_neighbors]
        # 统计k个邻居最多的标签值
        predict_label.append(stats.mode(train_label[k_near_neighbors])[0][0])
    predict_label = np.array(predict_label)
    # print(predict_label)
    return predict_label


def euc_dis(instance1, instance2):
    """
    计算两个样本instance1和instance2的欧式距离
    :param instance1: numpy数组类型，样本1
    :param instance2: numpy数组类型，样本2
    :return distance:float类型，欧式距离
    """
    distance = np.sqrt(sum((instance1 - instance2) ** 2))
    return distance
