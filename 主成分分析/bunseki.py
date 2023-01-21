import pandas as pd
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA

# CSVファイルからデータを読み込む
data = pd.read_csv("data.csv")

# 主成分分析を実行
pca = PCA(n_components=6)
pca.fit(data)

# 第2主成分までのデータを取得し、2次元平面にプロット
transformed_data = pca.transform(data)
plt.scatter(transformed_data[:, 0], transformed_data[:, 1])
plt.xlabel("First Principal Component")
plt.ylabel("Second Principal Component")
plt.show()

# 第6主成分までの累積寄与率をグラフに示す
plt.plot(range(1, 7), pca.explained_variance_ratio_.cumsum(), marker='o')
plt.xlabel("Number of Principal Components")
plt.ylabel("Cumulative Explained Variance Ratio")
plt.show()

# 6つの主成分について6項目の特徴の主成分負荷量を求め、プリント
print(pca.components_)
