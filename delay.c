void _delay_cycles(unsigned int cycles){
    while(cycles--) asm("nop");
}