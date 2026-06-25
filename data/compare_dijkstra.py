import networkx as nx

input_file = "bitcoin_alpha.txt"
source = 0

G = nx.DiGraph()

with open(input_file, "r") as f:
    first_line = f.readline().strip().split()
    v = int(first_line[0])
    e = int(first_line[1])
    print(f"Graph: {v} vertices, {e} edges")

    for line in f:
        parts = line.strip().split()
        src = int(parts[0])
        dst = int(parts[1])
        weight = int(parts[2])
        G.add_edge(src, dst, weight=weight)

print(f"NetworkX graph: {G.number_of_nodes()} nodes, {G.number_of_edges()} edges")
print(f"Running Dijkstra from v{source}...")

lengths = nx.single_source_dijkstra_path_length(G, source, weight="weight")

print(f"\nShortest distances from v{source} (first 20):")
for node in sorted(lengths.keys())[:20]:
    if node != source:
        print(f"  v{source} -> v{node}: {lengths[node]}")

print(f"\nTotal reachable vertices: {len(lengths) - 1}")
print(f"\nSave full results to networkx_result.txt...")

with open("networkx_result.txt", "w") as f:
    f.write(f"Dijkstra from v{source}\n")
    for node in sorted(lengths.keys()):
        if node != source:
            f.write(f"v{source} -> v{node}: {lengths[node]}\n")

print("Done!")