
# datraw

[![Build Status](https://visualisierungsinstitut.visualstudio.com/datraw/_apis/build/status/UniStuttgart-VISUS.datraw?branchName=master)](https://visualisierungsinstitut.visualstudio.com/datraw/_build/latest?definitionId=5&branchName=master)

This is a C++ header-only reimplementation of VIS's datraw library for volumetric data. The data sets are split in two files, the dat file containing a textual description of the data and the raw file which contains the actual volumetric data without any meta information.

## The dat file
The dat file holds properties describing the raw data. Each property must be in a separate line. The name of the property and its value are separated by a colon (":").

In principle, arbitrary properties can be stored. However, some properties have special meaning and some are considered to be required. The well-known properties are:

| Property name | Type | Description |
| --- | --- | --- |
| OBJECTFILENAME | String | The name(s) of the raw file(s). For a single raw file this is just the file name, for multiple raw files, forming for example a time-series, a the numbering is controlled by a format string. |
| FORMAT | Enum | The format (or data type) of a single element of the tuples (i.e. the scalar type) |
| GRIDTYPE | Enum | The type of grid the data is organised in. |
| COMPONENTS | Integer | The number N of components per tuple. |
| DIMENSIONS | Integer | The dimensionality M of the grid. |
| TIMESTEPS | Integer | The number of time steps/number of raw files (defaults to 1). |
| BYTEORDER | Enum | The byte order the raw files are stored in; either LITTLE_ENDIAN (default) or BIG_ENDIAN. |
| DATAOFFSET | Integer | The offset, in bytes, in the raw file(s) where the actual data starts (defaults to 0). |
| RESOLUTION | Array of Integer | The resolution of the grid, i.e. the number of elements in each dimension. |
| SLICETHICKNESS | Array of Float | The size of the grid cells in each direction/dimension (defaults to 1.0 for each dimension). |
| SLICETHICKNESS[a] | Array of Float | The distances between the grid cells on axis a for a rectilinear grid (defaults to 1.0 for all missing values). |

## The raw file
The raw file stores binary data as an M-dimensional array of N-dimensional tuples. All elements of the tuple need to have the same type.

## Usage
In order to use the library in your project, add the **datraw** folder to the list of `#include` directories. Add `#include "datraw.h"`. All other files are included via this file.

The library represents the dat file in the `info` class and provides access to the raw file(s) by means of the `raw_reader` class. The `raw_reader` can either be created from the path to a dat file or from an existing in-memory `info` instance. An `info` object can be parsed from the path to a dat file or from an in-memory string (e.g. if the data have been received from the network).

All classes are templated with the character type (`char` or `wchar_t`) and are located in the `datraw` namespace.

The following example shows the minimal example for iterating over all time steps in the data set described by "foot.dat":

```C++
#include "datraw.h"

typedef datraw::raw_reader<char> reader;

auto r = reader::open("foot.dat");
while (r) {
    std::vector<datraw::uint8> raw = r.read_current();
    r.move_next();
}
```

The variant with an explicit instance of the `info` class looks like:


```C++
#include "datraw.h"

typedef datraw::info<char> info;
typedef datraw::raw_reader<char> reader;

auto info = info::load("foot.dat");
reader r(info);
for (std::uint64_t i = 0; i < info.time_steps(); ++i) {
    r.move_to(i);
    assert(r);
    std::vector<datraw::uint8> raw = r.read_current();
}
```

There is a zero-copy overload of `raw_reader::read_current` which uses a user-provided buffer and returns the required buffer size. It can be used as follows:

```C++
#include "datraw.h"

typedef datraw::raw_reader<char> reader;

std::vector<datraw::uint8> frame;

auto r = reader::open("foot.dat");
while (r) {
    frame.resize(reader.read_current(nullptr, 0));
    reader.read_current(retval.data(), retval.size());
    r.move_next();
}
```
