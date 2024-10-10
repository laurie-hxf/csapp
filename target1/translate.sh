trans(){
    gcc -c "$1" -o 3.out
    objdump -d 3.out >3.asm
    rm 3.out
}