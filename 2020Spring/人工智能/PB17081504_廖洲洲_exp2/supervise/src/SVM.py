#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# @Time: 2020/6/21 11:09
# @Author: Liao Zhouzhou
# @File: SVM.py
# @software: PyCharm
"""
from numpy import *
import random


class SVMdata(object):
    def __init__(self, train_set, train_label, c, epsilon, kernel):
        """
        SVM分类器对象，保存了训练集的特征、标签和大小，误差表、b值、kernel函数等
        :param train_set:矩阵类型（m*d)，训练集的特征
        :param train_label:矩阵类型(m*1)，训练集的标签(-1 or 1)
        :param c:惩罚系数
        :param epsilon:精度
        :param kernel:元组类型，选择使用的核函数
        """
        self.x = train_set
        self.y = train_label
        self.c = c
        self.epsilon = epsilon
        self.m = shape(train_set)[0]
        self.alphas = mat(zeros((self.m, 1)))  # 拉格朗日乘子m*1
        self.b = 0
        self.eCache = mat(zeros((self.m, 2)))  # 误差缓存表m*2,第一列更新状态（0-未更新，1-已更新），第二列是实际的E值
        self.kernel = mat(zeros((self.m, self.m)))  # 核函数表kernel[i][j]=K(xi,xj)
        for i in range(self.m):
            self.kernel[:, i] = kernelTrans(self.x, self.x[i, :], kernel)


def kernelTrans(X, A, kernel):
    """
    为了不每次计算核函数的值，将K(Xi,Xj)保存在一个矩阵中，该方法计算矩阵的一列
    :param X: m*n矩阵
    :param A: 1*n矩阵
    :param kernel: 元组类型，核函数信息，第一个参数描述核函数类型，其他参数保存核函数相关参数
    :return k:1*m矩阵，K(Xi,Xj)的值，其中j固定，i从1到m
    """
    m = shape(X)[0]
    k = mat(zeros((m, 1)))
    if kernel[0] == "linear":
        k = X * A.T  # 线性核函数
    elif kernel[0] == "rbf":
        for j in range(m):
            delta = X[j, :] - A
            k[j] = delta * delta.T
            k[j] = exp(k[j] / (-1 * (kernel[1] ** 2)))
    elif kernel[0] == "poly":
        k = X * A.T
        k = kernel[1] * k + kernel[2]
        for i in range(kernel[3]):
            k = multiply(k, k)
    else:
        raise NameError("No such kernel")
    return k


def calcEk(SVMClassifier, k):
    """
    计算误差并返回
    :param SVMClassifier:SVM分类器对象
    :param k:计算fx的误差
    :return Ek：误差
    """
    fxk = float(multiply(SVMClassifier.alphas, SVMClassifier.y).T * SVMClassifier.kernel[:, k] + SVMClassifier.b)
    Ek = fxk - float(SVMClassifier.y[k])
    return Ek


def selectJ(SVMClassifier, i, Ei):
    """
    选择第二个alpha，以保证在每次优化中采用最大步长
    :param SVMClassifier:SVM分类器对象
    :param i: 第一个alpha值下标
    :param Ei: 第一个alpha值下标对应的误差
    :return j:第二个α的下标
    :return Ej:第二个α值下标对应的误差
    """
    maxK = -1
    maxDeltaE = 0
    Ej = 0
    SVMClassifier.eCache[i] = [1, Ei]  # 更新Ei
    validECacheList = nonzero(SVMClassifier.eCache[:, 0].A)[0]  # 构建出一个非零表
    if (len(validECacheList)) > 1:
        # 在有效的E值中循环，并找到最大步长的E
        for k in validECacheList:
            if k == i:
                continue
            Ek = calcEk(SVMClassifier, k)
            deltaE = abs(Ei - Ek)
            if (deltaE > maxDeltaE):
                maxK = k
                maxDeltaE = deltaE
                Ej = Ek
        return maxK, Ej
    else:  # 对于第一次循环
        j = selectJRand(i, SVMClassifier.m)
        Ej = calcEk(SVMClassifier, j)
    return j, Ej


def selectJRand(i, m):
    """
    随机寻找一个和i不同的j
    :param i: i值
    :param m: 最大值
    :return j：随机数
    """
    j = i
    while j == i:
        j = int(random.uniform(0, m))
    return j


def clipAlpha(aj, H, L):
    """
    调整α的值
    """
    if aj > H:
        aj = H
    if aj < L:
        aj = L
    return aj


def updateEk(SVMClassifier, k):
    """
    计算误差值并存入缓存中，在对α值进行优化后会用到这个值
    :param SVMClassifier: SVM分类器
    :param k: 下标
    :return:
    """
    Ek = calcEk(SVMClassifier, k)
    SVMClassifier.eCache[k] = [1, Ek]


def innerL(SVMClassifier, i):
    """
    SMO算法的内循环，如果i对应的数据向量可以被优化，选择另一个数据向量，同时优化这两个向量，如果都不能优化，退出
    :param SVMClassifier: SVM分类器
    :param i: 下标i
    :return:
    """
    Ei = calcEk(SVMClassifier, i)
    if ((SVMClassifier.y[i] * Ei < SVMClassifier.epsilon) and (SVMClassifier.alphas[i] < SVMClassifier.c)) or (
            (SVMClassifier.y[i] * Ei > SVMClassifier.epsilon) and (SVMClassifier.alphas[i] > 0)
    ):
        j, Ej = selectJ(SVMClassifier, i, Ei)  # 选择对应的alpha[j]
        alphaIold = SVMClassifier.alphas[i].copy()
        alphaJold = SVMClassifier.alphas[j].copy()
        if (SVMClassifier.y[i] != SVMClassifier.y[j]):
            L = max(0, SVMClassifier.alphas[j] - SVMClassifier.alphas[i])
            H = min(SVMClassifier.c, SVMClassifier.c + SVMClassifier.alphas[j] - SVMClassifier.alphas[i])
        else:
            L = max(0, SVMClassifier.alphas[j] + SVMClassifier.alphas[i] - SVMClassifier.c)
            H = min(SVMClassifier.c, SVMClassifier.alphas[j] + SVMClassifier.alphas[i])
        if L == H:
            return 0
        eta = 2 * SVMClassifier.kernel[i, j] - SVMClassifier.kernel[i, i] - SVMClassifier.kernel[j, j]
        if eta >= 0:
            return 0
        SVMClassifier.alphas[j] -= SVMClassifier.y[j] * (Ei - Ej) / eta
        SVMClassifier.alphas[j] = clipAlpha(SVMClassifier.alphas[j], H, L)
        updateEk(SVMClassifier, j)  # 更新
        if (abs(SVMClassifier.alphas[j] - alphaJold) < 0.00001):
            return 0
        SVMClassifier.alphas[i] += SVMClassifier.y[j] * SVMClassifier.y[i] * (
                alphaJold - SVMClassifier.alphas[j])  # update i by the same amount as j
        updateEk(SVMClassifier, i)  # 更新
        b1 = SVMClassifier.b - Ei - SVMClassifier.y[i] * (SVMClassifier.alphas[i] - alphaIold) * SVMClassifier.kernel[
            i, i] - SVMClassifier.y[j] * (
                     SVMClassifier.alphas[j] - alphaJold) * SVMClassifier.kernel[i, j]
        b2 = SVMClassifier.b - Ej - SVMClassifier.y[i] * (SVMClassifier.alphas[i] - alphaIold) * SVMClassifier.kernel[
            i, j] - SVMClassifier.y[j] * (
                     SVMClassifier.alphas[j] - alphaJold) * SVMClassifier.kernel[j, j]
        if (0 < SVMClassifier.alphas[i]) and (SVMClassifier.c > SVMClassifier.alphas[i]):
            SVMClassifier.b = b1
        elif (0 < SVMClassifier.alphas[j]) and (SVMClassifier.c > SVMClassifier.alphas[j]):
            SVMClassifier.b = b2
        else:
            SVMClassifier.b = (b1 + b2) / 2.0
        return 1
    else:
        return 0


def smoP(train_set, train_label, c, epsilon, maxIter, kernel=("linear", 0)):
    """
    Platter SMO算法，求解α和b值
    :param train_set: 训练集特征
    :param train_label: 训练集标签
    :param test_set: 测试机特征
    :param c: 软间隔的参数
    :param epsilon:精度
    :param maxIter:最大迭代次数
    :param kernel:核函数
    :return SVMClassifier.b:求得的b值
    :return SVMClassifier.alphas：求得的α矩阵
    """
    SVMClassifier = SVMdata(mat(train_set), mat(train_label).T, c, epsilon, kernel)
    iter = 0  # 计算循环的次数
    entireSet = True  # α被初始化为零向量，所以先遍历整个样本集
    alphaPairsChanged = 0
    while (iter < maxIter) and ((alphaPairsChanged > 0) or (entireSet)):
        # 循环在整个样本集或非边界点集上切换，达到最大循环次数时退出
        alphaPairsChanged = 0
        if entireSet:  # 循环遍历整个样本集
            for i in range(SVMClassifier.m):  # 外层循环
                alphaPairsChanged += innerL(SVMClassifier, i)
            iter += 1
        else:  # 循环遍历非边界点集
            nonBoundIs = nonzero((SVMClassifier.alphas.A > 0) * (SVMClassifier.alphas.A < c))[0]
            for i in nonBoundIs:  # 外层循环
                alphaPairsChanged += innerL(SVMClassifier, i)
            iter += 1
        if entireSet:
            entireSet = False
        elif (alphaPairsChanged == 0):  # 非边界点全部满足KKT条件，循环遍历整个样本集
            entireSet = True
    return SVMClassifier.b, SVMClassifier.alphas


def caclW(alphas, train_set, train_label):
    """
    计算w
    :param alphas:最优化α矩阵
    :param train_set: 训练集特征
    :param train_label: 训练集标签
    :return:
    """
    X = mat(train_set)
    Y = mat(train_label).T
    m, n = shape(X)
    w = zeros((n, 1))
    for i in range(m):
        w += multiply(alphas[i] * Y[i], X[i, :].T)
    return w


def SVM(train_set, train_label, test_set, c=200, epsilon=0.01, maxIter=60, kernel=("linear", 0)):
    """
    SVM主函数，支持向量机
    :param train_set: 训练集特征
    :param train_label: 训练集标签
    :param test_set: 测试机特征
    :param c: 软间隔的参数,默认值为200
    :param epsilon:容错率，停止训练的误差精度
    :param maxIter:最大迭代次数
    :param kernel:核函数
    :return predictLabel：预测测试集的标签
    """
    b, alphas = smoP(train_set, train_label, c, epsilon, maxIter, kernel)
    w = caclW(alphas, train_set, train_label)
    test_set = mat(test_set)
    train_set = mat(train_set)
    train_label = mat(train_label).T
    svIndex = nonzero(alphas.A > 0)[0]  # 找出alphas中大于0的元素位置
    sv_set = train_set[svIndex]  # 获得支持向量的矩阵
    sv_label = train_label[svIndex]  # 获得支持向量的标签
    m, n = shape(test_set)
    predictLabel = []
    for i in range(m):
        kernelValue = kernelTrans(sv_set, test_set[i, :], kernel)
        predictLabel.append(sign(kernelValue.T * multiply(sv_label, alphas[svIndex]) + b))
    return predictLabel
