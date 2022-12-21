#! /bin/bash

mkdir -p ../out/LZW/Decompressed_files

function create_decompressed_file {
    start_test_nr=$1
    end_test_nr=$2

	for test_nr in $(seq $start_test_nr $end_test_nr)
    do  
        extension=`find ../in/test${test_nr}.* | cut -d'.' -f4`
        ./LZW -d ../out/LZW/Compressed_files/test${test_nr}.bin ../out/LZW/Decompressed_files/test${test_nr}.${extension}
    done

}

create_decompressed_file 1 24