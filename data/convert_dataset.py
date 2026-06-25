import sys

input_file = "soc-sign-bitcoinalpha.csv"
output_file = "bitcoin_alpha.txt"

edges = []
nodes = set()

with open(input_file, "r") as f:
    for line in f:
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(",")
        src = int(parts[0])
        dst = int(parts[1])
        rating = int(parts[2])
        weight = rating + 11
        edges.append((src, dst, weight))
        nodes.add(src)
        nodes.add(dst)

node_list = sorted(nodes)
node_map = {old: new for new, old in enumerate(node_list)}

v = len(node_list)
e = len(edges)

print(f"Vertices: {v}, Edges: {e}")

with open(output_file, "w") as f:
    f.write(f"{v}\t{e}\n")
    for src, dst, weight in edges:
        f.write(f"{node_map[src]}\t{node_map[dst]}\t{weight}\n")

print(f"Saved to {output_file}")