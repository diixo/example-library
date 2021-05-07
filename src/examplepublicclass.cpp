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

#include "examplepublicclass.hpp"
#include "exampleprivateclass.hpp"

#include <iostream>

ExamplePublicClass::ExamplePublicClass() :
    examplePrivateInstance(new ExamplePrivateClass)
{
    std::cout << "ExamplePublicClass::ExamplePublicClass()\n";
}

ExamplePublicClass::~ExamplePublicClass()
{
    std::cout << "ExamplePublicClass::~ExamplePublicClass()\n";
}

void ExamplePublicClass::exampleFunction()
{
    examplePrivateInstance->exampleFunction();
}
