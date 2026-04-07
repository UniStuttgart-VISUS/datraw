// <copyright file="types.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <array>
#include <cinttypes>

#include "datraw/half.h"


DATRAW_NAMESPACE_BEGIN

typedef std::int8_t int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;

typedef std::uint8_t uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;

typedef half float16;
typedef float float32;
typedef double float64;

DATRAW_NAMESPACE_END
