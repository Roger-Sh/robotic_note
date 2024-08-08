import numpy as np
from matplotlib import pyplot


class KMeans(object):
    def __init__(self, k=2, tolerance=0.0001, max_iter=10):
        self.m_k = k                        # cluster number
        self.m_tolerance = tolerance        # tolerance
        self.m_max_iter = max_iter          # max_iter
        self.m_centers = {}                 # centers
        self.m_clusters = {}                # cluster features

    def fit(self, data):

        # 首先将前k个数据点作为中心点
        self.m_centers = {}
        for i in range(self.m_k):
            self.m_centers[i] = data[i]
        print(self.m_centers)

        # 循环最多max_iter次
        for i in range(self.m_max_iter):
            self.m_clusters = {}

            # 初始化每类的数组
            for j in range(self.m_k):
                self.m_clusters[j] = []

            # 遍历每一个点
            for p in data:

                # 记录每个点和当前所有中心点的距离
                distances = []
                for ct in self.m_centers:
                    # 欧几里得距离
                    distances.append((np.linalg.norm(p - self.m_centers[ct])))

                # 该点与哪个中心点的距离短，就说明该点属于这个中心点的类
                classification = distances.index(min(distances))
                self.m_clusters[classification].append(p)
                print("clusters", self.m_centers)

            # 保存当前的中心点
            prev_centers = dict(self.m_centers)
            print(prev_centers)

            # 求出新的中心点
            for c in self.m_clusters:
                # np.average() axix=0，纵轴求和，=1 横轴求和
                self.m_centers[c] = np.average(self.m_clusters[c], axis=0)
            print(self.m_centers)

            # 判断每个中心点的改动是否在容忍范围内
            optimized = True
            for ct in self.m_centers:
                prev_center = prev_centers[ct]
                cur_center = self.m_centers[ct]

                if np.sum((cur_center - prev_center) / prev_center * 100.0) > self.m_tolerance:
                    optimized = False

            # 如果中心点改动小于容忍范围，则说明改动不大，可以终止
            if optimized:
                break

    # 判断一个点属于哪个cluster，根据和每个中心点的距离
    def predict(self, p_data):
        distances = [np.linalg.norm(p_data - self.m_centers[ct]) for ct in self.m_centers]
        index = distances.index(min(distances))

        return index


if __name__ == '__main__':
    x = np.array([[1, 2],
                  [1.5, 1.8],
                  [5, 8],
                  [8, 8],
                  [1, 0.6],
                  [9, 11]])

    k_means = KMeans(k=2)
    k_means.fit(x)
    print(k_means.m_centers)

    # plot center
    for center in k_means.m_centers:
        pyplot.scatter(k_means.m_centers[center][0], k_means.m_centers[center][1], marker='*', s=150)

    # plot cluster
    for cat in k_means.m_clusters:
        for point in k_means.m_clusters[cat]:
            pyplot.scatter(point[0], point[1], c=('r' if cat == 0 else 'b'))

    predict_data = np.array([[2, 1],
                             [6, 9]])

    # plot prediction
    for feature in predict_data:
        print(feature)
        cat = k_means.predict(feature)
        pyplot.scatter(feature[0], feature[1], c=('r' if cat == 0 else 'b'), marker='x')

    pyplot.show()
