import re
import os
import sys


here = os.path.dirname(os.path.abspath(__file__))
nx_file = os.path.join(here, "networkx_result.txt")
result_name = sys.argv[1] if len(sys.argv) > 1 else "bitcoin_dijkstra.txt"
my_file = os.path.join(here, "..", "results", result_name)

def read_nx(path):
    dist = {}
    for line in open(path):
        m = re.search(r"v\d+ -> v(\d+):\s*(-?\d+)", line)
        if m:
            dist[int(m.group(1))] = int(m.group(2))
    return dist

def read_mine(path):
    dist = {}
    for line in open(path):
        if "koszt:" not in line:
            continue
        path_part, cost = line.split("koszt:")
        target = re.findall(r"v(\d+)", path_part)[-1] 
        dist[int(target)] = int(cost.strip())
    return dist

nx = read_nx(nx_file)
mine = read_mine(my_file)

ok = 0
roznice = []
for v, d in nx.items():
    if mine.get(v) == d:
        ok += 1
    else:
        roznice.append((v, d, mine.get(v)))

print(f"Porownano: {len(nx)} wierzcholkow")
print(f"Zgodne: {ok}")
print(f"Roznice: {len(roznice)}")
for v, expected, got in roznice[:20]:
    print(f"  v{v}: NetworkX={expected}, moje={got}")

print("OK - wyniki sie zgadzaja!" if not roznice else "BLAD - sa roznice!")
