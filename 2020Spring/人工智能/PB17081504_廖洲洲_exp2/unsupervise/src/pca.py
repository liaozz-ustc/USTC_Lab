#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# @Time: 2020/6/24 9:07
# @Author: Liao Zhouzhou
# @File: pca.py
# @software: PyCharm
"""
from numpy import *
import matplotlib.pyplot as plt

def plotData(Data,title="m-第m个主成分特征值贡献率"):
    """
    对数据Data作折线图，用于生成m-特征值贡献度图
    :param Data: 输入的数据
    :param title: 图标题
    :return:
    """
    # fig = plt.figure()
    # ax = fig.add_subplot(111)
    # print(len(Data))
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.rcParams['axes.unicode_minus'] = False
    plt.plot(list(range(1, len(Data))), Data[1:], c='red', marker='s', alpha=.8)
    plt.title(title)
    plt.xlabel("m")
    plt.ylabel("贡献率")
    plt.show()

def plotData2(Data,title="前m个主成分特征值贡献率-m"):
    """
    对数据Data作折线图，用于生成前m个主成分特征值贡献率-m
    :param Data: 输入的数据
    :param title: 图标题
    :return:
    """
    # fig = plt.figure()
    # ax = fig.add_subplot(111)
    # print(len(Data))
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.rcParams['axes.unicode_minus'] = False
    plt.plot(Data[1:],list(range(1, len(Data))), c='red', marker='s', alpha=.8)
    plt.title(title)
    plt.xlabel("贡献率")
    plt.ylabel("m")
    plt.show()

def pca(data,threshold=-1,firstm=2,plotOrNot=False):
    """
    使用pca算法实现对数据的降维，当输入了threshold时，根据threshold值计算选择前m个特征向量
    当未输入threshold值,而输入了firstm值时，直接根据firstm值选择前firstm个特征向量
    当threshold和firstm值都未输入时，选择前2个特征向量
    根据plotOrNot绘制m和特征值贡献度的值
    :param data: 输入的高维数据
    :param threshold: 表示特征值的累计贡献度，根据该值选择前m个特征向量
    :param firstm: 直接选择前m个特征向量
    :param plotOrNot:选择是否绘图
    :return lowDimensionDataMat: 降维的数据
    """
    # 对所有样本进行去中心化，减去平均值
    meanValues= mean(data,axis=0)
    newData=data-meanValues
    # 计算协方差矩阵
    covMat = cov(newData,rowvar=0)
    # 计算协方差矩阵的特征值和特征向量
    eigValues,eigVectors = linalg.eig(mat(covMat))
    # 将特征值从大到小排序，得到索引值
    eigValuesIndex = argsort(eigValues)[-1::-1]
    # 得到排序后的特征值，特征向量
    eigValuesSort = eigValues[eigValuesIndex]
    eigVectorsSort = eigVectors[:,eigValuesIndex]
    # print(eigValues)
    # # print(eigVectors)
    # print(eigValuesSort)
    sumEigValues = sum(eigValues)
    if plotOrNot:
        plotData([0] + list(eigValuesSort / sumEigValues))

    #  print(eigValues)
    # # print(eigVectors)
    #  print(eigValuesSort)
    # print(eigVectorsSort)
    # print(eigVectorsSort*eigVectorsSort.T)
    if threshold!=-1:
        firstmSum = [0]
        for i in range(len(eigValuesSort)):
            firstmSum.append(firstmSum[i]+eigValuesSort[i])
        firstmSum=[x/sumEigValues for x in firstmSum]
        for i in range(len(firstmSum)):
            if(threshold <= firstmSum[i] ):
                firstm = i
                break
        if plotOrNot:
            plotData(firstmSum,"m-前m个特征值的累计贡献率")
            plotData2(firstmSum)
    firstmEigVectors = eigVectorsSort[:,:firstm]
    lowDimensionDataMat = newData * firstmEigVectors
    return lowDimensionDataMat

if __name__ == '__main__':
    a = array([[1, 2, 3], [2, 2, 2], [1, 1, 1], [4, 4, 4]])
    pca(a,threshold=0.9)