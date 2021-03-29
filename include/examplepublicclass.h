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

#ifndef EXAMPLEPUBLICCLASS_H_
#define EXAMPLEPUBLICCLASS_H_

#include <memory>

class ExamplePrivateClass;

class ExamplePublicClass
{
public:
    ExamplePublicClass();
    ~ExamplePublicClass();

    void exampleFunction();

protected:

private:
    std::shared_ptr<ExamplePrivateClass> examplePrivateInstance;
};

#endif // EXAMPLEPUBLICCLASS_H_
