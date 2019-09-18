import numpy as numpy
import matplotlib.pyplot as plt 
import pandas as pd 
from pandas.plotting import scatter_matrix
import math

def plot_lat_lng():
    # GOALの緯度・経度
    g_lat = 40.869971
    g_lng = -119.106299

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
    if lat_max < g_lat:
        lat_max = g_lat
    if lat_min > g_lat:
        lat_min = g_lat
    if lng_max < g_lng:
        lng_max = g_lng
    if lng_min > g_lng:
        lng_min = g_lng

    # 軸の下限と上限を設定する
    # x軸は経度，y軸は緯度
    ax.set_xlim(lng_min-0.0004, lng_max+0.0004)
    ax.set_ylim(lat_min-0.0004, lat_max+0.0004)

    # 矢印を描画する
    for i in range(len(df)):
        # 矢印の終了点を計算する
        end_x = (math.cos(df['control'][i]*0.01745329252)*0.00005 + df['lng'][i]) # cosθ + 経度
        end_y = (math.sin(df['control'][i]*0.01745329252)*0.00005 + df['lat'][i]) # sinθ + 緯度

        # 矢印の開始点と終了点
        point = {
            'start': [df['lng'][i], df['lat'][i]],
            'end':   [end_x,        end_y]
        }
        r = math.sqrt(((end_x - df['lng'][i]) * (end_x - df['lng'][i])) + ((end_y - df['lat'][i]) * (end_y - df['lat'][i])))
        print(r)

        # 矢印の設定
        if i == 0:
            plot_str = ""#  Control start point\n  "+str(round(df['lat'][i], 6))+"\n  "+str(round(df['lng'][i], 6))
        elif i == len(df)-1:
            plot_str = ""#  Control finish point\n  "+str(round((df['lat'][i]), 6))+"\n  "+str(round(df['lng'][i], 6))
        else:
            plot_str = ""

        ax.annotate(plot_str, xy=point['end'], xytext=point['start'],
                    arrowprops=dict(shrink=0, width=1, headwidth=8,
                                    headlength=10, connectionstyle='arc3',
                                    facecolor='red', edgecolor='red')
                   )

    # 折れ線を描画する
    plt.plot(g_lng, g_lat, '^')
    ax.annotate("  Target point\n  "+str(round(g_lat, 6))+"\n  "+str(round(g_lng, 6)), (g_lng, g_lat))
    plt.plot(df['lng'], df['lat'], '-o', linestyle='dashed', color='black')

    plt.plot(df['lng'][0], df['lat'][0], '^', color='orange')
    plt.plot(df['lng'][len(df)-1], df['lat'][len(df)-1], '^', color='green')
    plt.xlabel('longtitude')
    plt.ylabel('latitude')
    plt.show()

def main():
    plot_lat_lng()

if __name__ == "__main__":
    main()