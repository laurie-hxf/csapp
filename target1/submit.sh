sub(){
    ./hex2raw < answer3.txt >1.txt
    ./ctarget -q -i 1.txt
}