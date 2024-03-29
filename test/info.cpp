﻿// <copyright file="info.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(Info) {

    public:

        TEST_METHOD(TestParseChar) {
            this->testParse<char>();
        }

        TEST_METHOD(TestParseWchar) {
            this->testParse<wchar_t>();
        }

    private:

        template<class C> void testParse(void) {
            typedef datraw::info<C> info;

            {
                auto input = DATRAW_TPL_LITERAL(C, "");
                auto fn = [&input] {
                    auto i = info::parse(input);
                };
                Assert::ExpectException<std::runtime_error>(fn, L"Empty string yields invalid info.");
            }

            {
                auto input = DATRAW_TPL_LITERAL(C, "\
ObjectFileName: foot.raw\n\
TaggedFileName: ---\n\
Resolution: 256 256 256\n\
SliceThickness: 1 1 1\n\
Format: UCHAR\n\
NbrTags: 0\n\
ObjectType: TEXTURE_VOLUME_OBJECT\n\
ObjectModel: RGBA\n\
GridType: EQUIDISTANT\n\
");
                auto i = info::parse(input);
                Assert::IsTrue(i.object_file_name() == DATRAW_TPL_LITERAL(C, "foot.raw"), L"Object file name was parsed correctly.", LINE_INFO());
                Assert::IsTrue(i.resolution().size() == 3, L"Sufficient resolution  parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[0] == 256, L"Correct resolution in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[1] == 256, L"Correct resolution in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[2] == 256, L"Correct resolution in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness().size() == 3, L"Sufficient slice thicknesses parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[0] == 1, L"Correct slice thickness in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[1] == 1, L"Correct slice thickness in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[2] == 1, L"Correct slice thickness in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.format() == datraw::scalar_type::uint8, L"Correct scalar format parsed.", LINE_INFO());
                Assert::IsTrue(i.grid_type() == datraw::grid_type::cartesian, L"Correct grid type parsed.", LINE_INFO());
                Assert::IsTrue(i.dimensions() == 3, L"Correct default dimension of data set resolved.", LINE_INFO());
                Assert::IsTrue(i.components() == 1, L"Correct default number of components resolved.", LINE_INFO());
                Assert::IsTrue(i.time_steps() == 1, L"Correct default number of time steps resolved.", LINE_INFO());
                Assert::IsTrue(i.byte_order() == datraw::endianness::little, L"Correct default byte order resolved.", LINE_INFO());
                Assert::AreEqual(std::size_t(256), i.row_size(), L"Computed correct row size.", LINE_INFO());
                Assert::AreEqual(i.row_size(), i.row_pitch(), L"Computed correct row pitch.", LINE_INFO());
                Assert::AreEqual(std::size_t(256), i.row_pitch(256), L"Computed correct aligned row pitch.", LINE_INFO());
            }


            {
                auto input = DATRAW_TPL_LITERAL(C, "\
ObjectFileName: fullbody288x168x939.raw\n\
TaggedFileName: ---\n\
Resolution:     288 168 939\n\
SliceThickness: 1 1 1\n\
Format:         UCHAR\n\
NbrTags:        0\n\
ObjectType:     TEXTURE_VOLUME_OBJECT\n\
ObjectModel:    DENSITY\n\
GridType:       EQUIDISTANT\n\
Components: 4\n\
");
                auto i = info::parse(input);
                Assert::IsTrue(i.object_file_name() == DATRAW_TPL_LITERAL(C, "fullbody288x168x939.raw"), L"Object file name was parsed correctly.", LINE_INFO());
                Assert::IsTrue(i.resolution().size() == 3, L"Sufficient resolution  parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[0] == 288, L"Correct resolution in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[1] == 168, L"Correct resolution in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[2] == 939, L"Correct resolution in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness().size() == 3, L"Sufficient slice thicknesses parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[0] == 1, L"Correct slice thickness in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[1] == 1, L"Correct slice thickness in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[2] == 1, L"Correct slice thickness in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.format() == datraw::scalar_type::uint8, L"Correct scalar format parsed.", LINE_INFO());
                Assert::IsTrue(i.grid_type() == datraw::grid_type::cartesian, L"Correct grid type parsed.", LINE_INFO());
                Assert::IsTrue(i.components() == 4, L"Correct number of components parsed.", LINE_INFO());
                Assert::IsTrue(i.dimensions() == 3, L"Correct default dimension of data set resolved.", LINE_INFO());
                Assert::IsTrue(i.time_steps() == 1, L"Correct default number of time steps resolved.", LINE_INFO());
                Assert::IsTrue(i.byte_order() == datraw::endianness::little, L"Correct default byte order resolved.", LINE_INFO());
                Assert::AreEqual(std::size_t(288 * 4), i.row_size(), L"Computed correct row size.", LINE_INFO());
                Assert::AreEqual(i.row_size(), i.row_pitch(), L"Computed correct row pitch.", LINE_INFO());
                Assert::AreEqual(std::size_t(1280), i.row_pitch(256), L"Computed correct aligned row pitch.", LINE_INFO());
            }

            {
                auto input = DATRAW_TPL_LITERAL(C, "\
ObjectFileName:	big_brown_bat.raw\n\
Resolution: 1024 1024 720\n\
SliceThickness: 0.0439453125 0.0439453125 0.1073\n\
Format:	USHORT\n\
");
                auto i = info::parse(input);
                Assert::IsTrue(i.object_file_name() == DATRAW_TPL_LITERAL(C, "big_brown_bat.raw"), L"Object file name was parsed correctly.", LINE_INFO());
                Assert::IsTrue(i.resolution().size() == 3, L"Sufficient resolution  parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[0] == 1024, L"Correct resolution in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[1] == 1024, L"Correct resolution in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[2] == 720, L"Correct resolution in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness().size() == 3, L"Sufficient slice thicknesses parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness()[0] - 0.0439453125) < 0.0001, L"Correct slice thickness in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness()[1] - 0.0439453125) < 0.0001, L"Correct slice thickness in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness()[2] - 0.1073) < 0.0001, L"Correct slice thickness in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.format() == datraw::scalar_type::uint16, L"Correct scalar format parsed.", LINE_INFO());
                Assert::IsTrue(i.grid_type() == datraw::grid_type::cartesian, L"Correct default grid type resolved.", LINE_INFO());
                Assert::IsTrue(i.dimensions() == 3, L"Correct default dimension of data set resolved.", LINE_INFO());
                Assert::IsTrue(i.components() == 1, L"Correct default number of components resolved.", LINE_INFO());
                Assert::IsTrue(i.time_steps() == 1, L"Correct default number of time steps resolved.", LINE_INFO());
                Assert::IsTrue(i.byte_order() == datraw::endianness::little, L"Correct default byte order resolved.", LINE_INFO());
                Assert::AreEqual(std::size_t(2048), i.row_size(), L"Computed correct row size.", LINE_INFO());
                Assert::AreEqual(i.row_size(), i.row_pitch(), L"Computed correct row pitch.", LINE_INFO());
                Assert::AreEqual(std::size_t(2048), i.row_pitch(256), L"Computed correct aligned row pitch.", LINE_INFO());

            }

            {
                auto input = DATRAW_TPL_LITERAL(C, "\
    ObjectFileName:	veiled-chameleon.u8.raw\n\
    Resolution:	1024 1024 1080\n\
    SliceThickness:	0.0016276 0.0016276 0.00185185\n\
    Format:		UCHAR\n\
    ObjectModel:	I\n\
    Modality:	unknown\n\
    Checksum:	4c830a96d2db5c779d6f10ff31f1dd21\n\
");
                auto i = info::parse(input);
                Assert::IsTrue(i.object_file_name() == DATRAW_TPL_LITERAL(C, "veiled-chameleon.u8.raw"), L"Object file name was parsed correctly.", LINE_INFO());
                Assert::IsTrue(i.resolution().size() == 3, L"Sufficient resolution  parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[0] == 1024, L"Correct resolution in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[1] == 1024, L"Correct resolution in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[2] == 1080, L"Correct resolution in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness().size() == 3, L"Sufficient slice thicknesses parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness()[0] - 0.0016276) < 0.0001, L"Correct slice thickness in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness()[1] - 0.0016276) < 0.0001, L"Correct slice thickness in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness()[2] - 0.00185185) < 0.0001, L"Correct slice thickness in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.format() == datraw::scalar_type::uint8, L"Correct scalar format parsed.", LINE_INFO());
                Assert::IsTrue(i.grid_type() == datraw::grid_type::cartesian, L"Correct default grid type resolved.", LINE_INFO());
                Assert::IsTrue(i.dimensions() == 3, L"Correct default dimension of data set resolved.", LINE_INFO());
                Assert::IsTrue(i.components() == 1, L"Correct default number of components resolved.", LINE_INFO());
                Assert::IsTrue(i.time_steps() == 1, L"Correct default number of time steps resolved.", LINE_INFO());
                Assert::IsTrue(i.byte_order() == datraw::endianness::little, L"Correct default byte order resolved.", LINE_INFO());
                Assert::AreEqual(std::size_t(1024), i.row_size(), L"Computed correct row size.", LINE_INFO());
                Assert::AreEqual(i.row_size(), i.row_pitch(), L"Computed correct row pitch.", LINE_INFO());
                Assert::AreEqual(std::size_t(1024), i.row_pitch(256), L"Computed correct aligned row pitch.", LINE_INFO());

            }

            {
                auto input = DATRAW_TPL_LITERAL(C, "\
    ObjectFileName: dummy.raw\r\n\
    Resolution: 4 4 2\r\n\
    SliceThickness: 1 2 3\r\n\
    SliceThickness[0]: 1.1 1.2 1.3 1.4\r\n\
    SliceThickness[1]: 2.1 2.2 2.3 2.4\r\n\
    SliceThickness[2]: 3.1 3.2\r\n\
    Format: UCHAR\r\n\
    GridType: rectilinear\n\
");
                auto i = info::parse(input);
                Assert::IsTrue(i.object_file_name() == DATRAW_TPL_LITERAL(C, "dummy.raw"), L"Object file name was parsed correctly.", LINE_INFO());
                Assert::IsTrue(i.resolution().size() == 3, L"Sufficient resolution  parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[0] == 4, L"Correct resolution in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[1] == 4, L"Correct resolution in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[2] == 2, L"Correct resolution in z-direction parsed.", LINE_INFO());
                Assert::IsFalse(i.contains(info::property_slice_thickness), L"Standard slicke thickness has been removed", LINE_INFO());
                Assert::AreEqual(std::size_t(4), i.slice_thickness(0).size(), L"Number of slices on x-axis.", LINE_INFO());
                Assert::AreEqual(std::size_t(4), i.slice_thickness(1).size(), L"Number of slices on y-axis.", LINE_INFO());
                Assert::AreEqual(std::size_t(2), i.slice_thickness(2).size(), L"Number of slices on z-axis.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[0] - 1.1f) < 0.0001, L"Correct slice thickness [0] in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[1] - 1.2f) < 0.0001, L"Correct slice thickness [1] in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[2] - 1.3f) < 0.0001, L"Correct slice thickness [2] in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[3] - 1.4f) < 0.0001, L"Correct slice thickness [3] in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[0] - 2.1f) < 0.0001, L"Correct slice thickness [0] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[1] - 2.2f) < 0.0001, L"Correct slice thickness [1] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[2] - 2.3f) < 0.0001, L"Correct slice thickness [2] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[3] - 2.4f) < 0.0001, L"Correct slice thickness [3] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(2)[0] - 3.1f) < 0.0001, L"Correct slice thickness [0] in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(2)[1] - 3.2f) < 0.0001, L"Correct slice thickness [0] in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.format() == datraw::scalar_type::uint8, L"Correct scalar format parsed.", LINE_INFO());
                Assert::IsTrue(i.grid_type() == datraw::grid_type::rectilinear, L"Correct grid type parsed.", LINE_INFO());
                Assert::AreEqual(std::size_t(4), i.row_size(), L"Computed correct row size.", LINE_INFO());
                Assert::AreEqual(i.row_size(), i.row_pitch(), L"Computed correct row pitch.", LINE_INFO());
                Assert::AreEqual(std::size_t(256), i.row_pitch(256), L"Computed correct aligned row pitch.", LINE_INFO());
            }

            {
                auto input = DATRAW_TPL_LITERAL(C, "\
    ObjectFileName: dummy.raw\r\n\
    Resolution: 4 4 2\r\n\
    SliceThickness[0]: 1.1 1.2 1.3\r\n\
    SliceThickness[1]: 2.1 2.2 2.3 2.4 2.5\r\n\
    Format: UCHAR\r\n\
    GridType: rectilinear\n\
");
                auto i = info::parse(input);
                Assert::IsTrue(i.object_file_name() == DATRAW_TPL_LITERAL(C, "dummy.raw"), L"Object file name was parsed correctly.", LINE_INFO());
                Assert::IsTrue(i.resolution().size() == 3, L"Sufficient resolution  parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[0] == 4, L"Correct resolution in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[1] == 4, L"Correct resolution in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[2] == 2, L"Correct resolution in z-direction parsed.", LINE_INFO());
                Assert::AreEqual(std::size_t(4), i.slice_thickness(0).size(), L"Number of slices on x-axis.", LINE_INFO());
                Assert::AreEqual(std::size_t(4), i.slice_thickness(1).size(), L"Number of slices on y-axis.", LINE_INFO());
                Assert::AreEqual(std::size_t(2), i.slice_thickness(2).size(), L"Number of slices on z-axis.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[0] - 1.1f) < 0.0001, L"Correct slice thickness [0] in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[1] - 1.2f) < 0.0001, L"Correct slice thickness [1] in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[2] - 1.3f) < 0.0001, L"Correct slice thickness [2] in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(0)[3] - 1.0f) < 0.0001, L"Default for slice thickness [3] in x-direction.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[0] - 2.1f) < 0.0001, L"Correct slice thickness [0] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[1] - 2.2f) < 0.0001, L"Correct slice thickness [1] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[2] - 2.3f) < 0.0001, L"Correct slice thickness [2] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(1)[3] - 2.4f) < 0.0001, L"Correct slice thickness [3] in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(2)[0] - 1.0f) < 0.0001, L"Missing slice thickness [0] in z-direction is default.", LINE_INFO());
                Assert::IsTrue(std::abs(i.slice_thickness(2)[1] - 1.0f) < 0.0001, L"Missing slice thickness [0] in z-direction is default.", LINE_INFO());
                Assert::IsTrue(i.format() == datraw::scalar_type::uint8, L"Correct scalar format parsed.", LINE_INFO());
                Assert::IsTrue(i.grid_type() == datraw::grid_type::rectilinear, L"Correct grid type parsed.", LINE_INFO());
                Assert::AreEqual(std::size_t(4), i.row_size(), L"Computed correct row size.", LINE_INFO());
                Assert::AreEqual(i.row_size(), i.row_pitch(), L"Computed correct row pitch.", LINE_INFO());
                Assert::AreEqual(std::size_t(256), i.row_pitch(256), L"Computed correct aligned row pitch.", LINE_INFO());

            }

            {
                auto input = DATRAW_TPL_LITERAL(C, "\
    ObjectFileName:	funs%05+0*100d\n\
    TaggedFileName:	---\n\
    Resolution: 256 256 256\n\
    Format:	UCHAR\n\
    TimeSteps: 8\n\
");

                auto i = info::parse(input);
                Assert::IsTrue(i.object_file_name() == DATRAW_TPL_LITERAL(C, "funs%05+0*100d"), L"Object file name was parsed correctly.", LINE_INFO());
                Assert::IsTrue(i.multi_file_name(0) == DATRAW_TPL_LITERAL(C, "funs00000"), L"Multi-file 0.", LINE_INFO());
                Assert::IsTrue(i.multi_file_name(1) == DATRAW_TPL_LITERAL(C, "funs00100"), L"Multi-file 0.", LINE_INFO());
                Assert::IsTrue(i.resolution().size() == 3, L"Sufficient resolution  parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[0] == 256, L"Correct resolution in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[1] == 256, L"Correct resolution in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.resolution()[2] == 256, L"Correct resolution in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness().size() == 3, L"Sufficient slice thicknesses parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[0] == 1, L"Correct slice thickness in x-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[1] == 1, L"Correct slice thickness in y-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.slice_thickness()[2] == 1, L"Correct slice thickness in z-direction parsed.", LINE_INFO());
                Assert::IsTrue(i.format() == datraw::scalar_type::uint8, L"Correct scalar format parsed.", LINE_INFO());
                Assert::IsTrue(i.grid_type() == datraw::grid_type::cartesian, L"Correct default grid type resolved.", LINE_INFO());
                Assert::IsTrue(i.dimensions() == 3, L"Correct default dimension of data set resolved.", LINE_INFO());
                Assert::IsTrue(i.components() == 1, L"Correct default number of components resolved.", LINE_INFO());
                Assert::IsTrue(i.time_steps() == 8, L"Correct default number of time steps resolved.", LINE_INFO());
                Assert::IsTrue(i.byte_order() == datraw::endianness::little, L"Correct default byte order resolved.", LINE_INFO());
                Assert::AreEqual(std::size_t(256), i.row_size(), L"Computed correct row size.", LINE_INFO());
                Assert::AreEqual(i.row_size(), i.row_pitch(), L"Computed correct row pitch.", LINE_INFO());
                Assert::AreEqual(std::size_t(256), i.row_pitch(256), L"Computed correct aligned row pitch.", LINE_INFO());

            }

        }
    };
}
