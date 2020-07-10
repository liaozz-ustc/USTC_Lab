#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# @Time: 2020/6/23 23:28
# @Author: Liao Zhouzhou
# @File: main.py
# @software: PyCharm
"""
import pandas
import numpy as np
import matplotlib.pyplot as plt
from pca import *
from Kmeans import *


def data_preprocessing(dataSet):
    """
    实现数据的预处理，标准化
    :param dataSet: 原始数据
    :return normDataAttr: 标准化数据特征
    :return data_label: 数据原始分类
    """
    data = dataSet.copy()
    data_label = np.array(data[0])
    data_attr = np.array(data.loc[:, 1:13])
    # 数据标准化
    minVals = data_attr.min(0)
    maxVals = data_attr.max(0)
    ranges = maxVals - minVals
    normDataAttr = np.zeros(np.shape(data_attr))
    m = data_attr.shape[0]
    normDataAttr = data_attr - np.tile(minVals, (m, 1))
    normDataAttr = normDataAttr / np.tile(ranges, (m, 1))
    return normDataAttr, data_label


def plotData(data_attr, data_label, centers=None):
    """
    实现降维后的数据和二维聚类数据的可视化
    :param data_attr:
    :param data_label:
    :param centers:
    :return:
    """
    data_attr = np.array(data_attr)
    data_label = np.array(data_label)
    n = np.shape(data_attr)[0]
    x1 = []
    y1 = []
    x2 = []
    y2 = []
    x3 = []
    y3 = []
    for i in range(n):
        if int(data_label[i]) == 1:
            x1.append(data_attr[i, 0])
            y1.append(data_attr[i, 1])
        elif int(data_label[i]) == 2:
            x2.append(data_attr[i, 0])
            y2.append(data_attr[i, 1])
        else:
            x3.append(data_attr[i, 0])
            y3.append(data_attr[i, 1])
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.rcParams['axes.unicode_minus'] = False
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(x1, y1, s=40, c='red', marker='s', alpha=.8)
    ax.scatter(x2, y2, s=40, c='green', marker='o', alpha=.8)
    ax.scatter(x3, y3, s=40, c='blue', marker='^', alpha=.8)
    if not centers is None:
        plt.title("对降至2维的初始数据使用Kmeans聚类的结果")
        ax.scatter(list(centers[:, 0]), list(centers[:, 1]), s=200, c='black', marker='*', alpha=.9)
    else:
        plt.title("使用PCA降至2维的初始数据")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.show()


def computeRI(data_label, clusterAssign):
    """
    计算兰德系数
    :param data_label:真实分组情况
    :param clusterAssign:聚类结果
    :return RI:兰德系数
    """
    num = len(data_label)
    a = 0
    b = 0
    c = 0
    d = 0
    for i in range(num):
        for j in range(i+1, num):
            # 如果i、j真实分组为同一类
            if data_label[i] == data_label[j]:
                if clusterAssign[i, 0] == clusterAssign[j, 0]:
                    a = a + 1
                else:
                    b = b + 1
            else:
                if clusterAssign[i, 0] == clusterAssign[j, 0]:
                    c = c + 1
                else:
                    d = d + 1
    # print(a,b,c,d)
    RI = (a + d) / (a + b + c + d)
    return RI


def visualization(data_attr, data_label):
    """
    pca性能绘图和数据绘图统一调用
    :param data_attr: 标准化后数据特征
    :param data_label: 数据原始分类
    :return:
    """
    lowDimensionData = pca(data_attr, threshold=0.8, plotOrNot=True)
    lowDimensionData = pca(data_attr)
    plotData(lowDimensionData, data_label)
    centers, clusterAssign, silhouetteCoefficient = Kmeans(3, lowDimensionData)
    plotData(lowDimensionData, clusterAssign[:, 0], centers)
    return 1

def plotSCRI(threshold,kList,silhouetteCoefficientList,RIList):
    """
    绘制不同threshold降维的数据在不同的k值下聚类的轮廓系数和兰德系数图表
    :param threshold:阈值
    :param kList:k值
    :param silhouetteCoefficientList:轮廓系数
    :param RIList:兰德系数
    :return:
    """
    score = [silhouetteCoefficientList,RIList]
    score = pandas.DataFrame(score)
    score.index =["轮廓系数","兰德系数"]
    columns=[]
    for k in kList:
        columns.append("k=" + str(k))
    score.columns = columns
    print(score)
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.rcParams['axes.unicode_minus'] = False
    plt.plot(kList,silhouetteCoefficientList,c='red',marker='s',alpha=0.8,label="轮廓系数")
    plt.plot(kList,RIList,c='green',marker='o',alpha=0.8,label="兰德系数")
    plt.xlabel("k")
    plt.ylabel("轮廓系数或兰德系数")
    title = "threshold="+str(threshold)+"下不同k值对应的兰德系数和轮廓系数"
    plt.title(title)
    plt.legend()
    plt.show()

def main():
    """
    使用不同的threshold值调用pca算法对数据降维，再使用不同的k值调用Kmeans算法对降维的数据进行聚类
    对聚类结果计算兰德系数和轮廓系数
    实现可视化操作
    Kmeans中聚类结果输出到csv文件，文件名给出了k值和输入数据特征维度
    :return:
    """
    pandas.set_option('display.max_columns', 30)
    pandas.options.display.float_format = '{:.3f}'.format
    file_name = "..\\input\\wine.data"
    data = pandas.read_csv(file_name, sep=",", header=None)
    # print(data)
    data_attr, data_label = data_preprocessing(data)

    # 可视化
    visualization(data_attr, data_label)

    thresholds = [0.5, 0.6, 0.7, 0.8, 0.9]
    maxSilhouetteCoefficient=-1
    bestThreshold=0
    bestk=0
    for threshold in thresholds:
        lowDimensionData = pca(data_attr,threshold=threshold)
        dimension = np.shape(lowDimensionData)[1]
        print("threshold=%.1f,dimension=%d" % (threshold, dimension))
        silhouetteCoefficientList=[]
        RIList=[]
        for k in [2,3,4,5]:
            centers, clusterAssign, silhouetteCoefficient = Kmeans(k, lowDimensionData)
            if silhouetteCoefficient > maxSilhouetteCoefficient:
                maxSilhouetteCoefficient = silhouetteCoefficient
                bestThreshold = threshold
                bestk = k
            silhouetteCoefficientList.append(silhouetteCoefficient)
            RIList.append(computeRI(data_label,clusterAssign))
        plotSCRI(threshold,[2,3,4,5],silhouetteCoefficientList,RIList)
    print("bestThreshold="+str(bestThreshold)+",bestk="+str(bestk)+",maxSilhouetteCoefficient="+str(maxSilhouetteCoefficient))

    print("\n使用原始数据进行聚类")
    silhouetteCoefficientList=[]
    RIList=[]
    for k in [2, 3, 4, 5]:
        centers, clusterAssign, silhouetteCoefficient = Kmeans(k, data_attr)
        silhouetteCoefficientList.append(silhouetteCoefficient)
        RIList.append(computeRI(data_label, clusterAssign))
    plotSCRI(1, [2, 3, 4, 5], silhouetteCoefficientList, RIList)


if __name__ == '__main__':
    main()
