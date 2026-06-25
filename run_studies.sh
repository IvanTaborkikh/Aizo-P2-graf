#!/bin/bash

BINARY="./graph"
ITERATIONS=67
RESULTS_DIR="../results"

mkdir -p $RESULTS_DIR

echo "=== Starting all studies ==="
echo ""

echo "=== Study A - wplyw rozmiaru grafu (gestosc 50%) ==="
SIZES=(10 20 50 100 200)

echo "--- MST ---"
for size in "${SIZES[@]}"; do
    echo "Size: $size"
    $BINARY --benchmark -p 0 -a 0 -s 0 -l $size -d 50 -n $ITERATIONS -r $RESULTS_DIR/study_A_mst.csv
done

echo "--- SP ---"
for size in "${SIZES[@]}"; do
    echo "Size: $size"
    $BINARY --benchmark -p 1 -a 0 -s 0 -l $size -d 50 -n $ITERATIONS -r $RESULTS_DIR/study_A_sp.csv
done

echo "--- MF ---"
for size in "${SIZES[@]}"; do
    echo "Size: $size"
    $BINARY --benchmark -p 2 -a 5 -s 0 -l $size -d 50 -n $ITERATIONS -r $RESULTS_DIR/study_A_mf.csv
done

echo "Study A done!"
echo ""

echo "=== Study B - wplyw gestosci grafu ==="
DENSITIES=(25 50 75)
B_SIZE=100

echo "--- MST ---"
for density in "${DENSITIES[@]}"; do
    echo "Density: $density%"
    $BINARY --benchmark -p 0 -a 0 -s 0 -l $B_SIZE -d $density -n $ITERATIONS -r $RESULTS_DIR/study_B_mst.csv
done

echo "--- SP ---"
for density in "${DENSITIES[@]}"; do
    echo "Density: $density%"
    $BINARY --benchmark -p 1 -a 0 -s 0 -l $B_SIZE -d $density -n $ITERATIONS -r $RESULTS_DIR/study_B_sp.csv
done

echo "--- MF ---"
for density in "${DENSITIES[@]}"; do
    echo "Density: $density%"
    $BINARY --benchmark -p 2 -a 5 -s 0 -l $B_SIZE -d $density -n $ITERATIONS -r $RESULTS_DIR/study_B_mf.csv
done

echo "Study B done!"
echo ""

echo "=== All studies completed! ==="
echo "Results saved in: $RESULTS_DIR/"
ls -la $RESULTS_DIR/