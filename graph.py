import pandas as pd
from matplotlib import pyplot as plt
import os

plt.rcParams["figure.figsize"] = [7.00, 3.50]
plt.rcParams["figure.autolayout"] = True

columns = ['RN', 'AVL', 'B-1', 'B-5', 'B-10']

df = pd.read_csv("output/avgCaseAdd.csv", usecols=columns, sep=";")

df.plot()
plt.xscale("linear")
plt.yscale("log")
plt.title("Gráfico do Caso Médio da Adição")
plt.savefig(os.path.join("output", "avgCaseAdd.png"))
plt.close()

dc = pd.read_csv("output/avgCaseRem.csv", usecols=columns, sep=";")

dc.plot()
plt.xscale("linear")
plt.yscale("log")
plt.title("Gráfico do Caso Médio da Remoção")
plt.savefig(os.path.join("output", "avgCaseRem.png"))
plt.close()
