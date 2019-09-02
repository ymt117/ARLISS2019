import numpy as numpy
import matplotlib.pyplot as plt 
import pandas as pd 
from pandas.plotting import scatter_matrix
import math

def plot_lat_lng():

    # csvファイルを読み込む
    # データは4列で構成される
    # |緯度|経度|制御量（角度）|
    df = pd.read_csv("control.csv")
    df.describe()

    fig=plt.figure(figsize=(10, 10))

    ax = fig.add_subplot(111)

    # 軸の下限と上限を設定するために最大値と最小値を所得する
    lat_max, lng_max, _ = df.max(axis='index')
    lat_min, lng_min, _ = df.min(axis='index')

    # 軸の下限と上限を設定する
    # x軸は経度，y軸は緯度
    ax.set_xlim(lng_min-0.0001, lng_max+0.0001)
    ax.set_ylim(lat_min-0.0001, lat_max+0.0001)

    # 矢印を描画する
    for i in range(len(df)):
        end_x = (math.cos(df['control'][i]*0.01745329252)*0.00005 + df['lng'][i]) # cosθ + 経度
        end_y = (math.sin(df['control'][i]*0.01745329252)*0.00005 + df['lat'][i]) # sinθ + 緯度

        print(end_x)

        # 矢印の開始点と終了点
        point = {
            'start': [df['lng'][i], df['lat'][i]],
            'end':   [end_x,        end_y]
        }

        # 矢印の設定
        ax.annotate('', xy=point['end'], xytext=point['start'],
                    arrowprops=dict(shrink=0, width=1, headwidth=8,
                                    headlength=10, connectionstyle='arc3',
                                    facecolor='red', edgecolor='red')
                   )

    # 折れ線を描画する
    plt.plot(df['lng'], df['lat'], '-o')
    plt.xlabel('longtitude')
    plt.ylabel('latitude')
    plt.show()

def main():
    plot_lat_lng()

if __name__ == "__main__":
    main()