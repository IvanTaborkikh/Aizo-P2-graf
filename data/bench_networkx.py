import networkx as nx
import time
import os
import csv

here = os.path.dirname(os.path.abspath(__file__))
input_file = os.path.join(here, "bitcoin_alpha.txt")
out_file = os.path.join(here, "..", "results", "networkx_times.csv")
runs = 50

# build the directed graph from the dataset
G = nx.DiGraph()
with open(input_file) as f:
    v, e = map(int, f.readline().split())
    for line in f:
        a, b, w = map(int, line.split())
        G.add_edge(a, b, weight=w)
print(f"Dataset: {v} wierzcholkow, {e} krawedzi")

# measure one algorithm "runs" times, return list of times in microseconds
def bench(func):
    times = []
    for _ in range(runs):
        t = time.perf_counter()
        func()
        times.append(int((time.perf_counter() - t) * 1_000_000))
    return times

algos = [
    ("dijkstra",     lambda: nx.single_source_dijkstra_path_length(G, 0, weight="weight")),
    ("bellman-ford", lambda: nx.single_source_bellman_ford_path_length(G, 0, weight="weight")),
]

write_header = not os.path.exists(out_file)
with open(out_file, "a", newline="") as fh:
    w = csv.writer(fh)
    if write_header:
        w.writerow(["narzedzie", "algorytm", "wierzcholki", "krawedzie", "iteracja", "czas_us"])
    for name, func in algos:
        ts = bench(func)
        for i, t in enumerate(ts, 1):
            w.writerow(["networkx", name, v, e, i, t])
        avg, mn, mx = sum(ts) // len(ts), min(ts), max(ts)
        w.writerow(["networkx", name, v, e, "avg", avg])
        w.writerow(["networkx", name, v, e, "min", mn])
        w.writerow(["networkx", name, v, e, "max", mx])
        print(f"networkx {name}: avg={avg} min={mn} max={mx} us")

print(f"Zapisano do {out_file}")
