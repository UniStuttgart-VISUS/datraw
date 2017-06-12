// t.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "datraw.h"


int main() {
    auto info = datraw::info<char>::parse("S:\\Daten\\Volumen\\tuebingen\\foot.dat");
    auto winfo = datraw::info<wchar_t>::parse(L"S:\\Daten\\Volumen\\tuebingen\\foot.dat");

    return 0;
}

