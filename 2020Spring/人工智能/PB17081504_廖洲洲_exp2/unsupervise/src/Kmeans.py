#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# @Time: 2020/6/24 20:59
# @Author: Liao Zhouzhou
# @File: Kmeans.py
# @software: PyCharm
"""
import numpy as np
import pandas

def euc_dis(instance1, instance2):
    """
    计算两个样本instance1和instance2的欧式距离
    :param instance1: numpy数组类型，样本1
    :param instance2: numpy数组类型，样本2
    :return distance:float类型，欧式距离
    """
    #distance = np.sqrt(sum(np.power(instance1-instance2,2)))
    distance = np.sqrt(np.sum((np.array(instance1) - np.array(instance2)) ** 2))
    return distance


def randCent(data, k):
    """
    随机选择k个点作为起始质心
    :param data:初始数据集
    :param k:k个起始质心
    :return Centers: k个起始质心
    """
    # 维度
    dimension = np.shape(data)[1]
    centers = np.mat(np.zeros((k, dimension)))
    for i in range(dimension):
        minColumn = min(data[:, i])
        maxColumn = max(data[:, i])
        rangeColumn = float(maxColumn - minColumn)
        centers[:, i] = minColumn + rangeColumn * np.random.rand(k, 1)
    return centers

def computeSilhouetteCoefficient(data,centers,clusterAssign):
    """
    计算轮廓系数
    :param data: 输入数据
    :param centers: 各个簇的中心
    :param clusterAssign: 每个数据点的分配结果和距离质心的距离
    :return SilhouetteCoefficient: 轮廓系数
    """
    m,n = np.shape(data)
    SilhouetteCoefficient=[]
    ai=0
    bi=0
    # 求每一个点的轮廓系数
    for i in range(m):
        # 同一簇的点
        points = data[np.nonzero(clusterAssign[:,0].A == clusterAssign[i,0])[0]]
        sumDistance = 0
        for point in points:
            sumDistance += euc_dis(data[i,:],point)
        # 求平均时去除自己点
        if len(points) > 1:
            ai=sumDistance/(len(points)-1)
        else:
            ai=0
        # 求与该点相邻最近的一簇
        secondDistance = np.Inf
        secondIndex = -1
        for j in range(np.shape(centers)[0]):
            if j != clusterAssign[i,0]:
                distance = euc_dis(data[i,:],centers[j,:])
                if distance<secondDistance:
                    secondDistance = distance
                    secondIndex = j
        # 相邻最近一簇中所有点
        points = data[np.nonzero(clusterAssign[:,0].A == secondIndex)[0]]
        sumDistance = 0
        for point in points:
            sumDistance += euc_dis(data[i,:],point)
        bi=sumDistance/(len(points))
        SilhouetteCoefficient.append((bi-ai)/max(ai,bi))
    # print(SilhouetteCoefficient)
    return np.mean(SilhouetteCoefficient)

def Kmeans(k,data):
    """
    Kmeans算法主函数，输入数据和分类簇的个数，输出每一簇的质心和每一点的簇分配结果、与质心的距离
    将聚类结果输出到csv文件，文件名给出了k值和输入数据特征维度
    :param k:分类簇的个数
    :param data:输入数据
    :return centers:各个簇的中心
    :return clusterAssign:每个数据点的分配结果和距离质心的距离
    :return SilhouetteCoefficient: 轮廓系数
    """
    m,n = np.shape(data)
    # 记录每个数据点所分配的簇和距离该簇的质心的距离
    clusterAssign = np.mat(np.zeros((m,2)))
    # 初始随机k个质心
    centers = randCent(data,k)
    # print("data",data)
    # print("centers",centers)
    # 记录是否有点的簇分配发生改变
    isAssignChanged = True
    # 当任意一个点的簇分配发生改变时
    while isAssignChanged:
        isAssignChanged = False
        # 对数据集中的每个点
        for i in range(m):
            # 计算所有质心与该点距离，将其分配到最近的簇
            minDistance = np.Inf
            minIndex = -1
            for j in range(k):
                distance = euc_dis(data[i,:],centers[j,:])
                # print("distance:",distance)
                if distance < minDistance:
                    minDistance = distance
                    minIndex = j
            # 数据点被重新分配
            if clusterAssign[i,0] != minIndex:
                isAssignChanged = True
            clusterAssign[i,:] = minIndex,minDistance
        # 对每一个簇，计算簇中所有点的均值，并将其均值作为质心
        for j in range(k):
            points = data[np.nonzero(clusterAssign[:,0].A == j)[0]]
            if np.shape(points)[0]>0:
                centers[j,:] = np.mean(points,axis=0)
            else:
                centers[j,:] = randCent(data,k)[j,:]
    newDataLabel = np.hstack((clusterAssign[:,0],data))
    # print(newDataLabel)
    newDataLabel = pandas.DataFrame(newDataLabel)
    columns = ["label"]
    for i in range(n):
        columns.append("dimension"+str(i+1))
    newDataLabel.columns=columns
    # print(newDataLabel)
    newDataLabel.to_csv("..\\output\\"+"k_"+str(k)+"_dimension_"+str(n)+".csv")
    return centers,clusterAssign,computeSilhouetteCoefficient(data,centers,clusterAssign)