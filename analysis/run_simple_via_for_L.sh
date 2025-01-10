# Define the range of L values
for L in {10..150..10}
do
  # Time the simple vamana creation command with the current value of L
  start_time=$(date +%s)
  ./bin/main --create -index-type 'simple' -base-file 'data/siftsmall/siftsmall_base.fvecs' -L $L -R 12 -alpha 1.0 -save "models/L_values/simple_index_${L}.bin"
  end_time=$(date +%s)

  elapsed_time=$((end_time - start_time))
  
  echo "L=${L} took ${elapsed_time} seconds"
done
