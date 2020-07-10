#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# @Time: 2020/6/19 12:21
# @Author: Liao Zhouzhou
# @File: main.py
# @software: PyCharm
"""

import pandas
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import numpy as np
from KNN import KNN
from SVM import SVM
from Logistic import Logisitic


def data_preprocessing(dataSet, label="G3"):
    """
    实现数据的预处理，包括字符串型属性转换为整型，属性和标签的划分，训练集和测试集的划分
    :param
        dataSet:从csv文件中读取出的DataFrame类型
    :return
        train_set:numpy数组类型，训练集的属性
        train_label:numpy数组类型，训练集的标签
        test_set:numpy数组类型，测试集的属性
        test_label:numpy数组类型，测试集的标签
    """
    data = dataSet.copy()
    columns = data.columns
    # 将字符型转换为整型
    for column in columns:
        if str(data[column].dtypes) != "int64":
            data[column] = LabelEncoder().fit_transform(data[column])
    #
    if label == "G3":
        data["G3"] = data["G3"].apply(lambda v: 1 if v >= 10 else -1)
    # print(data)
    # 分割属性和标签，同时将DataFrame转换为array
    if label == "G3":
        data_attr = np.array(data.loc[:, "school":"G2"])
    elif label == "famsup" or label == "higher":
        data_attr = np.hstack((np.array(data.loc[:, "famsize":"Fjob"]), np.array(data.loc[:, "G1":"G2"])))
    data_label = np.array(data[label])
    # 分割训练集和测试集
    train_set, test_set, train_label, test_label = train_test_split(data_attr, data_label, test_size=0.3,
                                                                    random_state=1)
    return train_set, train_label, test_set, test_label


def attr_select(train_set, test_set, selection):
    """
    实现数据的属性选择，选择使用哪些属性来进行模型训练
    :param train_set:numpy数组类型，训练集中所有的属性数据
    :param test_set:numpy数组类型，测试集中所有的属性数据
    :param selection:"onlyG1G2"只返回G1G2属性数据，"noG1G2"返回剔除G1G2属性的数据，"all"返回所有数据
    :return train_set_sel:返回用于训练模型的训练集数据
    :return test_set_sel:返回用于预测的测试集数据
    """
    if selection == "onlyG1G2":
        train_set_sel = train_set[:, -2:]
        test_set_sel = test_set[:, -2:]
    elif selection == "noG1G2":
        train_set_sel = train_set[:, :-2]
        test_set_sel = test_set[:, :-2]
    else:
        train_set_sel = train_set
        test_set_sel = test_set
    return train_set_sel, test_set_sel


def compute_f1score(test_label, predict_label):
    """
    计算性能评价指标
    :param test_label:numpy数组类型，样例真实标签
    :param predict_label: numpy数组类型，样例预测标签
    :return f1score:浮点数，性能评价指标
    """
    tp = 0
    fp = 0
    fn = 0
    tn = 0
    for i in list(range(len(test_label))):
        if test_label[i] == 1 and predict_label[i] == 1:
            tp = tp + 1
        elif (test_label[i] == -1 or test_label[i] == 0) and predict_label[i] == 1:
            fp = fp + 1
        elif test_label[i] == 1 and (predict_label[i] == -1 or predict_label[i] == 0):
            fn = fn + 1
        else:
            tn = tn + 1
    if tp == 0:
        return 0
    p = tp / (tp + fp)
    r = tp / (tp + fn)
    f1score = 2 * p * r / (p + r)
    return f1score


def call_knn(data):
    """
    实现KNN算法的调用，会使用不同的K值和选择不同的属性组合进行算法调用
    :param data: DataFrame格式，输入的数据集
    :return:
    """
    train_set, train_label, test_set, test_label = data_preprocessing(data)
    # print(type(train_label))
    print("训练集大小", len(train_set))
    print("测试集大小", len(test_set))
    attr_selections = ["all", "noG1G2", "onlyG1G2"]
    score = []
    for selection in attr_selections:
        # print(selection)
        train_set_selection, test_set_selection = attr_select(train_set, test_set, selection)
        f1score = []
        for k in list(range(1, 21)):
            predict_label = KNN(train_set_selection, train_label, test_set_selection, k_neighbors=k)
            # correct = np.count_nonzero((predict_label == test_lael))
            f1score.append(compute_f1score(test_label, predict_label))
        # print(f1score)
        score.append(f1score)
    score = pandas.DataFrame(score)
    score.index = attr_selections
    columns = []
    for k in list(range(1, 21)):
        columns.append("k=" + str(k))
    score.columns = columns
    print(score)


def call_svm(data):
    """
    实现SVM算法的调用，会使用不同核函数和参数组合进行算法调用，打印性能表
    :param data: DataFrame格式，输入的数据集
    :return:
    """
    train_set, train_label, test_set, test_label = data_preprocessing(data)
    print("训练集大小", len(train_set))
    print("测试集大小", len(test_set))
    print("未指明的参数默认c(惩罚参数)=200,epsilon(容错率)=0.01,maxInter(最大迭代次数)=100")
    attr_selections = ["all", "noG1G2", "onlyG1G2"]
    # attr_selections = ["all", "onlyG1G2"]
    # attr_selections = ["noG1G2"]
    # train_set_selection, test_set_selection = attr_select(train_set, test_set, "noG1G2")
    # print(compute_f1score(test_label, SVM(train_set_selection, train_label, test_set_selection,c=200,epsilon=0.1,maxIter=60,kernel=("linear",1))))

    score = []
    print("kernel:linear")
    for selection in attr_selections:
        train_set_selection, test_set_selection = attr_select(train_set, test_set, selection)
        f1score = []
        for c in [200, 500]:
            predict_label = SVM(train_set_selection, train_label, test_set_selection, c=c, kernel=("linear", 1))
            f1score.append(compute_f1score(test_label, predict_label))
            # print(f1score)
        score.append(f1score)
    score = pandas.DataFrame(score)
    score.index = attr_selections
    columns = []
    for k in [200, 500]:
        columns.append("c=" + str(k))
    score.columns = columns
    print(score)

    score = []
    print("kernel:poly (a*Xi*Xj+b)^p")
    for selection in attr_selections:
        train_set_selection, test_set_selection = attr_select(train_set, test_set, selection)
        f1score = []
        for kernel in [("poly", 1, 1, 2), ("poly", 1, 1, 3), ("poly", 1, 1, 4), ("poly", 1, 1, 5)]:
            predict_label = SVM(train_set_selection, train_label, test_set_selection, kernel=kernel)
            f1score.append(compute_f1score(test_label, predict_label))
        score.append(f1score)
    score = pandas.DataFrame(score)
    score.index = attr_selections
    columns = []
    for k in range(2, 6):
        columns.append("a=1,b=1,p=" + str(k))
    score.columns = columns
    print(score)

    score = []
    print("kernel:Gaussian")
    for selection in attr_selections:
        train_set_selection, test_set_selection = attr_select(train_set, test_set, selection)
        f1score = []
        for gamma in [2, 4, 8]:
            predict_label = SVM(train_set_selection, train_label, test_set_selection, kernel=("rbf", gamma))
            f1score.append(compute_f1score(test_label, predict_label))
        score.append(f1score)
    score = pandas.DataFrame(score)
    score.index = attr_selections
    columns = []
    for k in [2, 4, 8]:
        columns.append("gamma=" + str(k))
    score.columns = columns
    print(score)


def call_Logistic(data):
    """
    实现Logistic算法的调用,预测G3和根据家庭背景预测是否想接受高等教育
    :param data: DataFrame格式，输入的数据集
    :return:
    """
    print("预测Higher，是否想接受高等教育")
    train_set, train_label, test_set, test_label = data_preprocessing(data, label="higher")
    print("训练集大小", len(train_set))
    print("测试集大小", len(test_set))
    predictLabel = Logisitic(train_set, train_label, test_set)
    # print(predictLabel)
    f1score = compute_f1score(test_label, predictLabel)
    print("性能:%.3f" % f1score)

    print("\n预测G3")
    train_set, train_label, test_set, test_label = data_preprocessing(data)
    train_label = list(map(lambda x: 0 if x == -1 else 1, train_label))
    test_label = list(map(lambda x: 0 if x == -1 else 1, test_label))
    attr_selections = ["all", "noG1G2", "onlyG1G2"]
    score = []
    for selection in attr_selections:
        train_set_selection, test_set_selection = attr_select(train_set, test_set, selection)
        f1score = []
        predict_label = Logisitic(train_set_selection, train_label, test_set_selection)
        f1score.append(compute_f1score(test_label, predict_label))
        score.append(f1score)
    score = pandas.DataFrame(score)
    score.index = attr_selections
    columns = ["性能"]
    score.columns = columns
    print(score)


def main():
    """
    读取不同的数据集，对数据集分别调用KNN算法、SVM算法和Logistic算法,对每种算法对会使用不同的参数多次调用，输出不同参数的性能结果
    注：本函数将对2个文件、多种特征值、多种参数组合进行不同的算法调用，然后打印出性能表，因此时间会较长
    """
    pandas.set_option('display.max_columns', 30)
    pandas.options.display.float_format = '{:.3f}'.format
    file_name_mat = "..\\data\\student-mat.csv"
    file_name_por = "..\\data\\student-por.csv"
    # 读取csv文件
    data_mat = pandas.read_csv(file_name_mat, sep=";")
    data_por = pandas.read_csv(file_name_por, sep=';')
    # print(np.mat(data_por))

    print("KNN")
    print("student-mat.csv")
    call_knn(data_mat)
    print("\nstudent-por.csv")
    call_knn(data_por)

    print("SVM")
    print("student-mat.csv")
    call_svm(data_mat)
    print("\nstudent-por.csv")
    call_svm(data_por)

    print("Logistic")
    print("student-mat.csv")
    call_Logistic(data_mat)
    print("\nstudent-por.csv")
    call_Logistic(data_por)


if __name__ == '__main__':
    main()
