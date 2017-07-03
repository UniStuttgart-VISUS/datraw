
# datraw
This is a C++ header-only reimplementation of VIS's datraw library for volumetric data. The data sets are split in two files, the dat file containing a textual description of the data and the raw file which contains the actual volumetric data without any meta information.

## The dat file
The dat file holds properties describing the raw data. Each property must be in a separate line. The name of the property and its value are separated by a colon (":").

In principle, arbitrary properties can be stored. However, some properties have special meaning and some are considered to be required. The well-known properties are:

| Property name | Type | Description |
| --- | --- | --- |
| OBJECTFILENAME | String | The name(s) of the raw file(s). For a single raw file this is just the file name, for multiple raw files, forming for example a time-series, a the numbering is controlled by a format string. |
| FORMAT | Enum | The format (or data type) of a single element of the tuples (i.e. the scalar type) |
| GRIDTYPE | Enum | The type of grid the data is organised in. |
| COMPONENTS | Integer | The number of components per tuple. |
| DIMENSIONS | Integer | The dimensionality M of the grid. |
| TIMESTEPS | Integer | The number of time steps/number of raw files (defaults to 1). |
| BYTEORDER | Enum | The byte order the raw files are stored in; either LITTLE_ENDIAN (default) or BIG_ENDIAN. |
| DATAOFFSET | Integer | The offset in the raw file(s) where the actual data starts (defaults to 0). |
| RESOLUTION | Array of Integer | The resolution of the grid, i.e. the number of elements in each dimension. |
| SLICETHICKNESS | Array of Float | The size of the grid cells in each direction/dimension (defaults to 1.0 for each dimension). |

## The raw file
The raw file stores binary as an M-dimensional array of N-dimensional tuples. All elements of the tuple need to have the same type.