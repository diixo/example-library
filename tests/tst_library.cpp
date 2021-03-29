/*
 *   Copyright (C)
 *
 *   This Source Code Form is subject to the terms of the Mozilla Public
 *   License, v. 2.0. If a copy of the MPL was not distributed with this
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *   SPDX-License-Identifier: MPL-2.0
 *
 */

#include "examplepublicclass.h"

#include <gtest/gtest.h>
#include <typeinfo>

TEST(Library, class_instantiation)
{
    ExamplePublicClass obj;
    std::string expectedType("19ExamplePublicClass");
    EXPECT_TRUE((std::is_same<decltype(obj),ExamplePublicClass>::value));
}