#! /bin/bash

echo "sample input 1, fcntl"
./user_program/master 10 ./sample_input_1/target_file_1 ./sample_input_1/target_file_2 ./sample_input_1/target_file_3 ./sample_input_1/target_file_4 ./sample_input_1/target_file_5 ./sample_input_1/target_file_6 ./sample_input_1/target_file_7 ./sample_input_1/target_file_8 ./sample_input_1/target_file_9 ./sample_input_1/target_file_10 fcntl & ./user_program/slave 10 ./output/fcntl_output_file_1 ./output/fcntl_output_file_2 ./output/fcntl_output_file_3 ./output/fcntl_output_file_4 ./output/fcntl_output_file_5 ./output/fcntl_output_file_6 ./output/fcntl_output_file_7 ./output/fcntl_output_file_8 ./output/fcntl_output_file_9 ./output/fcntl_output_file_10 fcntl 127.0.0.1 > ./output/sample_input_1_fcntl_result.txt
echo "diff start"
diff ./sample_input_1/target_file_1 ./output/fcntl_output_file_1
diff ./sample_input_1/target_file_2 ./output/fcntl_output_file_2
diff ./sample_input_1/target_file_3 ./output/fcntl_output_file_3
diff ./sample_input_1/target_file_4 ./output/fcntl_output_file_4
diff ./sample_input_1/target_file_5 ./output/fcntl_output_file_5
diff ./sample_input_1/target_file_6 ./output/fcntl_output_file_6
diff ./sample_input_1/target_file_7 ./output/fcntl_output_file_7
diff ./sample_input_1/target_file_8 ./output/fcntl_output_file_8
diff ./sample_input_1/target_file_9 ./output/fcntl_output_file_9
diff ./sample_input_1/target_file_10 ./output/fcntl_output_file_10
echo "diff end"
sleep 5
echo "sample input 1, mmap"
./user_program/master 10 ./sample_input_1/target_file_1 ./sample_input_1/target_file_2 ./sample_input_1/target_file_3 ./sample_input_1/target_file_4 ./sample_input_1/target_file_5 ./sample_input_1/target_file_6 ./sample_input_1/target_file_7 ./sample_input_1/target_file_8 ./sample_input_1/target_file_9 ./sample_input_1/target_file_10 mmap & ./user_program/slave 10 ./output/mmap_output_file_1 ./output/mmap_output_file_2 ./output/mmap_output_file_3 ./output/mmap_output_file_4 ./output/mmap_output_file_5 ./output/mmap_output_file_6 ./output/mmap_output_file_7 ./output/mmap_output_file_8 ./output/mmap_output_file_9 ./output/mmap_output_file_10 mmap 127.0.0.1 > ./output/sample_input_1_mmap_result.txt
echo "diff start"
diff ./sample_input_1/target_file_1 ./output/mmap_output_file_1
diff ./sample_input_1/target_file_2 ./output/mmap_output_file_2
diff ./sample_input_1/target_file_3 ./output/mmap_output_file_3
diff ./sample_input_1/target_file_4 ./output/mmap_output_file_4
diff ./sample_input_1/target_file_5 ./output/mmap_output_file_5
diff ./sample_input_1/target_file_6 ./output/mmap_output_file_6
diff ./sample_input_1/target_file_7 ./output/mmap_output_file_7
diff ./sample_input_1/target_file_8 ./output/mmap_output_file_8
diff ./sample_input_1/target_file_9 ./output/mmap_output_file_9
diff ./sample_input_1/target_file_10 ./output/mmap_output_file_10
echo "diff end"
sleep 5
echo "sample input 2, fcntl"
./user_program/master 1 ./sample_input_2/target_file fcntl & ./user_program/slave 1 ./output/fcntl_output_file fcntl 127.0.0.1 > ./output/sample_input_2_fcntl_result.txt
echo "diff start"
diff ./sample_input_2/target_file ./output/fcntl_output_file
echo "diff end"
sleep 5
echo "sample input 2, mmap"
./user_program/master 1 ./sample_input_2/target_file mmap & ./user_program/slave 1 ./output/mmap_output_file mmap 127.0.0.1 > ./output/sample_input_2_mmap_result.txt
echo "diff start"
diff ./sample_input_2/target_file ./output/mmap_output_file
echo "diff end"
sleep 5
echo "my sample input, fcntl"
./user_program/master 1 ./my_sample_input/target_file_32M fcntl & ./user_program/slave 1 ./output/fcntl_output_file_32M fcntl 127.0.0.1 > ./output/my_sample_input_fcntl_result.txt
echo "diff start"
diff ./my_sample_input/target_file_32M ./output/fcntl_output_file_32M
echo "diff end"
sleep 5
echo "my sample input, mmap"
./user_program/master 1 ./my_sample_input/target_file_32M mmap & ./user_program/slave 1 ./output/mmap_output_file_32M mmap 127.0.0.1 > ./output/my_sample_input_mmap_result.txt
echo "diff start"
diff ./my_sample_input/target_file_32M ./output/mmap_output_file_32M
echo "diff end"
