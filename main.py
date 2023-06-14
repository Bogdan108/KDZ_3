import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("data.csv")

# строю зависимость каждого алгоритма от количества вершин

for algo_type in df["algo"].unique():
    fig, plot = plt.subplots(nrows=1, ncols=1)
    fig.set_size_inches(17, 8)
    plot.set_xlabel("Count of vertex")
    plot.set_ylabel("Time")
    plot.set_title(f"{algo_type} - {'Count of vertex'} - {'Time'}")
    for graph_type in df["graph_type"].unique():
        data = df.query(
            "algo == @algo_type & graph_type == @graph_type")

        plot.plot(data["count_of_vertex"], data['time'], label=graph_type)

        plot.legend()
    print(f"![](graphs/{algo_type}_{'count_of_vertex'}.png)")
    fig.savefig(f"graphs/{algo_type}_{'count_of_vertex'}.png")
    plt.close(fig)

    # строю зависимость каждого алгоритма от количества ребер

    fig, plot = plt.subplots(nrows=1, ncols=1)
    fig.set_size_inches(17, 8)
    plot.set_xlabel("Count of edges")
    plot.set_ylabel("Time")
    plot.set_title(f"{algo_type} - {'Count of edges'} - {'Time'}")
    for graph_type in df["graph_type"].unique():
        data = df.query(
            "algo == @algo_type & graph_type == @graph_type")

        plot.plot(data["count_of_edges"], data['time'], label=graph_type)

        plot.legend()
    print(f"![](graphs/{algo_type}_{'count_of_edges'}.png)")
    fig.savefig(f"graphs/{algo_type}_{'count_of_edges'}.png")
    plt.close(fig)

# строю зависимость каждого типа графов от количества вершин

for graph_type in df["graph_type"].unique():
    fig, plot = plt.subplots(nrows=1, ncols=1)
    fig.set_size_inches(17, 8)
    plot.set_xlabel("Count of vertex")
    plot.set_ylabel("Time")
    plot.set_title(f"{graph_type} - {'Count of vertex'} - {'Time'}")
    for algo_type in df["algo"].unique():
        data = df.query(
            "graph_type == @graph_type & algo == @algo_type")
        plot.plot(data["count_of_vertex"], data['time'], label=algo_type)
        plot.legend()
    print(f"![](graphs/graph_type/{graph_type}_{'count_of_vertex'}.png)")
    fig.savefig(f"graphs/graph_type/{graph_type}_{'count_of_vertex'}.png")
    plt.close(fig)

    # строю зависимость каждого типа графов от количества ребер

    fig, plot = plt.subplots(nrows=1, ncols=1)
    fig.set_size_inches(17, 8)
    plot.set_xlabel("Count of edges")
    plot.set_ylabel("Time")
    plot.set_title(f"{graph_type} - {'Count of edges'} - {'Time'}")
    for algo_type in df["algo"].unique():
        data = df.query(
            "graph_type == @graph_type & algo == @algo_type")
        plot.plot(data["count_of_edges"], data['time'], label=algo_type)
        plot.legend()
    print(f"![](graphs/graph_type/{graph_type}_{'count_of_edges'}.png)")
    fig.savefig(f"graphs/graph_type/{graph_type}_{'count_of_edges'}.png")
    plt.close(fig)
