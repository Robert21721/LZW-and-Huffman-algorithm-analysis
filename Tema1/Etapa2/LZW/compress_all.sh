#! /bin/bash

mkdir -p ../out/LZW/Compressed_files

function create_compressed_file {
    start_test_nr=$1
    end_test_nr=$2

	for test_nr in $(seq $start_test_nr $end_test_nr)
    do
        extension=`find ../in/test${test_nr}.* | cut -d'.' -f4`
        ./LZW -c ../in/test${test_nr}.${extension} ../out/LZW/Compressed_files/test${test_nr}.bin
    done

}

create_compressed_file 1 24