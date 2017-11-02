#ifndef ITOA_H_INCLUDED
#define ITOA_H_INCLUDED

void strreverse(char* begin, char* end) {
    char aux;
    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}

void itoa(int value, char* str, int base) {
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr=str;
    int sign;
    div_t res;

    // Validate base
    if (base<2 || base>35){ *wstr='\0'; return; }

    // Take care of sign
    if ((sign=value) < 0) value = -value;

    // Conversion. Number is reversed.
    do {
        res = div(value,base);
        *wstr++ = num[res.rem];
    }while(value = res.quot);//it's fine
    if(sign<0) *wstr++='-';
    *wstr='\0';

    // Reverse string
    strreverse(str,wstr-1);
}

#endif // ITOA_H_INCLUDED
