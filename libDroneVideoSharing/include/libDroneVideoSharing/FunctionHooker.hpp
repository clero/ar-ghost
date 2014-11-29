/*
 * Copyright 2014 Jules Cléro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include <dlfcn.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <stdexcept>

namespace ghost
{
namespace libDroneVideoSharing
{

/** Helper class to interpose symbols from a given library */
class FunctionHooker
{
public:

    /**
     * Allows to load a symbol dynamically from a library.
     *
     * @tparam F the function pointer type of the function to hook
     * @param[in] libToInterpose the library file to hook
     * @param[in] symbolToHook the symbol to hook
     *
     */
    template <typename F>
    static F hookSetup(const std::string& libToInterpose, const std::string& symbolToHook)
    {
        void* handle;

        handle = dlopen(libToInterpose.c_str(), RTLD_LAZY);
        if (!handle) {
            // fprintf(stderr,
            std::cerr << "When opening " << libToInterpose
                      << " library: " << dlerror()
                      << std::endl;
            throw std::runtime_error("dlopen error, please see logs");
        }

        F hookedFunction = reinterpret_cast<F>(dlsym(handle, symbolToHook.c_str()));
        if (!hookedFunction) {
            std::cerr << "When getting " << symbolToHook
                      << " symbol: " << dlerror()
                      << std::endl;
            throw std::runtime_error("dsym error, please see logs");
        }

        return hookedFunction;
    }

private:

    FunctionHooker();
    ~FunctionHooker();
};

} /* libDroneVideoSharing namespace */
} /* ghost namespace  */
