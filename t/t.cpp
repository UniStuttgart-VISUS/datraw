// t.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "datraw.h"
#include <iterator>


int main() {
    try {
        auto info = datraw::info<char>::parse("S:\\Daten\\Volumen\\tuebingen\\foot.dat");
        std::vector<std::string> props;
        info.property_names(std::back_inserter(props));
        for (auto& p : props) {
            std::cout << p << std::endl;
        }

        //{
        //    auto x = info.byte_order();
        //}
        //{
        //    auto x = info.components();
        //}
        //{
        //    auto x = info.data_offset();
        //}
        //{
        //    auto x = info.dimensions();
        //}
        {
            auto x = info.format();
            auto y = x;
        }
        {
            auto x = info.grid_type();
            auto y = x;
        }
        {
            auto x = info.object_file_name();
            auto y = x;
        }
        //{
        //    auto x = info.origin();
        //}
        {
            auto x = info.resolution();
            auto y = x;
        }
        {
            auto x = info.slice_thickness();
            auto y = x;
        }
        //{
        //    auto x = info.tetrahedra();
        //}
        //{
        //    auto x = info.time_steps();
        //}
        //{
        //    auto x = info.vertices();
        //}

        std::vector<int> x = { 23, 23, 23,434, 1234 };
        datraw::swap_byte_order<sizeof(int)>(x.data(), x.size());
        std::vector<__int64> y;
        datraw::convert<__int64>(x.begin(), x.end(), std::back_inserter(y));
        datraw::convert(x.begin(), x.end(), y.begin());
        datraw::swap_byte_order<sizeof(__int64)>(y.data(), y.size());

        {
            datraw::raw_reader<char> r(info);
            auto x = r.read_current(nullptr, 0);
            auto y = x;
        }


        auto winfo = datraw::info<wchar_t>::parse(L"S:\\Daten\\Volumen\\tuebingen\\foot.dat");
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}

