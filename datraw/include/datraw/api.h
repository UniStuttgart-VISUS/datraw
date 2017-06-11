/// <copyright file="api.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
/// Copyright � 2017 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph M�ller</author>

#pragma once

#if (defined(_MSC_VER) && !defined(DATRAW_STATIC))

#ifdef DATRAW_EXPORTS
#define DATRAW_API __declspec(dllexport)
#else /* DATRAW_EXPORTS */
#define DATRAW_EXPORTS __declspec(dllimport)
#endif /* DATRAW_EXPORTS*/

#else /* (defined(_MSC_VER) && defined(TRROJANCORE_DLL)) */

#define DATRAW_API

#endif /* (defined(_MSC_VER) && !defined(DATRAW_STATIC)) */
