-   使用不同的threshold值调用pca算法对数据降维，再使用不同的k值调用Kmeans算法对降维的数据进行聚类

-   对聚类结果计算兰德系数和轮廓系数

-   实现可视化操作

-   Kmeans中聚类结果输出到csv文件，文件保存在output目录下，文件名给出了k值和输入数据特征维度,k_m_dimension_d.csv表示是将数据降为d维后使用k=m值调用Kmeans算法得到的聚类结果