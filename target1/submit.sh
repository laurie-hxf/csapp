sub(){
    ./hex2raw < answer5.txt >1.txt
    ./rtarget -q -i 1.txt
}