to run: for j in {0..18}; do (for i in {1..10}; do (./sum_buffer 2 10 $((2**j)) 64 3000 >> out-sum-3000.txt 2>&1); done); done



