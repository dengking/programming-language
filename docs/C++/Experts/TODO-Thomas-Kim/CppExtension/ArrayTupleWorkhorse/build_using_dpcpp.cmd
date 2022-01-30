dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb for_workhorse.cpp -o for_workhorse_d.exe
dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb apply_operations.cpp -o apply_operations_d.exe
dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb array_tuple_conversion.cpp -o array_tuple_conversion_d.exe
dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb binary_operations.cpp -o binary_operations_d.exe
dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb lambda_evaluation.cpp -o lambda_evaluation_d.exe
dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb make_sequence.cpp -o make_sequence_d.exe
dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb type_output.cpp -o type_output_d.exe
dpcpp -Qstd=c++17 /EHsc -fno-sycl -Qtbb tuple_variant_operations.cpp -o tuple_variant_operations_d.exe
