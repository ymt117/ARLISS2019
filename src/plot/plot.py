import numpy as numpy
import matplotlib.pyplot as plt 
import pandas as pd 
from pandas.plotting import scatter_matrix
import math

def plot_lat_lng():
    # GOALの緯度・経度
    g_lat = 31.567613
    g_lng = 130.543375

    # csvファイルを読み込む
    # データは3列で構成される
    # |緯度|経度|制御量（角度）|
    df = pd.read_csv("control.csv")
    df.describe()

    fig=plt.figure(figsize=(15, 15))

    ax = fig.add_subplot(111)

    # 軸の下限と上限を設定するために最大値と最小値を所得する
    lat_max, lng_max, _ = df.max(axis='index')
    lat_min, lng_min, _ = df.min(axis='index')

    # 軸の下限と上限を設定する
    # x軸は経度，y軸は緯度
    ax.set_xlim(lng_min-0.0002, lng_max+0.0002)
    ax.set_ylim(lat_min-0.0002, lat_max+0.0002)

    # 矢印を描画する
    for i in range(len(df)):
        end_x = (math.cos(df['control'][i]*0.01745329252)*0.00005 + df['lng'][i]) # cosθ + 経度
        end_y = (math.sin(df['control'][i]*0.01745329252)*0.00005 + df['lat'][i]) # sinθ + 緯度

        # 矢印の開始点と終了点
        point = {
            'start': [df['lng'][i], df['lat'][i]],
            'end':   [end_x,        end_y]
        }

        # 矢印の設定
        if i == 0:
            plot_str = "  Control start point\n  "+str(df['lat'][i])+"\n  "+str(df['lng'][i])
        elif i == len(df)-1:
            plot_str = "  Control finish point\n  "+str(df['lat'][i])+"\n  "+str(df['lng'][i])
        else:
            plot_str = ""
        ax.annotate(plot_str, xy=point['end'], xytext=point['start'],
                    arrowprops=dict(shrink=0, width=1, headwidth=8,
                                    headlength=10, connectionstyle='arc3',
                                    facecolor='red', edgecolor='red')
                   )

    # 折れ線を描画する
    plt.plot(g_lng, g_lat, '^')
    ax.annotate("  Target point\n  "+str(g_lat)+"\n  "+str(g_lng), (g_lng, g_lat))
    plt.plot(df['lng'], df['lat'], '-o', linestyle='dashed', color='black')
    plt.xlabel('longtitude')
    plt.ylabel('latitude')
    plt.show()

def main():
    plot_lat_lng()

if __name__ == "__main__":
    main()